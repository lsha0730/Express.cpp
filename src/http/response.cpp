#include "core/router.h"
#include "http/byte_conversion.h"
#include "http/http_status.h"
#include "http/url_codec.h"
#include <express/concepts.h>
#include <express/metadata.h>
#include <express/response.h>

#include <algorithm>
#include <ctime>
#include <fmt/format.h>
#include <functional>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace express {

class Response::Impl {
public:
  Impl(std::function<void(const std::vector<char>)> write_to_socket,
       std::function<void()> close_socket) {
    write_to_socket_ = write_to_socket;
    close_socket_ = close_socket;
    set_defaults();
  };

  template <NullLike T> void send(const T &body) { send_bytes({}); }

  template <BufferLike T> void send(const T &body) {
    set("Content-Type", "application/octet-stream");
    std::vector<char> bytes = express::to_bytes(body);
    send_bytes(bytes);
  }

  template <StringLike T>
    requires(!BufferLike<T> && !NullLike<T>)
  void send(const T &body) {
    set("Content-Type", "text/html; charset=utf-8");
    std::vector<char> bytes = express::to_bytes(body);
    send_bytes(bytes);
  }

  template <BoolLike T>
    requires(!StringLike<T>)
  void send(const T &body) {
    set("Content-Type", "text/html; charset=utf-8");
    std::string serialized = (body) ? "true" : "false";
    std::vector<char> bytes = express::to_bytes(serialized);
    send_bytes(bytes);
  }

  template <NumberLike T>
    requires(!StringLike<T> && !BoolLike<T>)
  void send(const T &body) {
    if constexpr (std::is_same<T, int>::value) {
      if (HttpStatus::is_valid(body)) {
        status(body);
        send_bytes({});
        return;
      }
    }
    set("Content-Type", "text/html; charset=utf-8");
    std::vector<char> bytes = express::to_bytes(std::to_string(body));
    send_bytes(bytes);
  }

  void send(const nlohmann::json &body) { json(body); }

  template <ObjectLike T> void send(const T &body) {
    set("Content-Type", "application/json; charset=utf-8");
    std::string serialized = nlohmann::json(body).dump(4);
    std::vector<char> bytes = express::to_bytes(serialized);
    send_bytes(bytes);
  }

  template <JsonLike T>
    requires(!Sendable<T>)
  void send(const T &data) {
    json(nlohmann::json{data});
  }

  void json(const nlohmann::json &data) {
    std::string serialized = data.dump(4);
    json_str(serialized);
  }

  void json_str(const std::string &data) {
    check_sendable();
    set("Content-Type", "application/json; charset=utf-8", false);
    std::vector<char> bytes(data.begin(), data.end());
    send_bytes(bytes);
  }

  void status(int code) {
    status_code_ = code;
    status_message_ = HttpStatus::get_message(code);
  }

  /**
   * Sets a response header.
   * @param header
   * @param value
   * @param overwrite If false, keeps existing header (default: true).
   */
  void set(const std::string &header, const std::string &value, bool overwrite = true) {
    if (!overwrite && (headers_.find(header) != headers_.end())) {
      return;
    }
    headers_[header] = value;
  }

  std::string get(const std::string &header) {
    if (headers_.find(header) == headers_.end()) {
      throw std::runtime_error(fmt::format("Header {} does not exist", header));
    }
    return headers_[header];
  }

  void end() {
    headers_sent_ = true;
    close_socket_();
  }

  int status_code() { return status_code_; }

  bool headers_sent() { return headers_sent_; }

private:
  /* Response headers */
  std::unordered_map<std::string, std::string> headers_;

  /* Boolean indicating if headers have been sent */
  bool headers_sent_;

  /* Object for passing data through middleware to views */
  //   locals

  /* HTTP status code (e.g., 200, 404, 500) */
  int status_code_;

  /* HTTP status message (e.g., "OK", "Not Found") */
  std::string status_message_;

  /* HTTP Version (1.1 by default) */
  static const inline std::string http_version_ = "1.1";

  /* Callback registered by server to write to socket */
  std::function<void(const std::vector<char>)> write_to_socket_;

  /* Callback registered by server to close the socket */
  std::function<void()> close_socket_;

  /**
   * Sets default values for the object
   * @private
   */
  void set_defaults() {
    status(200);
    std::string server_info =
        fmt::format("{}/{} ({})", metadata::SERVER_NAME, metadata::VERSION, get_os_name());
    set("Server", server_info, false);
    headers_sent_ = false;
  }

  /**
   * Sends the response back to the client.
   * @param body Bytes to be sent as the response body.
   * @warning Does not check if a response has been sent already.
   * @private
   */
  void send_bytes(std::vector<char> body) {
    set("Content-Length", std::to_string(body.size()), false);
    set("Date", get_http_date_string(), true);

    std::vector<char> http_response = build_http_response(body);
    write_to_socket_(http_response);
    headers_sent_ = true;
    close_socket_();
  }

  /**
   * Checks if the response is locked.
   * @throws Runtime error if response has already been sent.
   * @private
   */
  void check_sendable() {
    if (headers_sent_) {
      throw new std::runtime_error("Cannot set headers after they are sent.");
    }
  }

  /**
   * Builds the complete HTTP response string.
   * @param body The response body.
   * @returns Complete HTTP response
   * @private
   * @todo Factor out into separate class
   */
  std::vector<char> build_http_response(std::vector<char> body) {
    std::string status_line = build_status_line();
    std::string headers = build_headers();
    std::string head = fmt::format("{}\r\n{}\r\n\r\n", status_line, headers);

    std::vector<char> response;
    response.reserve(head.size() + body.size());
    response.insert(response.end(), head.begin(), head.end());
    response.insert(response.end(), body.begin(), body.end());

    return response;
  }

  /**
   * Builds the HTTP status line.
   * @returns Status line in format "HTTP/1.1 200 OK"
   * @private
   */
  std::string build_status_line() {
    std::string http_version = "HTTP/" + http_version_;
    std::string status_code = std::to_string(status_code_);
    return fmt::format("{} {} {}", http_version, status_code, status_message_);
  }

  /**
   * Builds all response headers as a single string.
   * @returns Headers string with each header on a new line
   * @private
   */
  std::string build_headers() {
    std::string header_lines;
    bool is_first = true;
    for (const auto &[key, value] : headers_) {
      if (!is_first) {
        header_lines += "\r\n";
      }
      header_lines += fmt::format("{}: {}", key, value);
      is_first = false;
    }
    return header_lines;
  }

  /**
   * Generates a date string in the HTTP response header format.
   * @returns A date in the [Day, DD Mth YYYY HH:MM:SS GMT] format
   * @example Thu, 19 Apr 2025 12:00:00 GMT
   * @private
   */
  std::string get_http_date_string() {
    std::time_t now = std::time(nullptr);
    std::tm *gmt = std::gmtime(&now);
    std::ostringstream oss;
    oss << std::put_time(gmt, "%a, %d %b %Y %H:%M:%S GMT");
    return oss.str();
  }

  /**
   * Gets the current OS name in a format suitable for HTTP headers.
   * @returns OS name string
   * @private
   */
  std::string get_os_name() {
#ifdef _WIN64
    return "Windows";
#elif _WIN32
    return "Windows";
#elif __APPLE__ || __MACH__
    return "macOS";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Unknown";
#endif
  }
}; // namespace Response::Impl

// Constructor
Response::Response(std::function<void(const std::vector<char>)> write_to_socket,
                   std::function<void()> close_socket)
    : pImpl(std::make_unique<Impl>(write_to_socket, close_socket)) {
}

// Destructor
Response::~Response() = default;

template <Sendable T> Response &Response::send(const T &data) {
  pImpl->send(data);
  return *this;
}

Response &Response::json(const nlohmann::json &data) {
  pImpl->json(data);
  return *this;
}

Response &Response::json_str(const std::string &data) {
  pImpl->json_str(data);
  return *this;
}

Response &Response::status(int code) {
  pImpl->status(code);
  return *this;
}

Response &Response::set(const std::string &header, const std::string &value) {
  pImpl->set(header, value, true);
  return *this;
}

std::string Response::get(const std::string &header) {
  return pImpl->get(header);
}

void Response::end() {
  pImpl->end();
}

int Response::status_code() {
  return pImpl->status_code();
}

bool Response::headers_sent() {
  return pImpl->headers_sent();
}

// Explicit template instantiations
// BufferLike types
template Response &Response::send<std::vector<char>>(const std::vector<char> &);
template Response &Response::send<std::vector<uint8_t>>(const std::vector<uint8_t> &);
template Response &Response::send<std::vector<std::byte>>(const std::vector<std::byte> &);
template Response &Response::send<std::span<char>>(const std::span<char> &);
template Response &Response::send<std::span<uint8_t>>(const std::span<uint8_t> &);
template Response &Response::send<std::span<std::byte>>(const std::span<std::byte> &);

// StringLike types
template Response &Response::send<std::string>(const std::string &);
template Response &Response::send<std::string_view>(const std::string_view &);
template Response &Response::send<char>(const char &);
template Response &Response::send<std::filesystem::path>(const std::filesystem::path &);

// BoolLike type
template Response &Response::send<bool>(const bool &);

// NumberLike types (excluding char since it's already covered by StringLike)
template Response &Response::send<signed char>(const signed char &);
template Response &Response::send<unsigned char>(const unsigned char &);
template Response &Response::send<short>(const short &);
template Response &Response::send<unsigned short>(const unsigned short &);
template Response &Response::send<int>(const int &);
template Response &Response::send<unsigned int>(const unsigned int &);
template Response &Response::send<long>(const long &);
template Response &Response::send<unsigned long>(const unsigned long &);
template Response &Response::send<long long>(const long long &);
template Response &Response::send<unsigned long long>(const unsigned long long &);
template Response &Response::send<float>(const float &);
template Response &Response::send<double>(const double &);
template Response &Response::send<long double>(const long double &);

// NullLike type
template Response &Response::send<std::nullptr_t>(const std::nullptr_t &);

// MapLike types
template Response &
Response::send<std::map<std::string, std::string>>(const std::map<std::string, std::string> &);
template Response &Response::send<std::unordered_map<std::string, std::string>>(
    const std::unordered_map<std::string, std::string> &);

// Iterable types
template Response &Response::send<std::vector<int>>(const std::vector<int> &);
template Response &Response::send<std::vector<double>>(const std::vector<double> &);
template Response &Response::send<std::vector<std::string>>(const std::vector<std::string> &);

// Json type
template Response &Response::send<nlohmann::json>(const nlohmann::json &);

} // namespace express

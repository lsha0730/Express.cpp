// #include "request.h"
#include "include/flash/response.h"
#include "core/router.h"
#include "http/http_status.h"
#include "http/url_codec.h"
#include "include/flash/metadata.h"

#include <fmt/core.h>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

namespace flash {
class Response::Impl {
public:
  Impl(std::function<void(const std::vector<char>)> write_to_socket,
       std::function<void()> close_socket) {
    write_to_socket_ = write_to_socket;
    close_socket_ = close_socket;
    set_defaults();
  };

  template <typename T> void send(const T &data) {
    check_sendable();
    set("Content-Type", "application/json; charset=utf-8", false);
    send_bytes(data);
  }

  void json(const std::string &data) {
    check_sendable();
    set("Content-Type", "application/json; charset=utf-8", false);
    std::vector<char> bytes(data.begin(), data.end());
    send_bytes(bytes);
  }

  void status(int code) {
    status_code_ = code;
    status_message_ = HttpStatus::getMessage(code);
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

  void end() {
    headers_sent_ = true;
    close_socket_();
  }

  /**
   * @returns The HTTP status code.
   * @note Deviates from Express.js API in the name of cpp best practices!
   */
  int status_code() { return status_code_; }

  /**
   * @returns A boolean indicating whether or not the response has been sent.
   * @note Deviates from Express.js API in the name of cpp best practices!
   */
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

template <typename T> Response &send(const T &data) {
  // pImpl->send(data);
  // return *this;
}

Response &Response::json_(const std::string &data) {
  pImpl->json(data);
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

void Response::end() {
  pImpl->end();
}
} // namespace flash

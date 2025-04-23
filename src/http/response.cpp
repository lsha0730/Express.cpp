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
  Impl(std::function<void(std::string)> on_send) {
    on_send_ = on_send;
    set_defaults();
  };

  /**
   * Sends the jsonified response back to the client.
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   */
  template <typename T> void send(const T &data) {
    check_sendable();
    set_header("Content-Type", "application/json; charset=utf-8");
    set_header("Content-Length", std::to_string(data.size()));
    send_string(data);
  }

  /**
   * Sends already-stringified JSON response back to the client.
   * Sets a JSON-appropriate header.
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   */
  void json(const std::string &data) {
    check_sendable();
    set_header("Content-Type", "application/json; charset=utf-8");
    set_header("Content-Length", std::to_string(data.size()));
    send_string(data);
  }

  /**
   * Sets the status code of the response
   * @param code HTTP status code (e.g., 200, 404, 500)
   */
  void status(int code) {
    status_code_ = code;
    status_message_ = HttpStatus::getMessage(code);
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

  /* Callback registered by server to continue to socket write */
  std::function<void(std::string)> on_send_;

  /**
   * Sets default values for the object
   * @private
   */
  void set_defaults() {
    status(200);
    headers_sent_ = false;
  }

  /**
   * Sends the response back to the client.
   * @param content Pre-stringified content, to be sent as the response body.
   * @warning Does not check if a response has been sent already.
   * @private
   */
  void send_string(std::string content) {
    std::string http_response_string = build_http_response(content);
    on_send_(http_response_string);
    headers_sent_ = true;
  }

  /**
   * Sets a response header.
   * @param key Header name
   * @param value Header value
   * @private
   */
  void set_header(const std::string &key, const std::string &value) { headers_[key] = value; }

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
   * @param content Pre-stringified content, to be sent as the response body.
   * @returns Complete HTTP response string
   * @private
   * @todo Factor out into separate class
   */
  std::string build_http_response(std::string content) {
    std::string status_line = build_status_line();
    std::string headers = build_headers();
    return fmt::format("{}\r\n{}\r\n\r\n{}", status_line, headers, content);
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
   * Adds a date header with HTTP format date string.
   * @private
   */
  void build_date_header() { set_header("Date", get_http_date_string()); }

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
   * Adds a Server header with version and OS information.
   * @example Server: Flash/0.1.0 (macOS)
   * @private
   */
  void build_server_header() {
    std::string server_info =
        fmt::format("{}/{} ({})", metadata::SERVER_NAME, metadata::VERSION, get_os_name());
    set_header("Server", server_info);
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
};

// Constructor
Response::Response(std::function<void(const std::string &)> on_send)
    : pImpl(std::make_unique<Impl>(on_send)) {
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
} // namespace flash

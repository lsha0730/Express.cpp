#ifndef FLASH_PUBLIC_REQUEST_H
#define FLASH_PUBLIC_REQUEST_H

#include "types.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace flash {
class Request {
public:
  /**
   * Initializes a Request object and parses a raw HTTP request.
   *
   * @param raw_request The complete HTTP request string
   * @note The constructor assumes the request is well-formed
   */
  Request(std::string_view raw_request);

  std::string method;
  std::string original_url;
  std::string path;
  std::map<std::string, std::string> params;
  std::string http_version;
  std::map<std::string, std::string> headers;
  std::string body;

  // Rule of 5
  ~Request() = default;
  Request(const Request &) = delete;
  Request &operator=(const Request &) = delete;
  Request(Request &&) = default;
  Request &operator=(Request &&) = default;
};

} // namespace flash

#endif
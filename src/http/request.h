#ifndef FLASH_REQUEST_H
#define FLASH_REQUEST_H

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace flash {
class Request {
public:
  Request(std::string_view raw_request);

  std::string method;
  std::string original_url;
  std::string path;
  std::map<std::string, std::string> params;
  std::string http_version;
  std::map<std::string, std::string> headers;
  std::string body;

private:
  static constexpr char CR = '\r';
  static constexpr char LF = '\n';
  static constexpr char REQUEST_LINE_DELIMITER = ' ';
  static constexpr char HEADER_DELIMITER = ':';

  struct SplitRequest {
    std::string_view request_line;
    std::vector<std::string_view> headers;
    std::string_view body;
  };

  Request::SplitRequest split_request(std::string_view raw_request);
  void parse_request_line(std::string_view request_line);
  void parse_parameters();
  void parse_headers(std::vector<std::string_view> header_lines);
  bool is_crlf(std::string_view str, int i);
};
} // namespace flash

#endif
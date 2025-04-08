#ifndef FLASH_REQUEST_H
#define FLASH_REQUEST_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "url_codec.h"

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

  struct SplitUrl {
    std::string_view path;
    std::string_view params;
  };

  /**
   * Splits a full HTTP request string on the CRLF and categorizes them into
   * partially-parsed buckets.
   *
   * @param raw_request The complete HTTP request string
   * @return A SplitRequest struct containing the partially-parsed request line,
   * headers, and body.
   */
  SplitRequest split_request(std::string_view raw_request);

  /**
   * Parses the request line into the Method, Request URI and HTTP Version
   *
   * @param request_line The request line as a single, space-delimited string
   */
  void parse_request_line(std::string_view request_line);

  /**
   * Parses the full URL for parameters
   */
  void parse_url();

  /**
   * Parses in the path and returns the query string
   * Assumes the original_url is already populated with the unbroken string
   * @return The query part of the URL, excluding the '?'
   */
  std::string_view split_url();

  /**
   * Parses a vector of header lines into internal key-value pairs
   *
   * @param header_lines A vector of strings, each representing a header line
   */
  void parse_headers(std::vector<std::string_view> header_lines);

  /**
   * Checks if a CRLF sequence begins at the specified index for a string view
   *
   * @param str The string view in question
   * @param i The index which may be the CR in a CRLF sequence.
   * @return A boolean indicating whether or not CRLF begins at i
   */
  bool is_crlf(std::string_view str, int i);
};
} // namespace flash

#endif
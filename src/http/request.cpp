#include "request.h"

/**
 * Initializes a Request object and parses a raw HTTP request.
 *
 * @param raw_request The complete HTTP request string
 * @note The constructor assumes the request is well-formed
 */
flash::Request::Request(std::string_view raw_request) {
  flash::Request::SplitRequest split = split_request(raw_request);
  parse_request_line(split.request_line);
  parse_headers(split.headers);
  body = split.body;
}

/**
 * Splits a full HTTP request string on the CRLF and categorizes them into
 * partially-parsed buckets.
 *
 * @param raw_request The complete HTTP request string
 * @return A SplitRequest struct containing the partially-parsed request line,
 * headers, and body.
 */
flash::Request::SplitRequest
flash::Request::split_request(std::string_view raw_request) {
  std::string_view request_line;
  std::vector<std::string_view> headers;
  int left = 0;
  bool is_first_line = true;
  for (int i = 0; i < raw_request.length(); i++) {
    char c = raw_request.at(i);
    if (is_crlf(raw_request, i)) {
      if (is_crlf(raw_request, left))
        break; // left is CRLF
      std::string_view line = raw_request.substr(left, i - left);
      left = i + 2;
      if (is_first_line) {
        request_line = line;
        is_first_line = false;
        continue;
      }
      headers.push_back(line);
    }
  }
  int body_start_index = left + 2;
  std::string_view body = raw_request.substr(
      body_start_index, raw_request.length() - body_start_index);

  return flash::Request::SplitRequest{request_line, headers, body};
}

/**
 * Parses the request line into the Method, Request URI and HTTP Version
 *
 * @param request_line The request line as a single, space-delimited string
 */
void flash::Request::parse_request_line(std::string_view request_line) {
  int left = 0;
  std::vector<std::string_view> parts;
  for (int i = 0; i < request_line.length(); i++) {
    char c = request_line.at(i);
    if (c == Request::REQUEST_LINE_DELIMITER) {
      std::string_view segment = request_line.substr(left, i - left);
      parts.push_back(segment);
      left = i + 1;
    }
  }
  parts.push_back(request_line.substr(left, request_line.length() - left));
  if (parts.size() != 3)
    throw std::invalid_argument(
        "Request line does not have exactly three components");
  method = parts[0];
  original_url = parts[1];
  http_version = parts[2];

  parse_parameters();
}

/**
 * Parses the full URI for parameters
 */
void flash::Request::parse_parameters() {
  std::string_view url_view = std::string_view(original_url);
  // TODO: Implement, and make sure to use url decoding too
}

/**
 * Parses a vector of header lines into internal key-value pairs
 *
 * @param header_lines A vector of strings, each representing a header line
 */
void flash::Request::parse_headers(std::vector<std::string_view> header_lines) {
  for (std::string_view header : header_lines) {
    for (int i = 0; i < header.length(); i++) {
      char c = header.at(i);
      if (c == Request::HEADER_DELIMITER) {
        std::string key = std::string(header.substr(0, i));
        int value_start_index = i + 2;
        std::string value = std::string(header.substr(
            value_start_index, header.length() - value_start_index));
        headers[key] = value;
        break;
      }
    }
  }
}

/**
 * Checks if a CRLF sequence begins at the specified index for a string view
 *
 * @param str The string view in question
 * @param i The index which may be the CR in a CRLF sequence.
 * @return A boolean indicating whether or not CRLF begins at i
 */
bool flash::Request::is_crlf(std::string_view str, int i) {
  if (i < 0 || i + 1 >= str.length())
    return false;
  char a = str.at(i);
  char b = str.at(i + 1);
  return a == flash::Request::CR && b == flash::Request::LF;
}

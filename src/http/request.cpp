// #include "request.h"
#include "include/flash/request.h"
#include "http/url_codec.h"

namespace flash {
class RequestParser {

public:
  static void parse(Request &request, std::string_view raw_request) {
    SplitRequest split = split_request(raw_request);
    parse_request_line(request, split.request_line);
    parse_headers(request, split.headers);
    request.body = split.body;
  }

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

  static SplitRequest split_request(std::string_view raw_request) {
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

    return SplitRequest{request_line, headers, body};
  }

  static void parse_request_line(Request &request,
                                 std::string_view request_line) {
    int left = 0;
    std::vector<std::string_view> parts;
    for (int i = 0; i < request_line.length(); i++) {
      char c = request_line.at(i);
      if (c == REQUEST_LINE_DELIMITER) {
        std::string_view segment = request_line.substr(left, i - left);
        parts.push_back(segment);
        left = i + 1;
      }
    }
    parts.push_back(request_line.substr(left, request_line.length() - left));
    if (parts.size() != 3)
      throw std::invalid_argument(
          "Request line does not have exactly three components");
    request.method = parts[0];
    request.original_url = parts[1];
    request.http_version = parts[2];

    parse_url(request);
  }

  static void parse_url(Request &request) {
    std::string_view query = split_url(request);
    size_t param_start = 0;
    size_t param_end;

    while (param_start < query.length()) {
      param_end = query.find('&', param_start);
      if (param_end == std::string::npos) {
        param_end = query.length();
      }

      std::string_view param =
          query.substr(param_start, param_end - param_start);

      size_t equals_pos = param.find('=');
      if (equals_pos != std::string::npos) {
        std::string_view key = param.substr(0, equals_pos);
        std::string_view value = param.substr(equals_pos + 1);

        if (!key.empty()) {
          request.params[UrlCodec::decode(key)] = UrlCodec::decode(value);
        }
      }

      param_start = param_end + 1;
    }
  }

  static std::string_view split_url(Request &request) {
    size_t question_mark_index = request.original_url.find("?");
    if (question_mark_index == std::string::npos) {
      request.path = request.original_url;
      return "";
    }
    request.path = request.original_url.substr(0, question_mark_index);

    std::string_view query =
        std::string_view(request.original_url).substr(question_mark_index + 1);
    return query;
  }

  static void parse_headers(Request &request,
                            std::vector<std::string_view> header_lines) {
    for (std::string_view header : header_lines) {
      for (int i = 0; i < header.length(); i++) {
        char c = header.at(i);
        if (c == HEADER_DELIMITER) {
          std::string key = std::string(header.substr(0, i));
          int value_start_index = i + 2;
          std::string value = std::string(header.substr(
              value_start_index, header.length() - value_start_index));
          request.headers[key] = value;
          break;
        }
      }
    }
  }

  static bool is_crlf(std::string_view str, int i) {
    if (i < 0 || i + 1 >= str.length())
      return false;
    char a = str.at(i);
    char b = str.at(i + 1);
    return a == CR && b == LF;
  }
};

// Constructor
Request::Request(std::string_view raw_request) {
  RequestParser::parse(*this, raw_request);
}

} // namespace flash

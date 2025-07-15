#include <flash/request.h>
#include <gtest/gtest.h>

namespace flash {
namespace test {

// Test a basic GET request
TEST(RequestTest, BasicGetRequest) {
  std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "User-Agent: Mozilla/5.0\r\n"
                            "\r\n"
                            "This is the request body";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.original_url, "/index.html");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 2);
  EXPECT_EQ(request.headers["Host"], "example.com");
  EXPECT_EQ(request.headers["User-Agent"], "Mozilla/5.0");
  EXPECT_EQ(request.body, "This is the request body");
}

// Test a POST request with a body
TEST(RequestTest, PostRequestWithBody) {
  std::string raw_request = "POST /api/data HTTP/1.1\r\n"
                            "Host: api.example.com\r\n"
                            "Content-Type: application/json\r\n"
                            "Content-Length: 28\r\n"
                            "\r\n"
                            "{\"key\": \"value\", \"number\": 42}";

  Request request(raw_request);

  EXPECT_EQ(request.method, "POST");
  EXPECT_EQ(request.original_url, "/api/data");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 3);
  EXPECT_EQ(request.headers["Host"], "api.example.com");
  EXPECT_EQ(request.headers["Content-Type"], "application/json");
  EXPECT_EQ(request.headers["Content-Length"], "28");
  EXPECT_EQ(request.body, "{\"key\": \"value\", \"number\": 42}");
}

// Test a request with no headers
TEST(RequestTest, RequestWithNoHeaders) {
  std::string raw_request = "GET / HTTP/1.1\r\n"
                            "\r\n"
                            "No headers, just a body";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.original_url, "/");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 0);
  EXPECT_EQ(request.body, "No headers, just a body");
}

// Test a request with no body
TEST(RequestTest, RequestWithNoBody) {
  std::string raw_request = "HEAD / HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "HEAD");
  EXPECT_EQ(request.original_url, "/");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 1);
  EXPECT_EQ(request.headers["Host"], "example.com");
  EXPECT_TRUE(request.body.empty());
}

// Test a request with multiple header values
TEST(RequestTest, RequestWithMultipleHeaderValues) {
  std::string raw_request = "GET / HTTP/1.1\r\n"
                            "Accept: text/html,application/xhtml+xml\r\n"
                            "Accept-Language: en-US,en;q=0.9\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.original_url, "/");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 2);
  EXPECT_EQ(request.headers["Accept"], "text/html,application/xhtml+xml");
  EXPECT_EQ(request.headers["Accept-Language"], "en-US,en;q=0.9");
  EXPECT_TRUE(request.body.empty());
}

// Test a request with a complex body containing CRLF
TEST(RequestTest, RequestWithComplexBody) {
  std::string raw_request = "POST /upload HTTP/1.1\r\n"
                            "Content-Type: multipart/form-data\r\n"
                            "\r\n"
                            "This is a complex body\r\n"
                            "with multiple lines\r\n"
                            "and some special characters: \r\n\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "POST");
  EXPECT_EQ(request.original_url, "/upload");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 1);
  EXPECT_EQ(request.headers["Content-Type"], "multipart/form-data");
  EXPECT_EQ(request.body, "This is a complex body\r\nwith multiple "
                          "lines\r\nand some special characters: \r\n\r\n");
}

// Test a request with a PUT method
TEST(RequestTest, PutRequest) {
  std::string raw_request = "PUT /resource/123 HTTP/1.1\r\n"
                            "Host: api.example.com\r\n"
                            "Content-Type: application/json\r\n"
                            "\r\n"
                            "{\"updated\": true}";

  Request request(raw_request);

  EXPECT_EQ(request.method, "PUT");
  EXPECT_EQ(request.original_url, "/resource/123");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 2);
  EXPECT_EQ(request.headers["Host"], "api.example.com");
  EXPECT_EQ(request.headers["Content-Type"], "application/json");
  EXPECT_EQ(request.body, "{\"updated\": true}");
}

// Test a request with a DELETE method
TEST(RequestTest, DeleteRequest) {
  std::string raw_request = "DELETE /resource/123 HTTP/1.1\r\n"
                            "Host: api.example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "DELETE");
  EXPECT_EQ(request.original_url, "/resource/123");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 1);
  EXPECT_EQ(request.headers["Host"], "api.example.com");
  EXPECT_TRUE(request.body.empty());
}

// Test a request with a query string in the URI
TEST(RequestTest, RequestWithQueryString) {
  std::string raw_request = "GET /search?q=test&page=1 HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.original_url, "/search?q=test&page=1");
  EXPECT_EQ(request.path, "/search");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 1);
  EXPECT_EQ(request.headers["Host"], "example.com");
  EXPECT_TRUE(request.body.empty());

  // Test that params were parsed correctly
  EXPECT_EQ(request.params.size(), 2);
  EXPECT_EQ(request.params["q"], "test");
  EXPECT_EQ(request.params["page"], "1");
}

// Test a request with a fragment in the URI
TEST(RequestTest, RequestWithFragment) {
  std::string raw_request = "GET /page.html#section1 HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.original_url, "/page.html#section1");
  EXPECT_EQ(request.http_version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 1);
  EXPECT_EQ(request.headers["Host"], "example.com");
  EXPECT_TRUE(request.body.empty());
}

// Test parsing of URL-encoded parameters
TEST(RequestTest, RequestWithEncodedParameters) {
  std::string raw_request = "GET /search?q=hello+world&category=books%20%26%20media HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.original_url, "/search?q=hello+world&category=books%20%26%20media");
  EXPECT_EQ(request.path, "/search");
  EXPECT_EQ(request.params.size(), 2);
  EXPECT_EQ(request.params["q"], "hello world");
  EXPECT_EQ(request.params["category"], "books & media");
}

// Test parsing parameters with no values
TEST(RequestTest, RequestWithEmptyParameterValues) {
  std::string raw_request = "GET /api?token=abc123&filter=&sort= HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.path, "/api");
  EXPECT_EQ(request.params.size(), 3);
  EXPECT_EQ(request.params["token"], "abc123");
  EXPECT_EQ(request.params["filter"], "");
  EXPECT_EQ(request.params["sort"], "");
}

// Test parsing parameters with special characters
TEST(RequestTest, RequestWithSpecialCharactersInParameters) {
  std::string raw_request = "GET /search?q=%E6%97%A5%E6%9C%AC%E8%AA%9E&year=2023-2024 HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.path, "/search");
  EXPECT_EQ(request.params.size(), 2);
  EXPECT_EQ(request.params["q"], "日本語");
  EXPECT_EQ(request.params["year"], "2023-2024");
}

// Test parsing a complex query string with multiple parameters
TEST(RequestTest, RequestWithComplexQueryString) {
  std::string raw_request =
      "GET "
      "/products?category=electronics&price=100-500&brand=apple&brand=samsung&"
      "in_stock=true&sort=price&direction=asc HTTP/1.1\r\n"
      "Host: example.com\r\n"
      "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.path, "/products");
  EXPECT_EQ(request.params.size(), 6);
  EXPECT_EQ(request.params["category"], "electronics");
  EXPECT_EQ(request.params["price"], "100-500");
  EXPECT_EQ(request.params["in_stock"], "true");
  EXPECT_EQ(request.params["sort"], "price");
  EXPECT_EQ(request.params["direction"], "asc");
  // Note: For multiple values with the same key, only the last one is stored
  EXPECT_EQ(request.params["brand"], "samsung");
}

// Test parsing POST request with both URL parameters and body
TEST(RequestTest, PostRequestWithUrlParameters) {
  std::string raw_request = "POST /submit?source=web&ref=homepage HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "Content-Type: application/x-www-form-urlencoded\r\n"
                            "\r\n"
                            "name=John&email=john%40example.com";

  Request request(raw_request);

  EXPECT_EQ(request.method, "POST");
  EXPECT_EQ(request.path, "/submit");
  EXPECT_EQ(request.params.size(), 2);
  EXPECT_EQ(request.params["source"], "web");
  EXPECT_EQ(request.params["ref"], "homepage");
  EXPECT_EQ(request.body, "name=John&email=john%40example.com");
}

// Test a request with no query parameters
TEST(RequestTest, RequestWithNoParameters) {
  std::string raw_request = "GET /about HTTP/1.1\r\n"
                            "Host: example.com\r\n"
                            "\r\n";

  Request request(raw_request);

  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.original_url, "/about");
  EXPECT_EQ(request.path, "/about");
  EXPECT_TRUE(request.params.empty());
}

} // namespace test
} // namespace flash
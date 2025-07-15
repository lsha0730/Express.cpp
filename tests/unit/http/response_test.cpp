#include <express/response.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <span>
#include <vector>

namespace express {
namespace test {

class TestableResponse : public Response {
public:
  TestableResponse(std::function<void(const std::vector<char> &)> write_to_socket,
                   std::function<void()> close_socket)
      : Response(write_to_socket, close_socket) {}
};

class ResponseFixture : public ::testing::Test {
protected:
  void SetUp() override {
    // Mock socket functions
    write_to_socket = [this](const std::vector<char> &data) {
      last_written = data;
    };
    close_socket = []() {};

    response = std::make_unique<TestableResponse>(write_to_socket, close_socket);
  }

  std::function<void(const std::vector<char>)> write_to_socket;
  std::function<void()> close_socket;
  std::unique_ptr<Response> response;
  std::vector<char> last_written;
};

// Header tests
TEST_F(ResponseFixture, SetHeader) {
  response->set("Content-Type", "application/json");
  EXPECT_EQ(response->get("Content-Type"), "application/json");
}

TEST_F(ResponseFixture, SetHeaderOverwrite) {
  response->set("X-Custom", "first");
  response->set("X-Custom", "second");
  EXPECT_EQ(response->get("X-Custom"), "second");
}

TEST_F(ResponseFixture, SetHeaderAfterSend) {
  response->send("test");
  EXPECT_THROW(response->set("X-Custom", "value"), std::runtime_error);
}

TEST_F(ResponseFixture, GetNonExistentHeader) {
  EXPECT_THROW(response->get("Non-Existent"), std::runtime_error);
}

TEST_F(ResponseFixture, SetMultipleHeaders) {
  response->set("Content-Type", "application/json");
  response->set("X-Custom", "value");
  response->set("Cache-Control", "no-cache");

  EXPECT_EQ(response->get("Content-Type"), "application/json");
  EXPECT_EQ(response->get("X-Custom"), "value");
  EXPECT_EQ(response->get("Cache-Control"), "no-cache");
}

TEST_F(ResponseFixture, SetEmptyHeaderValue) {
  response->set("X-Empty", "");
  EXPECT_EQ(response->get("X-Empty"), "");
}

// BufferLike tests
TEST_F(ResponseFixture, SendVectorChar) {
  std::vector<char> data = {'a', 'b', 'c'};
  response->send(data);
  EXPECT_EQ(last_written, data);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/octet-stream");
}

TEST_F(ResponseFixture, SendVectorUint8) {
  std::vector<uint8_t> data = {1, 2, 3};
  response->send(data);
  std::vector<char> expected = {1, 2, 3};
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/octet-stream");
}

TEST_F(ResponseFixture, SendSpanChar) {
  std::vector<char> original = {'x', 'y', 'z'};
  std::span<char> data(original);
  response->send(data);
  EXPECT_EQ(last_written, original);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/octet-stream");
}

// StringLike tests
TEST_F(ResponseFixture, SendString) {
  std::string data = "hello world";
  response->send(data);
  std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "text/html; charset=utf-8");
}

TEST_F(ResponseFixture, SendStringView) {
  std::string original = "test string";
  std::string_view data(original);
  response->send(data);
  std::vector<char> expected(original.begin(), original.end());
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "text/html; charset=utf-8");
}

TEST_F(ResponseFixture, SendChar) {
  char data = 'A';
  response->send(data);
  std::vector<char> expected = {'A'};
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "text/html; charset=utf-8");
}

// BoolLike tests
TEST_F(ResponseFixture, SendTrue) {
  response->send(true);
  std::string serialized = "true";
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "text/html; charset=utf-8");
}

TEST_F(ResponseFixture, SendFalse) {
  response->send(false);
  std::string serialized = "false";
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "text/html; charset=utf-8");
}

// NumberLike tests
TEST_F(ResponseFixture, SendInt) {
  response->send(42);
  std::string serialized = "42";
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "text/html; charset=utf-8");
}

TEST_F(ResponseFixture, SendDouble) {
  response->send(3.14);
  std::string serialized = "3.14";
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(last_written, expected);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "text/html; charset=utf-8");
}

TEST_F(ResponseFixture, SendStatusCode) {
  response->send(404);
  EXPECT_TRUE(last_written.empty());
  EXPECT_EQ(response->status_code(), 404);
  EXPECT_THROW(response->get("Content-Type"), std::runtime_error);
}

// NullLike test
TEST_F(ResponseFixture, SendNull) {
  response->send(nullptr);
  EXPECT_TRUE(last_written.empty());
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_THROW(response->get("Content-Type"), std::runtime_error);
}

// Json tests
TEST_F(ResponseFixture, SendJsonObject) {
  nlohmann::json data = {{"key", "value"}, {"number", 42}};
  response->json(data);
  std::string expected = data.dump(4);
  std::vector<char> expected_vec(expected.begin(), expected.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendJsonEmptyObject) {
  nlohmann::json data = {};
  response->json(data);
  std::string expected = data.dump(4);
  std::vector<char> expected_vec(expected.begin(), expected.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendJsonArray) {
  nlohmann::json data = {1, 2, 3, 4, 5};
  response->json(data);
  std::string expected = data.dump(4);
  std::vector<char> expected_vec(expected.begin(), expected.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendJsonNested) {
  nlohmann::json data = {{"array", {1, 2, 3}}, {"object", {{"nested", "value"}}},
                         {"string", "test"},   {"number", 42},
                         {"boolean", true},    {"null", nullptr}};
  response->json(data);
  std::string expected = data.dump(4);
  std::vector<char> expected_vec(expected.begin(), expected.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

// Generic type to JSON tests
TEST_F(ResponseFixture, SendMapAsJson) {
  std::map<std::string, int> data = {{"one", 1}, {"two", 2}};
  response->json(data);
  nlohmann::json expected = data;
  std::string expected_str = expected.dump(4);
  std::vector<char> expected_vec(expected_str.begin(), expected_str.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendVectorAsJson) {
  std::vector<int> data = {1, 2, 3, 4, 5};
  response->json(data);
  nlohmann::json expected = data;
  std::string expected_str = expected.dump(4);
  std::vector<char> expected_vec(expected_str.begin(), expected_str.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendStringAsJson) {
  std::string data = "test string";
  response->json(data);
  nlohmann::json expected = data;
  std::string expected_str = expected.dump(4);
  std::vector<char> expected_vec(expected_str.begin(), expected_str.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendNumberAsJson) {
  int data = 42;
  response->json(data);
  nlohmann::json expected = data;
  std::string expected_str = expected.dump(4);
  std::vector<char> expected_vec(expected_str.begin(), expected_str.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendBoolAsJson) {
  bool data = true;
  response->json(data);
  nlohmann::json expected = data;
  std::string expected_str = expected.dump(4);
  std::vector<char> expected_vec(expected_str.begin(), expected_str.end());
  EXPECT_EQ(last_written, expected_vec);
  EXPECT_EQ(response->status_code(), 200);
  EXPECT_EQ(response->get("Content-Type"), "application/json; charset=utf-8");
}

TEST_F(ResponseFixture, SendJsonAfterSend) {
  response->send("test");
  nlohmann::json data = {{"key", "value"}};
  EXPECT_THROW(response->json(data), std::runtime_error);
}

// Error cases
TEST_F(ResponseFixture, SendAfterEnd) {
  response->end();
  EXPECT_THROW(response->send("test"), std::runtime_error);
}

} // namespace test
} // namespace express
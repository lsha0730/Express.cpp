#include "http/http_verb.h"
#include <gtest/gtest.h>

namespace express {
namespace test {

TEST(HttpVerbStruct, TestVerbToString) {
  EXPECT_EQ(express::HttpVerb::toString(express::HttpVerb::Value::GET), "GET");
  EXPECT_EQ(express::HttpVerb::toString(express::HttpVerb::Value::POST), "POST");
  EXPECT_EQ(express::HttpVerb::toString(express::HttpVerb::Value::PUT), "PUT");
  EXPECT_EQ(express::HttpVerb::toString(express::HttpVerb::Value::DELETE), "DELETE");
}

TEST(HttpVerbStruct, TestStringToEnum) {
  EXPECT_EQ(express::HttpVerb::toEnum("GET"), express::HttpVerb::Value::GET);
  EXPECT_EQ(express::HttpVerb::toEnum("POST"), express::HttpVerb::Value::POST);
  EXPECT_EQ(express::HttpVerb::toEnum("PUT"), express::HttpVerb::Value::PUT);
  EXPECT_EQ(express::HttpVerb::toEnum("DELETE"), express::HttpVerb::Value::DELETE);
}

} // namespace test
} // namespace express
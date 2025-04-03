#include "http/http_verb.h"
#include <gtest/gtest.h>

TEST(HttpVerbStruct, TestVerbToString) {
  EXPECT_EQ(flash::HttpVerb::toString(flash::HttpVerb::Value::GET), "GET");
  EXPECT_EQ(flash::HttpVerb::toString(flash::HttpVerb::Value::POST), "POST");
  EXPECT_EQ(flash::HttpVerb::toString(flash::HttpVerb::Value::PUT), "PUT");
  EXPECT_EQ(flash::HttpVerb::toString(flash::HttpVerb::Value::DELETE),
            "DELETE");
}

TEST(HttpVerbStruct, TestStringToEnum) {
  EXPECT_EQ(flash::HttpVerb::toEnum("GET"), flash::HttpVerb::Value::GET);
  EXPECT_EQ(flash::HttpVerb::toEnum("POST"), flash::HttpVerb::Value::POST);
  EXPECT_EQ(flash::HttpVerb::toEnum("PUT"), flash::HttpVerb::Value::PUT);
  EXPECT_EQ(flash::HttpVerb::toEnum("DELETE"), flash::HttpVerb::Value::DELETE);
}
#include "http/http_status.h"
#include <gtest/gtest.h>

namespace flash {
namespace test {

TEST(HttpStatusClass, RudimentaryTest) {
  EXPECT_EQ(HttpStatus::getMessage(200), "OK");
  EXPECT_EQ(HttpStatus::getMessage(404), "Not Found");
  EXPECT_EQ(HttpStatus::getMessage(500), "Internal Server Error");
  EXPECT_EQ(HttpStatus::getMessage(418), "I'm a teapot");
  EXPECT_THROW(HttpStatus::getMessage(999), std::invalid_argument);
}

} // namespace test
} // namespace flash

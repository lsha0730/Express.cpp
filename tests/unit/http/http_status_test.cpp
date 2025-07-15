#include "http/http_status.h"
#include <gtest/gtest.h>

namespace express {
namespace test {

TEST(HttpStatusClass, RudimentaryTest) {
  EXPECT_EQ(HttpStatus::get_message(200), "OK");
  EXPECT_EQ(HttpStatus::get_message(404), "Not Found");
  EXPECT_EQ(HttpStatus::get_message(500), "Internal Server Error");
  EXPECT_EQ(HttpStatus::get_message(418), "I'm a teapot");
  EXPECT_THROW(HttpStatus::get_message(999), std::invalid_argument);
}

} // namespace test
} // namespace express

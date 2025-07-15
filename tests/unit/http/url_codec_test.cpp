#include "http/url_codec.h"
#include <gtest/gtest.h>

namespace express {
namespace test {

// Test basic URL decoding
TEST(UrlCodecTest, BasicDecode) {
  UrlCodec codec;
  std::string encoded = "Hello+World";
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "Hello World");
}

// Test decoding with percent-encoded ASCII characters
TEST(UrlCodecTest, PercentEncodedAscii) {
  UrlCodec codec;
  std::string encoded = "Hello%20World%21";
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "Hello World!");
}

// Test decoding with a mix of space encodings
TEST(UrlCodecTest, MixedSpaceEncoding) {
  UrlCodec codec;
  std::string encoded = "Hello+World%20and%20Universe";
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "Hello World and Universe");
}

// Test decoding of UTF-8 characters
TEST(UrlCodecTest, Utf8Characters) {
  UrlCodec codec;
  std::string encoded = "Hello%20%E6%97%A5%E6%9C%AC"; // "Hello 日本"
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "Hello 日本");
}

// Test decoding with multiple UTF-8 characters
TEST(UrlCodecTest, MultipleUtf8Characters) {
  UrlCodec codec;
  std::string encoded = "%E4%BD%A0%E5%A5%BD%E4%B8%96%E7%95%8C"; // "你好世界"
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "你好世界");
}

// Test with URL having query parameters containing UTF-8
TEST(UrlCodecTest, QueryParamsWithUtf8) {
  UrlCodec codec;
  std::string encoded = "/search?q=%E6%B1%82%E8%81%8C&lang=zh"; // "/search?q=求职&lang=zh"
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "/search?q=求职&lang=zh");
}

// Test with special characters
TEST(UrlCodecTest, SpecialCharacters) {
  UrlCodec codec;
  std::string encoded = "%40%23%24%25%5E%26%2A%28%29"; // "@#$%^&*()"
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "@#$%^&*()");
}

// Test with a mix of encoded and unencoded characters
TEST(UrlCodecTest, MixedEncodedAndUnencoded) {
  UrlCodec codec;
  std::string encoded = "abc%20123/xyz?param=value&other=%7Bkey%7D";
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "abc 123/xyz?param=value&other={key}");
}

// Test with 4-byte UTF-8 characters (emoji)
TEST(UrlCodecTest, FourByteUtf8Characters) {
  UrlCodec codec;
  std::string encoded = "%F0%9F%98%80%F0%9F%98%8E"; // "😀😎"
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "😀😎");
}

// Test with an empty string
TEST(UrlCodecTest, EmptyString) {
  UrlCodec codec;
  std::string encoded = "";
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "");
}

// Test with no percent encodings
TEST(UrlCodecTest, NoPercentEncodings) {
  UrlCodec codec;
  std::string encoded = "HelloWorld";
  std::string decoded = codec.decode(encoded);

  EXPECT_EQ(decoded, "HelloWorld");
}

// Test with malformed percent encoding (missing digits)
TEST(UrlCodecTest, MalformedPercentEncoding) {
  UrlCodec codec;
  std::string encoded = "Hello%2World";

  EXPECT_THROW(codec.decode(encoded), std::out_of_range);
}

// Test with malformed UTF-8 sequence
TEST(UrlCodecTest, MalformedUtf8Sequence) {
  UrlCodec codec;
  std::string encoded = "%E6%97ABC"; // Incomplete UTF-8 sequence

  EXPECT_THROW(codec.decode(encoded), std::invalid_argument);
}

} // namespace test
} // namespace express
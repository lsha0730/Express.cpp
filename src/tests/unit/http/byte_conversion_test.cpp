#include "http/byte_conversion.h"
#include <gtest/gtest.h>
#include <span>
#include <vector>

namespace flash {
namespace test {

TEST(ByteConversion, BufferLikeVectorChar) {
  std::vector<char> input = {'a', 'b', 'c', 'd'};
  auto result = flash::to_bytes(input);
  EXPECT_EQ(result, input);
}

TEST(ByteConversion, BufferLikeVectorUint8) {
  std::vector<uint8_t> input = {1, 2, 3, 4};
  auto result = flash::to_bytes(input);
  std::vector<char> expected = {1, 2, 3, 4};
  EXPECT_EQ(result, expected);
}

TEST(ByteConversion, BufferLikeVectorByte) {
  std::vector<std::byte> input = {std::byte{1}, std::byte{2}, std::byte{3}};
  auto result = flash::to_bytes(input);
  std::vector<char> expected = {1, 2, 3};
  EXPECT_EQ(result, expected);
}

TEST(ByteConversion, BufferLikeSpanChar) {
  std::vector<char> original = {'x', 'y', 'z'};
  std::span<char> input(original);
  auto result = flash::to_bytes(input);
  EXPECT_EQ(result, original);
}

TEST(ByteConversion, BufferLikeSpanUint8) {
  std::vector<uint8_t> original = {5, 6, 7};
  std::span<uint8_t> input(original);
  auto result = flash::to_bytes(input);
  std::vector<char> expected = {5, 6, 7};
  EXPECT_EQ(result, expected);
}

TEST(ByteConversion, StringLikeString) {
  std::string input = "hello world";
  auto result = flash::to_bytes(input);
  std::vector<char> expected(input.begin(), input.end());
  EXPECT_EQ(result, expected);
}

TEST(ByteConversion, StringLikeStringView) {
  std::string original = "test string";
  std::string_view input(original);
  auto result = flash::to_bytes(input);
  std::vector<char> expected(original.begin(), original.end());
  EXPECT_EQ(result, expected);
}

TEST(ByteConversion, StringLikeChar) {
  char input = 'A';
  auto result = flash::to_bytes(input);
  std::vector<char> expected = {'A'};
  EXPECT_EQ(result, expected);
}

TEST(ByteConversion, StringLikePath) {
  std::filesystem::path input = "/path/to/file.txt";
  auto result = flash::to_bytes(input);
  std::vector<char> expected(input.string().begin(), input.string().end());
  EXPECT_EQ(result, expected);
}

TEST(ByteConversion, EmptyInput) {
  // Test empty vector
  std::vector<char> empty_vec;
  auto vec_result = flash::to_bytes(empty_vec);
  EXPECT_TRUE(vec_result.empty());

  // Test empty string
  std::string empty_str;
  auto str_result = flash::to_bytes(empty_str);
  EXPECT_TRUE(str_result.empty());
}

} // namespace test
} // namespace flash
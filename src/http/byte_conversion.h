#ifndef FLASH_BYTE_CONVERSION
#define FLASH_BYTE_CONVERSION

#include "include/flash/concepts.h"
#include <filesystem>
#include <iterator>

namespace flash {
template <BufferLike T> std::vector<char> to_bytes(const T &input) {
  const char *data_ptr = reinterpret_cast<const char *>(std::data(input));
  return std::vector<char>(data_ptr, data_ptr + std::size(input));
}

template <typename T, size_t N> std::vector<char> to_bytes(const T (&input)[N]) {
  return std::vector<char>(input, input + N);
}

template <StringLike T> std::vector<char> to_bytes(const T &input) {
  std::vector<char> data;
  if constexpr (std::is_same_v<std::decay_t<T>, char>) {
    data.push_back(input);
  } else if constexpr (std::is_same_v<std::decay_t<T>, std::filesystem::path>) {
    std::string stringified(input);
    data.reserve(stringified.size());
    data.insert(data.end(), stringified.begin(), stringified.end());
  } else {
    data.reserve(input.size());
    data.insert(data.end(), input.begin(), input.end());
  }
  return data;
}
} // namespace flash

#endif
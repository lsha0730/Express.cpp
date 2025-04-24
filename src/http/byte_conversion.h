#ifndef FLASH_BYTE_CONVERSION
#define FLASH_BYTE_CONVERSION

#include "include/flash/concepts.h"

namespace flash {
template <BufferLike T> std::vector<char> to_bytes(const T &input) {
  const char *data_ptr = reinterpret_cast<const char *>(std::data(input));
  return std::vector<char>(data_ptr, data_ptr + std::size(input));
}

template <StringLike T> std::vector<char> to_bytes(const T &input) {
  std::vector<char> data;
  data.reserve(input.size());
  data.insert(data.end(), input.begin(), input.end());
  return data;
}
} // namespace flash

#endif
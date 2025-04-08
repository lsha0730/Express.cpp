#ifndef FLASH_URL_CODEC_H
#define FLASH_URL_CODEC_H

#include <string>
#include <string_view>
#include <vector>

namespace flash {
class UrlCodec {
public:
  std::string decode(std::string_view);
  //   std::string encode(std::string_view); TODO

private:
  struct utf8_decoded {
    std::string utf8;
    int new_index;
  };

  utf8_decoded decode_percent_encoding(std::string_view str, int i);
  int get_utf8_sequence_length(unsigned char first_byte);
  std::string hex_to_utf8(const std::string &hex_str);
  bool is_valid_hex_digit(char);
};
} // namespace flash

#endif
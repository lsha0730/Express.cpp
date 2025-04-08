#ifndef FLASH_URL_CODEC_H
#define FLASH_URL_CODEC_H

#include <string>
#include <string_view>
#include <vector>

namespace flash {
class UrlCodec {
public:
  static std::string decode(std::string_view);
  //   static std::string encode(std::string_view); TODO

private:
  struct utf8_decoded {
    std::string utf8;
    int new_index;
  };

  static utf8_decoded decode_percent_encoding(std::string_view str, int i);
  static int get_utf8_sequence_length(unsigned char first_byte);
  static std::string hex_to_utf8(const std::string &hex_str);
  static bool is_valid_hex_digit(char);
};
} // namespace flash

#endif
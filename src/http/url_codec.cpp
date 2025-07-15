#include "url_codec.h"

std::string express::UrlCodec::decode(std::string_view input) {
  std::string output;
  output.reserve(input.length());
  for (int i = 0; i < input.length(); i++) {
    char c = input[i];
    if (c == '+') {
      output += ' ';
      continue;
    }
    if (c == '%') {
      bool at_least_2_more_chars = i + 2 < input.length();
      if (!at_least_2_more_chars) {
        throw std::out_of_range("Incomplete percent encoding");
      }

      for (int j = 1; j <= 2; j++) {
        char hex_char = input[i + j];
        if (!is_valid_hex_digit(hex_char)) {
          throw std::out_of_range("Invalid hex digit in percent encoding");
        }
      }

      utf8_decoded decoded = decode_percent_encoding(input, i);
      output += decoded.utf8;
      i = decoded.new_index;
      continue;
    }
    output += c;
  }
  return output;
}

bool express::UrlCodec::is_valid_hex_digit(char c) {
  return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

express::UrlCodec::utf8_decoded express::UrlCodec::decode_percent_encoding(std::string_view str,
                                                                           int i) {
  if (i + 2 >= str.length()) {
    throw std::out_of_range("Not enough bytes to read into UTF8!");
  }

  std::string first_byte = std::string(str.substr(i + 1, 2));
  u_int first_byte_base10 = std::stoul(first_byte, nullptr, 16);
  int sequence_length = get_utf8_sequence_length(first_byte_base10);

  int last_sequence_index = i + (sequence_length * 3) - 1;
  if (last_sequence_index > str.length()) {
    throw std::out_of_range("Not enough bytes to read full UTF-8 sequence!");
  }

  std::string full_hex;
  full_hex += first_byte;

  i += 3;

  for (int j = 1; j < sequence_length; j++) {
    bool enough_chars_remaining = i + 2 <= str.length();
    if (enough_chars_remaining && str[i] == '%') {
      std::string next_hex_pair = std::string(str.substr(i + 1, 2));
      full_hex += next_hex_pair;
      i += 3;
    } else {
      throw std::invalid_argument("Invalid percent-encoded UTF-8 sequence");
    }
  }

  std::string utf8 = hex_to_utf8(full_hex);
  return {utf8, i - 1}; // -1 because the main loop will increment i
}

std::string express::UrlCodec::hex_to_utf8(const std::string &hex_str) {
  std::string result;
  for (size_t i = 0; i < hex_str.length(); i += 2) {
    std::string byte = hex_str.substr(i, 2);
    char chr = static_cast<char>(std::stoi(byte, nullptr, 16));
    result.push_back(chr);
  }
  return result;
}

int express::UrlCodec::get_utf8_sequence_length(unsigned char first_byte) {
  if ((first_byte & 0x80) == 0)
    return 1; // 0xxxxxxx (ASCII)
  if ((first_byte & 0xE0) == 0xC0)
    return 2; // 110xxxxx
  if ((first_byte & 0xF0) == 0xE0)
    return 3; // 1110xxxx
  if ((first_byte & 0xF8) == 0xF0)
    return 4; // 11110xxx

  return 0; // Invalid leading byte
}
#ifndef FLASH_HTTP_VERB_H
#define FLASH_HTTP_VERB_H

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace flash {
struct HttpVerb {
public:
  enum class Value { GET, POST, PUT, DELETE };

  static inline const std::string toString(Value value) {
    return enum_to_string.at(value);
  }

  static inline Value toEnum(const std::string &str) {
    try {
      return string_to_enum.at(str);
    } catch (const std::out_of_range &e) {
      throw std::runtime_error("String is not a valid HttpVerb");
    }
  }

private:
  static const std::unordered_map<Value, std::string> enum_to_string;
  static const std::unordered_map<std::string, Value> string_to_enum;
};

// Define the static members
inline const std::unordered_map<HttpVerb::Value, std::string>
    HttpVerb::enum_to_string = {{HttpVerb::Value::GET, "GET"},
                                {HttpVerb::Value::POST, "POST"},
                                {HttpVerb::Value::PUT, "PUT"},
                                {HttpVerb::Value::DELETE, "DELETE"}};

inline const std::unordered_map<std::string, HttpVerb::Value>
    HttpVerb::string_to_enum = []() {
      std::unordered_map<std::string, HttpVerb::Value> result;
      for (const auto &[value, str] : enum_to_string) {
        result[str] = value;
      }
      return result;
    }();

} // namespace flash

#endif
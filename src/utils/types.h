#ifndef FLASH_TYPES_H
#define FLASH_TYPES_H

#include <functional>
#include <string>

namespace flash {
using Handler = std::function<void(std::string, std::string)>;
using Callback = std::function<void()>;
using Request = std::string;  // TODO: Replace with proper struct
using Response = std::string; // TODO: Replace with proper struct

} // namespace flash

#endif
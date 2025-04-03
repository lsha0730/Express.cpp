#ifndef FLASH_TYPES_H
#define FLASH_TYPES_H

#include <functional>
#include <string>

namespace flash {
using Handler = std::function<void(std::string, std::string)>;
using Callback = std::function<void()>;

} // namespace flash

#endif
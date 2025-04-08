#ifndef FLASH_PUBLIC_TYPES_H
#define FLASH_PUBLIC_TYPES_H

#include "request.h"
#include <functional>
#include <string>

namespace flash {

class Request;

using Handler = std::function<void(Request &request, std::string)>;
using Callback = std::function<void()>;
using Response = std::string; // TODO: Replace with proper struct
} // namespace flash

#endif
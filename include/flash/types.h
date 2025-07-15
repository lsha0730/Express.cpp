#ifndef FLASH_PUBLIC_TYPES_H
#define FLASH_PUBLIC_TYPES_H

#include "request.h"
#include <functional>
#include <string>

namespace flash {

class Request;
class Response;

using Handler = std::function<void(Request &request, Response &response)>;
using Callback = std::function<void()>;
} // namespace flash

#endif
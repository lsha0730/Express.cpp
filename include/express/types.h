#ifndef EXPRESS_PUBLIC_TYPES_H
#define EXPRESS_PUBLIC_TYPES_H

#include "request.h"
#include <functional>
#include <string>

namespace express {

class Request;
class Response;

using Handler = std::function<void(Request &request, Response &response)>;
using Callback = std::function<void()>;
} // namespace express

#endif
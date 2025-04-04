#ifndef FLASH_SOCKET_ERROR_H
#define FLASH_SOCKET_ERROR_H

#include <stdexcept>
#include <string>

namespace flash {

// Exception class for socket-related errors
class SocketError : public std::runtime_error {
public:
  explicit SocketError(const std::string &message)
      : std::runtime_error(message) {}
};

} // namespace flash

#endif
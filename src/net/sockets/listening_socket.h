#ifndef FLASH_LISTENING_SOCKET_H
#define FLASH_LISTENING_SOCKET_H

#include <stdio.h>

#include "binding_socket.h"

namespace flash {
class ListeningSocket : public BindingSocket {
public:
  ListeningSocket(SocketConfig config);
  void start_listening();

private:
  int backlog_;
  int listening_;
};
} // namespace flash

#endif
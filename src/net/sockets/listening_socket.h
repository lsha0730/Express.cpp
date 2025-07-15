#ifndef EXPRESS_LISTENING_SOCKET_H
#define EXPRESS_LISTENING_SOCKET_H

#include <stdio.h>

#include "binding_socket.h"

namespace express {
class ListeningSocket : public BindingSocket {
public:
  ListeningSocket(SocketConfig config);
  void start_listening();

private:
  int backlog_;
  int listening_;
};
} // namespace express

#endif
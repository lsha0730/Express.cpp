#ifndef FLASH_BINDING_SOCKET_H
#define FLASH_BINDING_SOCKET_H

#include <stdio.h>

#include "socket.h"

namespace flash {
class BindingSocket : public Socket {
public:
  BindingSocket(SocketConfig config);
  int connect_to_network(int sock, struct sockaddr_in address);
};
}; // namespace flash

#endif
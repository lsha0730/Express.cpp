#ifndef EXPRESS_BINDING_SOCKET_H
#define EXPRESS_BINDING_SOCKET_H

#include <stdio.h>

#include "socket.h"

namespace express {
class BindingSocket : public Socket {
public:
  BindingSocket(SocketConfig config);
  int connect_to_network(int sock, struct sockaddr_in address);
};
}; // namespace express

#endif
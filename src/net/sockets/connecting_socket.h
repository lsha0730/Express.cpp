#ifndef EXPRESS_CONNECTING_SOCKET_H
#define EXPRESS_CONNECTING_SOCKET_H

#include <stdio.h>

#include "socket.h"

namespace express {
class ConnectingSocket : public Socket {
public:
  ConnectingSocket(SocketConfig config);
  int connect_to_network(int sock, struct sockaddr_in address);
};
}; // namespace express

#endif
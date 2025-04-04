#ifndef FLASH_CONNECTING_SOCKET_H
#define FLASH_CONNECTING_SOCKET_H

#include <stdio.h>

#include "socket.h"

namespace flash {
class ConnectingSocket : public Socket {
public:
  ConnectingSocket(SocketConfig config);
  int connect_to_network(int sock, struct sockaddr_in address);
};
}; // namespace flash

#endif
#ifndef ConnectingSocket_h
#define ConnectingSocket_h

#include <stdio.h>

#include "socket.h"

namespace flash {
class ConnectingSocket : public Socket {
public:
  ConnectingSocket(int domain, int service, int protocol, int port,
                   u_long interface);
  int connect_to_network(int sock, struct sockaddr_in address);
};
}; // namespace flash

#endif
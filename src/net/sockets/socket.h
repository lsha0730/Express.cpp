#ifndef EXPRESS_SOCKET_H
#define EXPRESS_SOCKET_H

#include "socket_error.h"
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

namespace express {
struct SocketConfig {
  int domain;
  int service;
  int protocol;
  int port;
  u_long interface;
  int backlog;
};

class Socket {
public:
  Socket(SocketConfig config);
  virtual ~Socket();
  virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
  void test_connection(int);
  struct sockaddr_in address();
  int sock();
  void set_connection(int);

private:
  struct sockaddr_in address_;
  int sock_;
  int connection_;
};
} // namespace express

#endif
#include "socket.h"
#include <cerrno>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

// Default constructor
express::Socket::Socket(SocketConfig config) {
  // Define address structure
  address_.sin_family = config.domain;
  address_.sin_port = htons(config.port);
  address_.sin_addr.s_addr = htonl(config.interface);

  // Establish connection
  sock_ = socket(config.domain, config.service, config.protocol);
  test_connection(sock_);

  // Enable address reuse for all socket types
  int opt = 1;
  setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

express::Socket::~Socket() {
  if (sock_ > 0) {
    close(sock_);
  }
}

// Test connection virtual function
void express::Socket::test_connection(int item_to_test) {
  if (item_to_test < 0) {
    std::string error_msg = "Socket operation failed: ";
    error_msg += std::strerror(errno);
    throw SocketError(error_msg);
  }
}

// Getter functions
struct sockaddr_in express::Socket::address() {
  return address_;
}

int express::Socket::sock() {
  return sock_;
}

// Setter functions
void express::Socket::set_connection(int con) {
  connection_ = con;
}
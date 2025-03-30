#include "socket.h"
#include <cerrno>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

// Default constructor
flash::Socket::Socket(int domain, int service, int protocol, int port,
                      u_long interface) {
  // Define address structure
  address_.sin_family = domain;
  address_.sin_port = htons(port);
  address_.sin_addr.s_addr = htonl(interface);

  // Establish connection
  sock_ = socket(domain, service, protocol);
  test_connection(sock_);

  // Enable address reuse for all socket types
  int opt = 1;
  setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

flash::Socket::~Socket() {
  if (sock_ > 0) {
    close(sock_);
  }
}

// Test connection virtual function
void flash::Socket::test_connection(int item_to_test) {
  if (item_to_test < 0) {
    std::string error_msg = "Socket operation failed: ";
    error_msg += std::strerror(errno);
    throw SocketError(error_msg);
  }
}

// Getter functions
struct sockaddr_in flash::Socket::address() { return address_; }

int flash::Socket::sock() { return sock_; }

// Setter functions
void flash::Socket::set_connection(int con) { connection_ = con; }
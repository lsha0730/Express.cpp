#include "binding_socket.h"

// Constructor
flash::BindingSocket::BindingSocket(int domain, int service, int protocol,
                                    int port, u_long interface)
    : Socket(domain, service, protocol, port, interface) {
  int connection = connect_to_network(sock(), address());
  set_connection(connection);
  test_connection(connection);
}

// Implementation of connect_to_network virtual function
int flash::BindingSocket::connect_to_network(int sock,
                                             struct sockaddr_in address) {
  return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
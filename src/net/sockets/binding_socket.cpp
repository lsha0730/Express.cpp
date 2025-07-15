#include "binding_socket.h"

// Constructor
express::BindingSocket::BindingSocket(SocketConfig config) : Socket(config) {
  int connection = connect_to_network(sock(), address());
  set_connection(connection);
  test_connection(connection);
}

// Implementation of connect_to_network virtual function
int express::BindingSocket::connect_to_network(int sock, struct sockaddr_in address) {
  return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
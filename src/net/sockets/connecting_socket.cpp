#include "connecting_socket.h"

flash::ConnectingSocket::ConnectingSocket(SocketConfig config)
    : Socket(config) {
  int connection = connect_to_network(sock(), address());
  set_connection(connection);
  test_connection(connection);
}

// Implementation of connect_to_network virtual function
int flash::ConnectingSocket::connect_to_network(int sock,
                                                struct sockaddr_in address) {
  return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
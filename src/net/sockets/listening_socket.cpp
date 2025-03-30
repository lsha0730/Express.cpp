#include "listening_socket.h"
#include "socket.h"

flash::ListeningSocket::ListeningSocket(int domain, int service, int protocol,
                                        int port, u_long interface, int backlog)
    : BindingSocket(domain, service, protocol, port, interface) {
  backlog_ = backlog;
  start_listening();
  test_connection(listening_);
}

void flash::ListeningSocket::start_listening() {
  listening_ = listen(sock(), backlog_);
}
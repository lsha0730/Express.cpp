#include "listening_socket.h"
#include "socket.h"

flash::ListeningSocket::ListeningSocket(SocketConfig config)
    : BindingSocket(config) {
  backlog_ = config.backlog;
  start_listening();
  test_connection(listening_);
}

void flash::ListeningSocket::start_listening() {
  listening_ = listen(sock(), backlog_);
}
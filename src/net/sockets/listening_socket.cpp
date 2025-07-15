#include "listening_socket.h"
#include "socket.h"

express::ListeningSocket::ListeningSocket(SocketConfig config) : BindingSocket(config) {
  backlog_ = config.backlog;
  start_listening();
  test_connection(listening_);
}

void express::ListeningSocket::start_listening() {
  listening_ = listen(sock(), backlog_);
}
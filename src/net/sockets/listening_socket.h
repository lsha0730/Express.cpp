#ifndef ListeningSocket_h
#define ListeningSocket_h

#include <stdio.h>

#include "binding_socket.h"

namespace flash {
class ListeningSocket : public BindingSocket {
public:
  ListeningSocket(SocketConfig config);
  void start_listening();

private:
  int backlog_;
  int listening_;
};
} // namespace flash

#endif
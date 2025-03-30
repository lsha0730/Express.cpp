#ifndef Server_h
#define Server_h

#include <stdio.h>

#include "../flash_networking.h"

namespace flash {
class Server {
public:
  Server(int domain, int service, int protocol, int port, u_long interface,
         int backlog);
  virtual void launch() = 0;
  ListeningSocket *socket();

private:
  ListeningSocket *socket_;
  virtual void accepter() = 0;
  virtual void handler() = 0;
  virtual void responder() = 0;
};
}; // namespace flash

#endif
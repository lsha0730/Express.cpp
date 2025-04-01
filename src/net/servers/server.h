#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <unistd.h>
#include <vector>

#include "../../utils/constants.h"
#include "../flash_networking.h"

namespace flash {
class Server {
public:
  Server(int domain, int service, int protocol, int port, u_long interface,
         int backlog);
  virtual void launch() = 0;
  ListeningSocket *socket();

private:
  static constexpr size_t CHUNK_SIZE_ = 8 * constants::KB_;

  ListeningSocket *socket_;
  int new_socket_;
  std::vector<char> buffer_;

  void accepter();
  virtual void handler() = 0;
  virtual void responder() = 0;
};
}; // namespace flash

#endif
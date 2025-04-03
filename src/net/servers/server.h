#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <unistd.h>
#include <vector>

#include "core/routing/router.h"
#include "net/flash_networking.h"
#include "utils/constants.h"
#include "utils/types.h"

namespace flash {
struct ServerConfig { // TODO: Put in lower level as SocketConfig
  int domain;
  int service;
  int protocol;
  int port;
  u_long interface;
  int backlog;
};

class Server {
public:
  Server(ServerConfig config, Router router);
  void launch();
  ListeningSocket *socket();

private:
  static constexpr size_t CHUNK_SIZE_ = 8 * constants::KB_;

  ListeningSocket *socket_;
  int new_socket_;
  std::vector<char> buffer_;
  Router router_;

  void accepter();
  Response handler();
  void responder(Response response);
};
}; // namespace flash

#endif
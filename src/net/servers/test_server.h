#ifndef TestServer_h
#define TestServer_h

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>

#include "../../utils/constants.h"
#include "server.h"

namespace flash {
class TestServer : public Server {
public:
  TestServer(int port);
  void launch();

private:
  static constexpr size_t CHUNK_SIZE_ = 8 * constants::KB_;

  std::vector<char> buffer_;
  int new_socket_;
  void accepter();
  void handler();
  void responder(std::string response);
};
}; // namespace flash

#endif
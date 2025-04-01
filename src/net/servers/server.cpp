#include "server.h"

flash::Server::Server(int domain, int service, int protocol, int port,
                      u_long interface, int backlog) {
  socket_ =
      new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

void flash::Server::accepter() {
  struct sockaddr_in address = socket()->address();
  int addrlen = sizeof(address);
  new_socket_ = accept(socket()->sock(), (struct sockaddr *)&address,
                       (socklen_t *)&addrlen);

  std::vector<char> chunk(CHUNK_SIZE_);

  while (true) {
    ssize_t bytes_read = read(new_socket_, chunk.data(), CHUNK_SIZE_);

    if (bytes_read <= 0)
      break;

    if (buffer_.size() + bytes_read > flash::constants::MAX_REQUEST_SIZE_)
      break;

    buffer_.insert(buffer_.end(), chunk.begin(), chunk.begin() + bytes_read);

    if (bytes_read < CHUNK_SIZE_)
      break;
  }
}

flash::ListeningSocket *flash::Server::socket() { return socket_; }
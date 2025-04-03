#include "server.h"

flash::Server::Server(ServerConfig config, Router router) {
  socket_ = new ListeningSocket(config.domain, config.service, config.protocol,
                                config.port, config.interface, config.backlog);
  router_ = router;
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

flash::Response flash::Server::handler() {
  std::string raw_request = std::string(buffer_.begin(), buffer_.end());
  Request request = raw_request; // TODO: Add parsing step here
  Response response = router_.run(request);
  return response;
}

void flash::Server::responder(Response response) {
  std::string_view response_string =
      response; // TODO: Add deconstruction step here
  write(new_socket_, response.data(), response.size());
  close(new_socket_);
}

flash::ListeningSocket *flash::Server::socket() { return socket_; }

void flash::Server::launch() {
  while (true) {
    accepter();
    Response response = handler();
    responder(response);
  }
}
#include "server.h"

flash::Server::Server(SocketConfig config, Router router) {
  socket_ = new ListeningSocket(config);
  router_ = router;
}

flash::Server::~Server() {
  stop();
  delete socket_;
}

std::unique_ptr<flash::Request> flash::Server::accepter() {
  struct sockaddr_in address = socket()->address();
  int addrlen = sizeof(address);
  new_socket_ = accept(socket()->sock(), (struct sockaddr *)&address,
                       (socklen_t *)&addrlen);

  std::vector<char> buffer;
  read_socket(buffer);

  std::string raw_request = std::string(buffer.begin(), buffer.end());
  return std::make_unique<Request>(raw_request);
}

int flash::Server::read_socket(std::vector<char> &buffer) {
  int total_bytes_read = 0;
  std::vector<char> chunk(CHUNK_SIZE_);

  while (true) {
    ssize_t bytes_read = read(new_socket_, chunk.data(), CHUNK_SIZE_);
    total_bytes_read += bytes_read;

    if (bytes_read <= 0)
      break;

    if (buffer.size() + bytes_read > flash::constants::MAX_REQUEST_SIZE_)
      break;

    buffer.insert(buffer.end(), chunk.begin(), chunk.begin() + bytes_read);

    if (bytes_read < CHUNK_SIZE_)
      break;
  }

  return total_bytes_read;
}

void flash::Server::responder(std::string content) {
  write(new_socket_, content.data(), content.size());
  close(new_socket_);
}

void flash::Server::handle_connection() {
  std::unique_ptr<Request> request = accepter();
  Response response([this](std::string content) { this->responder(content); });
  router_.run(*request, response);
}

flash::ListeningSocket *flash::Server::socket() { return socket_; }

void flash::Server::launch() {
  if (is_running)
    return;
  is_running = true;
  server_thread_ = std::thread(&Server::run, this);
}

void flash::Server::run() {
  while (is_running) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socket_->sock(), &readfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = constants::DEFAULT_SELECT_TIMEOUT_US;

    int activity = select(socket_->sock() + 1, &readfds, NULL, NULL, &tv);
    bool got_new_connection = activity > 0;

    if (got_new_connection) {
      handle_connection();
    }
  }
}

void flash::Server::stop() {
  if (!is_running)
    return;
  is_running = false;

  try {
    if (server_thread_.joinable()) {
      server_thread_.join();
    }
  } catch (const std::exception &e) {
    throw std::runtime_error("Failed to stop server: " + std::string(e.what()));
  }
}
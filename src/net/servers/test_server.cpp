#include "test_server.h"
using namespace std;

flash::TestServer::TestServer(int port)
    : Server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10) {
  launch();
}

void flash::TestServer::accepter() {
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

void flash::TestServer::handler() {
  // TODO: Handle this string
  cout << string(buffer_.begin(), buffer_.end()) << endl;
}

void flash::TestServer::responder(string response) {
  write(new_socket_, response.c_str(), response.length());
  close(new_socket_);
}

void flash::TestServer::launch() {
  while (true) {
    accepter();
    handler();
    responder("Hello world");
  }
}
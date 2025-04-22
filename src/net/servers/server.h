#ifndef FLASH_SERVER_H
#define FLASH_SERVER_H

#include <memory>
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "core/router.h"
#include "include/flash/types.h"
#include "net/flash_networking.h"
#include "utils/constants.h"

namespace flash {
class Server {
public:
  Server(SocketConfig config, Router router);
  ~Server();

  /** Flag indicating if server is running */
  std::atomic<bool> is_running{false};

  /**
   * Starts the server to receive and handle requests, on a new thread.
   * Does nothing if the server is already running.
   */
  void launch();

  /**
   * Stops the server and waits for the server thread to finish.
   * Safe to call multiple times.
   */
  void stop();

  /**
   * Returns the socket the server is listening on.
   */
  ListeningSocket *socket();

private:
  /** Size of each chunk when reading request data (8KB) */
  static constexpr size_t CHUNK_SIZE_ = 8 * constants::KB_;

  /** Socket for accepting incoming connections */
  ListeningSocket *socket_;

  /** Temporary socket for current client connection */
  int new_socket_;

  /** Router for handling requests */
  Router router_;

  /** Thread running the server loop */
  std::thread server_thread_;

  /**
   * Starts the server if it is marked as running.
   * @private
   */
  void run();

  /**
   * Reads from socket, processes, and writes response to socket.
   * @private
   */
  void handle_connection();

  /**
   * Reads the raw data chunk-by-chunk into buffer.
   * @private
   * @return Total number of bytes read.
   */
  int read_socket(std::vector<char> &buffer);

  /**
   * Accepts a request and reads the raw data chunk-by-chunk into the internal
   * buffer. Parses into Request object.
   * @private
   * @return Pointer to the parsed Request object.
   */
  std::unique_ptr<flash::Request> accepter();

  /**
   * Writes given string to socket.
   * @private
   */
  void responder(std::string content);
};
}; // namespace flash

#endif
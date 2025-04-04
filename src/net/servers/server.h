#ifndef FLASH_SERVER_H
#define FLASH_SERVER_H

#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "core/routing/router.h"
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

  /** Buffer for reading request data */
  std::vector<char> buffer_;

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
   * Accepts a request and reads the raw data chunk-by-chunk into the internal
   * buffer.
   * @private
   */
  void accepter();

  /**
   * Parses the buffer into a Request object, then delegates execution to Router
   * instance. Clears the buffer.
   * @return Response object with response data
   * @private
   */
  Response handler();

  /**
   * Encodes Response object into strings before writing to socket.
   * @private
   */
  void responder(Response response);
};
}; // namespace flash

#endif
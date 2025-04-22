#ifndef FLASH_PUBLIC_RESPONSE_H
#define FLASH_PUBLIC_RESPONSE_H

#include "types.h"
#include <functional>
#include <memory>
#include <string>

namespace flash {
class Server;

class Response {
public:
  /**
   * Sends a JSON-serializable response back to the client.
   * @tparam T Type that can be converted to JSON
   * @param data The data to send as JSON
   * @returns Reference to this response for chaining
   */
  template <typename T> Response &send(const T &data);

  /**
   * Sends a JSON-serializable response back to the client with pretty printing.
   * @tparam T Type that can be converted to JSON
   * @param data The data to send as JSON
   * @returns Reference to this response for chaining
   */
  template <typename T> Response &json(const T &data);

  /**
   * Sets the HTTP status code.
   * @param code The status code to set
   * @returns Reference to this response for chaining
   */
  Response &status(int code);

  // Rule of 5
  ~Response();
  Response(const Response &) = delete;
  Response &operator=(const Response &) = delete;
  Response(Response &&) = default;
  Response &operator=(Response &&) = default;

private:
  friend class Server;
  explicit Response(std::function<void(const std::string &)> on_send);

  class Impl;
  std::unique_ptr<Impl> pImpl;
};

} // namespace flash

#endif
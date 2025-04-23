#ifndef FLASH_PUBLIC_RESPONSE_H
#define FLASH_PUBLIC_RESPONSE_H

#include "types.h"
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace flash {
class Server;

class Response {
public:
  /**
   * Sends a response to the client with the specified body content.
   * @tparam T Type, either Buffer, String, Object, Array, or Number.
   * @param data The data to send.
   * @note Numbers are interpreted as status codes.
   * @returns Reference to this response for chaining
   */
  template <typename T> Response &send(const T &data);

  /**
   * Sends a response to the client with the included data as serialized JSON string.
   * @warning Any structs that are passed in must be registered with one of
   * FLASH_REGISTER_STRUCT_INTRUSIVE or FLASH_REGISTER_STRUCT_NON_INTRUSIVE.
   * @tparam T Type that can be converted to JSON
   * @param data The data to send as JSON
   * @returns Reference to this response for chaining
   */
  template <typename T> Response &json(const T &data) {
    nlohmann::json s = nlohmann::json(data).dump(4);
    return json_(std::move(s));
  }

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

  Response &json_(const std::string &data);

  class Impl;
  std::unique_ptr<Impl> pImpl;
};

} // namespace flash

#endif
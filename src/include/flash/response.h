#ifndef FLASH_PUBLIC_RESPONSE_H
#define FLASH_PUBLIC_RESPONSE_H

#include "types.h"
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace flash {
class Server;

class Response {
public:
  /**
   * Sends a response to the client with the specified body content.
   * @tparam T Type, either Buffer, String, Object, Array, or Number.
   * @param data The data to send.
   * @note Numbers are interpreted as status codes.
   * @note Infers the Content-Type header from the data type.
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   * @returns Reference to this response for chaining
   */
  template <typename T> Response &send(const T &data);

  /**
   * Sends a nlohmann::json object to the client with the included data as serialized JSON string.
   * @tparam T Type that can be converted to JSON
   * @param data The data to send.
   * @note Sets Content-Type header to "application/json; charset=utf-8".
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   * @returns Reference to this response for chaining
   */
  Response &json(const nlohmann::json &data) {
    nlohmann::json s = data.dump(4);
    return json_(std::move(s));
  }

  /**
   * Sends a response to the client with the included data as serialized JSON string.
   * @warning Any structs that are passed in must be registered with one of
   * FLASH_REGISTER_STRUCT_INTRUSIVE or FLASH_REGISTER_STRUCT_NON_INTRUSIVE.
   * @tparam T Type that can be converted to JSON
   * @param data The data to send.
   * @note Sets Content-Type header to "application/json; charset=utf-8".
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   * @returns Reference to this response for chaining
   */
  template <typename T> Response &json(const T &data) {
    nlohmann::json s = nlohmann::json(data).dump(4);
    return json_(std::move(s));
  }

  /**
   * Sets the status code of the response
   * @param code HTTP status code (e.g., 200, 404, 500)
   * @returns Reference to this response for chaining
   */
  Response &status(int code);

  /**
   * Sets a response header.
   * @param header
   * @param value
   */
  Response &set(const std::string &header, const std::string &value);

  /**
   * Ends the response without sending any data.
   * @param header
   * @param value
   */
  void end();

  // Rule of 5
  ~Response();
  Response(const Response &) = delete;
  Response &operator=(const Response &) = delete;
  Response(Response &&) = default;
  Response &operator=(Response &&) = default;

private:
  friend class Server;
  explicit Response(std::function<void(const std::vector<char>)> write_to_socket,
                    std::function<void()> close_socket);

  Response &json_(const std::string &data);

  class Impl;
  std::unique_ptr<Impl> pImpl;
};

} // namespace flash

#endif
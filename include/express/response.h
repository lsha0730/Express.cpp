#ifndef EXPRESS_PUBLIC_RESPONSE_H
#define EXPRESS_PUBLIC_RESPONSE_H

#include "concepts.h"
#include "types.h"
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace express {
class Server;

class Response {
public:
  /**
   * Sends a response to the client with the specified body content.
   * @param data The data to send.
   * @note Numbers are interpreted as status codes.
   * @note Infers the Content-Type header from the data type.
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   * @returns Reference to this response for chaining
   */
  template <Sendable T> Response &send(const T &data);

  /**
   * @brief Sends a raw string literal to the client.
   *
   * @tparam N    Size of the array (including the '\0' terminator).
   * @param lit  The string literal to send.
   * @return      Reference to this Response for chaining.
   */
  template <size_t N> Response &send(const char (&lit)[N]) { return send(std::string_view{lit}); }

  /**
   * @brief Fallback for types serializable to JSON (via nlohmann::json{T})
   *        but not already handled by other send<> overloads.
   *
   * @tparam T     The type to serialize (must satisfy JsonLike and not Sendable).
   * @param data   The object to JSON-serialize.
   * @returns      Reference to this Response for chaining.
   */
  template <JsonLike T>
    requires(!Sendable<T>)
  Response &send(const T &data) {
    return json(nlohmann::json{data});
  }

  /**
   * Sends a nlohmann::json object to the client with the included data as serialized JSON string.
   * @tparam T Type that can be converted to JSON
   * @param data The data to send.
   * @note Sets Content-Type header to "application/json; charset=utf-8".
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   * @returns Reference to this response for chaining
   */
  Response &json(const nlohmann::json &data);

  /**
   * Sends a response to the client with the included data as serialized JSON string.
   * @warning Any structs that are passed in must be registered with one of
   * EXPRESS_REGISTER_STRUCT_INTRUSIVE or EXPRESS_REGISTER_STRUCT_NON_INTRUSIVE.
   * @tparam T Type that can be converted to JSON
   * @param data The data to send.
   * @note Sets Content-Type header to "application/json; charset=utf-8".
   * @warning Finalizing action. Locks down the response from further sends.
   * @throws Error if a redundant send is attempted.
   * @returns Reference to this response for chaining
   */
  template <typename T> Response &json(const T &data) {
    std::string serialized = nlohmann::json(data).dump(2);
    return json_str(std::move(serialized));
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
   * Gets the specified response header.
   * @param header
   */
  std::string get(const std::string &header);

  /**
   * @returns The HTTP status code.
   * @note Deviates from Express.js API in the name of cpp best practices!
   */
  int status_code();

  /**
   * @returns A boolean indicating whether or not the response has been sent.
   * @note Deviates from Express.js API in the name of cpp best practices!
   */
  bool headers_sent();

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

protected:
  explicit Response(std::function<void(const std::vector<char>)> write_to_socket,
                    std::function<void()> close_socket);

private:
  friend class Server;
  Response &json_str(const std::string &data);

  class Impl;
  std::unique_ptr<Impl> pImpl;
};

} // namespace express

#endif
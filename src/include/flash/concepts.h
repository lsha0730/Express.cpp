#ifndef FLASH_CONCEPTS_H
#define FLASH_CONCEPTS_H

#include <concepts>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace flash {
/**
 * @brief Concept for contiguous byte containers that can be sent as raw binary data
 *
 * Includes vector and span types for char, uint8_t, and std::byte.
 */
template <typename T>
concept BufferLike = std::same_as<std::decay_t<T>, std::vector<char>> ||
                     std::same_as<std::decay_t<T>, std::vector<uint8_t>> ||
                     std::same_as<std::decay_t<T>, std::vector<std::byte>> ||
                     std::same_as<std::decay_t<T>, std::span<char>> ||
                     std::same_as<std::decay_t<T>, std::span<uint8_t>> ||
                     std::same_as<std::decay_t<T>, std::span<std::byte>>;

/**
 * @brief Concept for types that can be converted to string_view
 *
 * Includes std::string, char arrays, char, and filesystem paths.
 */
template <typename T>
concept StringLike =
    std::convertible_to<T, std::string_view> ||
    std::same_as<std::decay_t<T>, char> ||                // Treat char like string
    std::same_as<std::decay_t<T>, std::filesystem::path>; // Filesystem path as string

/**
 * @brief Concept for boolean values
 */
template <typename T>
concept BoolLike = std::same_as<std::decay_t<T>, bool>;

/**
 * @brief Concept for numeric types (excluding bool)
 *
 * Includes all arithmetic types except bool.
 */
template <typename T>
concept NumberLike = std::is_arithmetic_v<T> && !BoolLike<T>;

/**
 * @brief Concept for null values
 */
template <typename T>
concept NullLike = std::same_as<std::decay_t<T>, std::nullptr_t>;

/**
 * @brief Concept for nlohmann::json objects
 */
template <typename T>
concept Json = std::same_as<std::decay_t<T>, nlohmann::json>;

/**
 * @brief Concept for map-like containers with string keys
 *
 * MapLike types are containers that have key-value pairs with string keys.
 * Excludes StringLike, BufferLike, and PairOrTuple types.
 */
template <typename T>
concept MapLike =
    requires(T t) {
      typename T::key_type;
      typename T::mapped_type;
      std::begin(t);
      std::end(t);
    } && std::convertible_to<typename T::key_type, std::string> && !StringLike<T> &&
    !BufferLike<T> && !PairOrTuple<T>;

/**
 * @brief Concept for iterable containers
 *
 * Iterable types are containers that can be iterated over.
 * Excludes StringLike, BufferLike, MapLike, and RawArray types.
 */
template <typename T>
concept Iterable = (requires(T t) {
                     std::begin(t);
                     std::end(t);
                   } || RawArray<T>) && !StringLike<T> && !BufferLike<T> && !MapLike<T>;

/**
 * @brief Concept for raw C-style arrays
 *
 * RawArray types are treated as a special case of iterable containers.
 */
template <typename T>
concept RawArray = std::is_array_v<T>;

/**
 * @brief Concept for object-like types
 *
 * ObjectLike types are either MapLike or Iterable containers.
 */
template <typename T>
concept ObjectLike = MapLike<T> || Iterable<T>;

/**
 * @brief Concept for all types that can be sent in a response
 *
 * Sendable types include all the basic response types:
 * - BufferLike (raw binary)
 * - StringLike (text)
 * - BoolLike (boolean)
 * - NumberLike (status codes)
 * - NullLike (null values)
 * - ObjectLike (containers)
 * - Json (JSON objects)
 */
template <typename T>
concept Sendable = BufferLike<T> || StringLike<T> || BoolLike<T> || NumberLike<T> || NullLike<T> ||
                   ObjectLike<T> || Json<T>;
} // namespace flash

#endif
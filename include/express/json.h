#ifndef EXPRESS_PUBLIC_JSON_H
#define EXPRESS_PUBLIC_JSON_H

/**
 * @file json.h
 * @brief JSON serialization utilities for Express
 */

#include <nlohmann/json.hpp>

namespace express {

/**
 * @brief Register a struct for JSON serialization (intrusive version)
 *
 * This macro wraps nlohmann/json's NLOHMANN_DEFINE_TYPE_INTRUSIVE macro.
 * Use this when you can modify the struct definition.
 *
 * @param Type The struct type to register
 * @param ... The member variables to include in serialization
 */
#define EXPRESS_REGISTER_STRUCT_INTRUSIVE(Type, ...)                                               \
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, __VA_ARGS__)

/**
 * @brief Register a struct for JSON serialization (non-intrusive version)
 *
 * This macro wraps nlohmann/json's NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE macro.
 * Use this when you cannot modify the struct definition.
 *
 * @param Type The struct type to register
 * @param ... The member variables to include in serialization
 */
#define EXPRESS_REGISTER_STRUCT_NON_INTRUSIVE(Type, ...)                                           \
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Type, __VA_ARGS__)

} // namespace express

#endif // EXPRESS_PUBLIC_JSON_H
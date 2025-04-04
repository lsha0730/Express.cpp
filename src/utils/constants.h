#ifndef FLASH_CONSTANTS_H
#define FLASH_CONSTANTS_H

namespace flash {
namespace constants {
// Size constants
static constexpr size_t KB_ = 1024;
static constexpr size_t MB_ = KB_ * 1024;
static constexpr size_t GB_ = MB_ * 1024;
static constexpr size_t MAX_REQUEST_SIZE_ = 1 * MB_;

// Socket config defaults
static constexpr int DEFAULT_DOMAIN = AF_INET;
static constexpr int DEFAULT_SERVICE = SOCK_STREAM;
static constexpr int DEFAULT_PROTOCOL = 0;
static constexpr u_long DEFAULT_INTERFACE = INADDR_ANY;
static constexpr int DEFAULT_BACKLOG = 511;
} // namespace constants
} // namespace flash

#endif
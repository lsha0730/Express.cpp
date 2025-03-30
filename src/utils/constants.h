#ifndef FLASH_CONSTANTS_H
#define FLASH_CONSTANTS_H

namespace flash
{
    namespace constants
    {
        // Size constants
        static constexpr size_t KB_ = 1024;
        static constexpr size_t MB_ = KB_ * 1024;
        static constexpr size_t GB_ = MB_ * 1024;
        static constexpr size_t MAX_REQUEST_SIZE_ = 1 * MB_;
    }
}

#endif
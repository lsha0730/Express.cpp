#ifndef SocketError_h
#define SocketError_h

#include <stdexcept>
#include <string>

namespace flash
{

    // Exception class for socket-related errors
    class SocketError : public std::runtime_error
    {
    public:
        explicit SocketError(const std::string &message)
            : std::runtime_error(message) {}
    };

}

#endif
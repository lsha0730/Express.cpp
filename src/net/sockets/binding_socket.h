#ifndef BindingSocket_h
#define BindingSocket_h

#include <stdio.h>

#include "socket.h"

namespace flash
{
    class BindingSocket : public Socket
    {
    public:
        BindingSocket(int domain, int service, int protocol, int port, u_long interface);
        int connect_to_network(int sock, struct sockaddr_in address);
    };
};

#endif
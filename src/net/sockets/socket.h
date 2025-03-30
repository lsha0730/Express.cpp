#ifndef Socket_h
#define Socket_h

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "socket_error.h"

namespace flash
{
    class Socket
    {
    public:
        Socket(int domain, int service, int protocol, int port, u_long interface);
        virtual ~Socket();
        virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
        void test_connection(int);
        struct sockaddr_in address();
        int sock();
        void set_connection(int);

    private:
        struct sockaddr_in address_;
        int sock_;
        int connection_;
    };
}

#endif
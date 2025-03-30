#include "server.h"

flash::Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
    socket_ = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

flash::ListeningSocket *flash::Server::socket()
{
    return socket_;
}
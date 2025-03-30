#ifndef TestServer_h
#define TestServer_h

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>

#include "server.h"
#include "../../utils/constants.h"

namespace flash
{
    class TestServer : public Server
    {
    public:
        TestServer(int port);
        void launch();

    private:
        static constexpr size_t CHUNK_SIZE_ = 8 * constants::KB_;

        std::vector<char> buffer_;
        int new_socket_;
        void accepter();
        void handler();
        void responder(std::string response);
    };
};

#endif
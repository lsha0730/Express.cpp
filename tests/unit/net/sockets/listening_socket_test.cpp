#include "net/sockets/listening_socket.cpp"
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>

constexpr int SERVER_PORT = 8080;
constexpr u_long SERVER_INTERFACE = INADDR_ANY;
constexpr int BACKLOG = 3;

/**
 * Test fixture for ListeningSocket tests.
 * Creates a ListeningSocket for every test and cleans them up.
 */
class ListeningSocketFixture : public ::testing::Test {
protected:
  std::unique_ptr<flash::ListeningSocket> server_socket;

  void SetUp() override {
    server_socket = std::make_unique<flash::ListeningSocket>(
        AF_INET, SOCK_STREAM, 0, SERVER_PORT, SERVER_INTERFACE, BACKLOG);
  }

  ~ListeningSocketFixture() noexcept override = default;
};

TEST_F(ListeningSocketFixture, SocketIsListeningAfterInit) {
  // Create a test client socket
  int client_sock = socket(AF_INET, SOCK_STREAM, 0);
  ASSERT_GE(client_sock, 0) << "Failed to create client socket";

  // Set up the server address
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Try to connect to the server
  int connect_result = connect(client_sock, (struct sockaddr *)&server_addr,
                               sizeof(server_addr));

  // The connection should either succeed or fail with EINPROGRESS/EWOULDBLOCK
  // Both indicate the socket is listening
  EXPECT_TRUE(connect_result == 0 || errno == EINPROGRESS ||
              errno == EWOULDBLOCK)
      << "Socket is not in listening state. Error: " << strerror(errno);

  // Clean up
  close(client_sock);
}
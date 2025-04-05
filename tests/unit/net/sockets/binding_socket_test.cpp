#include "net/sockets/binding_socket.h"
#include <gtest/gtest.h>

/**
 * Test fixture for BindingSocket tests.
 * Provides a fresh socket instance for each test and handles cleanup.
 */
class BindingSocketFixture : public ::testing::Test {
protected:
  std::unique_ptr<flash::BindingSocket> socket;

  void SetUp() override {
    flash::SocketConfig config = {AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY};
    socket = std::make_unique<flash::BindingSocket>(config);
  }

  ~BindingSocketFixture() noexcept override = default;
};

// Verify socket address structure is correctly initialized
TEST_F(BindingSocketFixture, TestCorrectAddressInitialization) {
  struct sockaddr_in addr = socket->address();

  EXPECT_EQ(addr.sin_family, AF_INET);
  EXPECT_EQ(ntohs(addr.sin_port), 8080);
  EXPECT_EQ(ntohl(addr.sin_addr.s_addr), INADDR_ANY);
}

// Verify socket descriptor is valid
TEST_F(BindingSocketFixture, GetSockCorrectValue) {
  int sock = socket->sock();
  EXPECT_TRUE(sock > 0);
}

// Test error handling for invalid socket operations
TEST_F(BindingSocketFixture, TestConnectionCorrectReturn) {
  EXPECT_NO_THROW(socket->test_connection(3));
  EXPECT_ANY_THROW(socket->test_connection(-1));
}

// Verify successful socket binding
TEST_F(BindingSocketFixture, SocketIsBoundAfterInitialization) {
  int sock = socket->sock();
  struct sockaddr_in addr = socket->address();

  // Get the local address bound to the socket
  struct sockaddr_in bound_addr;
  socklen_t addr_len = sizeof(bound_addr);
  int result = getsockname(sock, (struct sockaddr *)&bound_addr, &addr_len);

  EXPECT_EQ(result, 0) << "Failed to get socket name: " << std::strerror(errno);
  EXPECT_EQ(bound_addr.sin_family, addr.sin_family);
  EXPECT_EQ(bound_addr.sin_port, addr.sin_port);
  EXPECT_EQ(bound_addr.sin_addr.s_addr, addr.sin_addr.s_addr);
}
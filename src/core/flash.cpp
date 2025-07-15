#include "core/router.h"
#include "net/servers/server.h"
#include "utils/constants.h"
#include <express/express.h>
#include <memory>

namespace express {

// Define the implementation class
class Express::Impl : public Router {
public:
  Impl() = default;

  void listen(int port, Callback callback) {
    using namespace express::constants;
    express::SocketConfig config = {DEFAULT_DOMAIN, DEFAULT_SERVICE,   DEFAULT_PROTOCOL,
                                    port,           DEFAULT_INTERFACE, DEFAULT_BACKLOG};
    server_ = std::make_unique<Server>(config, *this);
    server_->launch();
    callback();
    block_while_running();
  }

  void shutdown() { server_ = nullptr; }

  void get(std::string route, Handler handler) { Router::get(route, std::move(handler)); }

  void post(std::string route, Handler handler) { Router::post(route, std::move(handler)); }

  void put(std::string route, Handler handler) { Router::put(route, std::move(handler)); }

  void del(std::string route, Handler handler) { Router::del(route, std::move(handler)); }

private:
  std::unique_ptr<Server> server_;
  void block_while_running() {
    while (server_->is_running) {
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = constants::DEFAULT_SELECT_TIMEOUT_US;

      select(0, NULL, NULL, NULL, &tv);
    }
  }
};

// Constructor
Express::Express() : pImpl(std::make_unique<Impl>()) {
}

// Destructor (can be defaulted since unique_ptr handles cleanup)
Express::~Express() = default;

// Factory function
Express Express::express() {
  return Express();
}

// Method implementations
void Express::listen(int port, Callback callback) {
  pImpl->listen(port, std::move(callback));
}

void Express::shutdown() {
  pImpl->shutdown();
}

void Express::get(std::string route, Handler handler) {
  pImpl->get(route, std::move(handler));
}

void Express::post(std::string route, Handler handler) {
  pImpl->post(route, std::move(handler));
}

void Express::put(std::string route, Handler handler) {
  pImpl->put(route, std::move(handler));
}

void Express::del(std::string route, Handler handler) {
  pImpl->del(route, std::move(handler));
}

} // namespace express
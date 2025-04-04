#include "include/flash/flash.h"
#include "core/routing/router.h"
#include "net/servers/server.h"
#include "utils/constants.h"
#include <memory>

namespace flash {

// Define the implementation class
class Flash::Impl : public Router {
public:
  Impl() = default;

  void listen(int port, Callback callback) {
    using namespace flash::constants;
    flash::ServerConfig config = {DEFAULT_DOMAIN,    DEFAULT_SERVICE,
                                  DEFAULT_PROTOCOL,  port,
                                  DEFAULT_INTERFACE, DEFAULT_BACKLOG};
    server = std::make_unique<Server>(config, *this);
    server->launch();
    callback();
  }

  void shutdown() {
    // TODO: Implementation
  }

  void get(std::string route, Handler handler) {
    Router::get(route, std::move(handler));
  }

  void post(std::string route, Handler handler) {
    Router::post(route, std::move(handler));
  }

  void put(std::string route, Handler handler) {
    Router::put(route, std::move(handler));
  }

  void del(std::string route, Handler handler) {
    Router::del(route, std::move(handler));
  }

private:
  std::unique_ptr<Server> server;
};

// Constructor
Flash::Flash() : pImpl(std::make_unique<Impl>()) {}

// Destructor (can be defaulted since unique_ptr handles cleanup)
Flash::~Flash() = default;

// Factory function
Flash Flash::flash() { return Flash(); }

// Method implementations
void Flash::listen(int port, Callback callback) {
  pImpl->listen(port, std::move(callback));
}

void Flash::shutdown() { pImpl->shutdown(); }

void Flash::get(std::string route, Handler handler) {
  pImpl->get(route, std::move(handler));
}

void Flash::post(std::string route, Handler handler) {
  pImpl->post(route, std::move(handler));
}

void Flash::put(std::string route, Handler handler) {
  pImpl->put(route, std::move(handler));
}

void Flash::del(std::string route, Handler handler) {
  pImpl->del(route, std::move(handler));
}

} // namespace flash
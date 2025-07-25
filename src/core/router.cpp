#include "router.h"

express::Router::Router() = default;

void express::Router::register_handler(HttpVerb::Value verb, Handler new_handler) {
  std::vector<Handler> &verb_handlers = handlers_[verb];
  verb_handlers.push_back(new_handler);
}

void express::Router::get(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::GET, std::move(handler));
}

void express::Router::post(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::POST, std::move(handler));
}

void express::Router::put(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::PUT, std::move(handler));
}

void express::Router::del(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::DELETE, std::move(handler));
}

void express::Router::use(std::string subroute, Router router) {
  subrouters_[subroute] = router;
}

void express::Router::run(Request &request, Response &response) {
  HttpVerb::Value http_verb = HttpVerb::toEnum(request.method);
  if (request.path == "/") {
    std::vector<Handler> &handlers = handlers_[http_verb];
    for (Handler &handler : handlers) {
      handler(request, response);
    }
  } else {
    // TODO: Recursive matching
  }
}
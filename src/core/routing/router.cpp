#include "router.h"

flash::Router::Router() = default;

void flash::Router::register_handler(HttpVerb::Value verb,
                                     Handler new_handler) {
  std::vector<Handler> &verb_handlers = handlers_[verb];
  verb_handlers.push_back(new_handler);
}

void flash::Router::get(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::GET, handler);
}

void flash::Router::post(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::POST, handler);
}

void flash::Router::put(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::PUT, handler);
}

void flash::Router::del(std::string route, Handler handler) {
  register_handler(HttpVerb::Value::DELETE, handler);
}

void flash::Router::use(std::string subroute, Router router) {
  subrouters_[subroute] = router;
}

flash::Response flash::Router::run(Request &request) {
  // TODO: Replace mock implementation
  std::cout << request.path << std::endl;
  return request.path;
}
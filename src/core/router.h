#ifndef EXPRESS_ROUTER_H
#define EXPRESS_ROUTER_H

#include "http/http_verb.h"
#include <express/request.h>
#include <express/response.h>
#include <express/types.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace express {
class Router {
public:
  Router();
  // Execute incoming request
  void run(Request &request, Response &response);

  // Handler registration functions
  void del(std::string route, Handler handler);
  void get(std::string route, Handler handler);
  void post(std::string route, Handler handler);
  void put(std::string route, Handler handler);

  // Sub-router registration
  void use(std::string subroute, Router router);

private:
  std::map<HttpVerb::Value, std::vector<Handler>> handlers_;
  void register_handler(HttpVerb::Value verb, Handler new_handler);
  std::map<std::string, Router> subrouters_;
  // TODO: Middleware stack
};
} // namespace express

#endif
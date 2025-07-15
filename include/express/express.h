#ifndef EXPRESS_PUBLIC_H
#define EXPRESS_PUBLIC_H

#include "types.h"
#include <functional>
#include <memory>
#include <string>

namespace express {
class Express {
public:
  static Express express();
  void listen(int port, Callback callback = Callback());
  void shutdown();

  // HTTP method handlers
  void get(std::string route, Handler handler);
  void post(std::string route, Handler handler);
  void put(std::string route, Handler handler);
  void del(std::string route, Handler handler);

  // Rule of 5
  ~Express();
  Express(const Express &) = delete;
  Express &operator=(const Express &) = delete;
  Express(Express &&) = default;
  Express &operator=(Express &&) = default;

private:
  Express();
  class Impl;
  std::unique_ptr<Impl> pImpl;
};

inline Express express() {
  return Express::express();
}

} // namespace express

#endif
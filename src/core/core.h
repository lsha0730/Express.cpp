#ifndef FLASH_CORE_H
#define FLASH_CORE_H

#include <functional>
#include <string>

namespace flash {
class Flash {
public:
  using Handler = std::function<void(std::string, std::string)>;
  using Callback = std::function<void()>;

  Flash();
  void listen(int port, Callback callback = Callback());
  // TODO
  // void del(std::string route, Handler callback);
  void get(std::string route, Handler callback);
  // void post(std::string route, Handler callback);
  // void put(std::string route, Handler callback);
  void shutdown();
};
} // namespace flash

#endif
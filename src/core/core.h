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
  void post(std::string route, Handler callback);
  void shutdown();
};
} // namespace flash

#endif
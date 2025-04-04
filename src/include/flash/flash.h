#ifndef FLASH_PUBLIC_H
#define FLASH_PUBLIC_H

#include "include/flash/types.h" // Include your types
#include <functional>
#include <memory>
#include <string>

namespace flash {
class Flash {
public:
  static Flash flash();
  void listen(int port, Callback callback = Callback());
  void shutdown();

  // HTTP method handlers
  void get(std::string route, Handler handler);
  void post(std::string route, Handler handler);
  void put(std::string route, Handler handler);
  void del(std::string route, Handler handler);

  // Rule of 5
  ~Flash();
  Flash(const Flash &) = delete;
  Flash &operator=(const Flash &) = delete;
  Flash(Flash &&) = default;
  Flash &operator=(Flash &&) = default;

private:
  Flash();
  class Impl;
  std::unique_ptr<Impl> pImpl;
};

} // namespace flash

#endif
#include "core/core.h"
#include <cpr/cpr.h>
#include <gtest/gtest.h>

TEST(CoreTest, ReceivesRequest) {
  flash::Flash app = flash::Flash();
  std::string temp;
  app.post("/test", [&temp](std::string req, std::string res) { temp = req; });

  std::thread server_thread([&app]() {
    app.listen(1234, [&app]() {
      auto response = cpr::Post(cpr::Url{"http://localhost:1234/test"});
      app.shutdown();
    });
  });

  server_thread.join();
  EXPECT_FALSE(temp.empty());
}
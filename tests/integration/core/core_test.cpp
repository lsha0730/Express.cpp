// #include "core/core.h"
// #include <cpr/cpr.h>
// #include <format>
// #include <gtest/gtest.h>

// TEST(CoreTest, ReceivesRequestPost) {
//   express::Express app = express::Express();
//   int port = 1234;
//   std::string temp;
//   app.get("/test", [&temp](std::string req, std::string res) { temp = req;
//   });

//   std::thread server_thread([&app, port]() {
//     app.listen(port, [&app, port]() {
//       std::string url = "http://localhost:" + std::to_string(port) + "/test";
//       auto response = cpr::Get(cpr::Url{url});
//       app.shutdown();
//     });
//   });

//   server_thread.join();
//   EXPECT_FALSE(temp.empty());
// }
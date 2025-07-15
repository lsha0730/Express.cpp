#include <express/express.h>
#include <express/json.h>
#include <express/response.h>
#include <express/types.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
  auto app = express::express();

  // Define routes
  app.get("/", [](const express::Request &req, express::Response &res) {
    res.status(200).send("Hello world!");
  });

  return 0;
}
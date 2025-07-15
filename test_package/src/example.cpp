#include <express/express.h>
#include <express/json.h>
#include <express/response.h>
#include <express/types.h>
#include <iostream>
#include <string>
#include <vector>

struct Job {
  int tc;
  std::string company;

  EXPRESS_REGISTER_STRUCT_INTRUSIVE(Job, tc, company);
};

struct Person {
  int age;
  std::string name;
  Job job;

  EXPRESS_REGISTER_STRUCT_INTRUSIVE(Person, age, name, job);
};

int main() {
  auto app = express::Express::express();

  Job amzn = {160000, "Amazon"};
  Person lincoln = {21, "Lincoln", amzn};

  // Define routes
  app.get("/", [lincoln](const express::Request &req, express::Response &res) {
    res.status(200).send("hello everybody!");
  });

  return 0;
}
#include <flash/flash.h>
#include <flash/json.h>
#include <flash/response.h>
#include <flash/types.h>
#include <iostream>
#include <string>
#include <vector>

struct Job {
  int tc;
  std::string company;

  FLASH_REGISTER_STRUCT_INTRUSIVE(Job, tc, company);
};

struct Person {
  int age;
  std::string name;
  Job job;

  FLASH_REGISTER_STRUCT_INTRUSIVE(Person, age, name, job);
};

int main() {
  auto app = flash::Flash::flash();

  Job amzn = {160000, "Amazon"};
  Person lincoln = {21, "Lincoln", amzn};

  // Define routes
  app.get("/", [lincoln](const flash::Request &req, flash::Response &res) {
    res.status(200).send("hello everybody!");
  });

  return 0;
}
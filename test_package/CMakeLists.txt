cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PackageTest CXX)

find_package(express CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example express::express nlohmann_json::nlohmann_json)

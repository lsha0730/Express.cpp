file(GLOB_RECURSE TEST_SOURCES
    "*.cpp"
)

# tests/CMakeLists.txt
add_executable(express_tests ${TEST_SOURCES})

find_package(cpr REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(express_tests
    PRIVATE
    express
    GTest::gtest_main
    cpr::cpr
    nlohmann_json::nlohmann_json
)

# Add include directories for tests
target_include_directories(express_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/third_party/cpr/include
)

include(GoogleTest)
gtest_discover_tests(express_tests)
file(GLOB_RECURSE TEST_SOURCES
    "*.cpp"
)

# tests/CMakeLists.txt
add_executable(flash_tests ${TEST_SOURCES})

find_package(cpr REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(flash_tests
    PRIVATE
    flash
    GTest::gtest_main
    cpr::cpr
    nlohmann_json::nlohmann_json
)

# Add include directories for tests
target_include_directories(flash_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/third_party/cpr/include
)

include(GoogleTest)
gtest_discover_tests(flash_tests)
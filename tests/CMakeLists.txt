file(GLOB_RECURSE TEST_SOURCES
    "*.cpp"
)

# tests/CMakeLists.txt
add_executable(flash_tests ${TEST_SOURCES})

target_link_libraries(flash_tests
    PRIVATE
    flash
    GTest::gtest_main
)


# Add include directories for tests
target_include_directories(flash_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(flash_tests)
cmake_minimum_required(VERSION 3.14)

project(express
    VERSION 0.1.0
    DESCRIPTION "A modern C++ web framework"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
include(CTest)
enable_testing()

# Get dependencies from Conan
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)

# Find all source files for the library
file(GLOB_RECURSE EXPRESS_SOURCES 
    "src/core/*.cpp"
    "src/http/*.cpp"
    "src/net/*.cpp"
)

# Exclude test files from main library
list(FILTER EXPRESS_SOURCES EXCLUDE REGEX ".*_test\\.cpp$")

# Create main library target
add_library(express ${EXPRESS_SOURCES})

# Set include directories for the library
target_include_directories(express
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add compile options for the library
target_compile_options(express
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Link dependencies
target_link_libraries(express PRIVATE 
    fmt::fmt 
    nlohmann_json::nlohmann_json
)

# Add tests directory if building locally
if (EXISTS "${CMAKE_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()

# Install headers
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS express
    EXPORT expressTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export files
install(EXPORT expressTargets
    FILE expressTargets.cmake
    NAMESPACE express::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/express
)

# Generate package config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "expressConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
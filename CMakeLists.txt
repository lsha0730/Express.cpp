cmake_minimum_required(VERSION 3.14)

# Project definition
project(flash
    VERSION 0.1.0
    DESCRIPTION "A modern C++ web framework"
    LANGUAGES CXX
)

# Enable testing
include(CTest)
enable_testing()
add_subdirectory(third_party/googletest)

# Find all source files automatically
file(GLOB_RECURSE FLASH_SOURCES 
    "src/core/*.cpp"
    "src/http/*.cpp"
    "src/net/*.cpp"
)

# Exclude test files from main library
list(FILTER FLASH_SOURCES EXCLUDE REGEX ".*_test\\.cpp$")

# Create main library target
add_library(flash ${FLASH_SOURCES})

# Set compile features
target_compile_features(flash PRIVATE cxx_std_17)

# Set include directories
target_include_directories(flash
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add compile options
target_compile_options(flash
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS flash
    EXPORT flashTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export files
install(EXPORT flashTargets
    FILE flashTargets.cmake
    NAMESPACE flash::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/flash
)

# Add tests directory
add_subdirectory(tests)

# Generate package config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "flashConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
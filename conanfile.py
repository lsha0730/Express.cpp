from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout, CMake
from conan.tools.build import check_min_cppstd

class FlashConan(ConanFile):
    name            = "flash"
    version         = "0.1.0"
    license         = "MIT"
    author          = "Lincoln <lsha0730@gmail.com>"
    url             = "https://github.com/lsha0730/expresscpp"
    description     = "A multithreaded drop-in C++ replacement for ExpressJS developers."
    topics          = ("http", "web", "framework", "cpp20")

    settings        = "os", "compiler", "build_type", "arch"
    options         = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # what to package
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["flash"]
        self.cpp_info.requires = ["nlohmann_json::nlohmann_json", "fmt::fmt"]

    def validate(self):
        check_min_cppstd(self, "20")
    
    def requirements(self):
        self.requires("fmt/10.2.1")
        self.requires("nlohmann_json/3.11.2")

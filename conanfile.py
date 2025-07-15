from conan import ConanFile

class ExpresscppConan(ConanFile):
    name = "flash"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    requires = [
        "fmt/10.2.1",
        "nlohmann_json/3.11.2",
        "gtest/1.14.0",
        "cpr/1.10.5"
    ]
    default_options = {
        "gtest/*:shared": False,
        "fmt/*:shared": False,
        "cpr/*:shared": False
    }

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build" 
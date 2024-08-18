from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class ContrAllConan(ConanFile):
    name = "contrAll"
    version = "1.0.0"
    url = "https://github.com/mmacz/contrAll"
    license = "L-GPL 2.1"
    description = "CLI tool for USB game controllers calibration/verification"
    settings = "os", "arch", "build_type", "compiler"
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("ftxui/5.0.0")
        self.requires("libusb/1.0.26")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()


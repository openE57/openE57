import shutil,os
from conans import ConanFile, CMake, tools

class TestOpenE57Conan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "cmake_find_package"

    def build(self):
        if not tools.cross_building(self):
            cmake = CMake(self)
            cmake.configure()
            cmake.build()

    def test(self):
        if not tools.cross_building(self):
            self.run("openE57_example", run_environment=True)

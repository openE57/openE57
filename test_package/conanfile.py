import shutil,os
from conans import ConanFile, CMake, tools

class TestOpenE57Conan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "cmake_find_package"
    exports_sources = "foo.e57"

    def build(self):
        if not tools.cross_building(self):
            cmake = CMake(self)
            cmake.configure()
            cmake.build()

    def test(self):
        if not tools.cross_building(self):
            bin_path = os.path.join("bin", "openE57_example")
            self.run(bin_path, run_environment=True)

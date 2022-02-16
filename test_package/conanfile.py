import shutil,os
from conan.tools.microsoft import msvc_runtime_flag
from conans import ConanFile, CMake, tools

class TestOpene57Conan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "cmake_find_package"

    def build(self):
        if not tools.cross_building(self):
            cmake = CMake(self)
            cmake.configure()
            cmake.build()

    def test(self):
        if not tools.cross_building(self):
            bin_path = os.path.join("bin", "opene57_example")
            self.run(bin_path, run_environment=True)

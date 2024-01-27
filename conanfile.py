import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy, export_conandata_patches, get, replace_in_file
from conan.tools.microsoft import is_msvc, is_msvc_static_runtime
from conan.tools.scm import Version

required_conan_version = ">=1.54.0"


class Opene57Conan(ConanFile):
    name = "opene57"
    version = "1.7.0"
    description = "A C++ library for reading and writing E57 files, " \
                  "fork of the original libE57 (http://libe57.org)"
    topics = ("e57", "libe57", "3d", "astm")
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://github.com/openE57/openE57"
    license = ("MIT", "BSL-1.0")
    exports_sources = [ "src/*", "LICENSE*", "CHANGELOG.md", "*.txt"]
    settings = "os", "compiler", "arch", "build_type"
    options = { "with_tools": [True, False],
                "with_tests": [True, False],
                "with_examples": [True, False],
                "with_docs":  [True, False],
                "shared": [True, False],
                "fPIC": [True, False]
               }
    default_options = {
                "with_tools": False,
                "with_tests": False,
                "with_examples": False,
                "with_docs":  False,
                "shared": False,
                "fPIC": True
               }

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    @property
    def _minimum_compilers_version(self):
        return {
            "Visual Studio": "15",
            "msvc": "191",
            "gcc": "7",
            "clang": "6",
            "apple-clang": "10",
        }

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

        if self.options.with_tools:
            self.options['boost'].multithreading = True

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def validate(self):
        if self.options.shared:
            raise ConanInvalidConfiguration("OpenE57 cannot be built as shared library yet")
            
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 17)

        minimum_version = self._minimum_compilers_version.get(str(self.settings.compiler), False)
        if not minimum_version:
            self.output.warn("C++17 support required. Your compiler is unknown. Assuming it supports C++17.")
        elif Version(self.settings.compiler.version) < minimum_version:
            raise ConanInvalidConfiguration("C++17 support required, which your compiler does not support.")
        
    def requirements(self):
        if self.options.with_tests:
            self.requires("doctest/2.4.9")

        if self.options.with_tools:
            self.requires("boost/1.84.0")

        if self.options.with_docs:
            self.requires("doxygen/1.9.4")

        if self.settings.os != "Windows":
            self.requires("icu/74.1")

        self.requires("xerces-c/3.2.4")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["PROJECT_VERSION"] = self.version
        tc.variables["BUILD_EXAMPLES"] = self.options.with_examples
        tc.variables["BUILD_TOOLS"] = self.options.with_tools
        tc.variables["BUILD_TESTS"] = self.options.with_tests
        tc.variables["BUILD_DOCS"] = self.options.with_docs

        if is_msvc(self):
            tc.variables["BUILD_WITH_MT"] = is_msvc_static_runtime(self)

        tc.variables["CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS"] = self.options.shared
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        copy(self, "LICENSE.libE57", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        cmake = CMake(self)
        cmake.install()
        os.remove(os.path.join(self.package_folder, "CHANGELOG.md"))

    def package_info(self):
        if self.options.with_tools:
            bin_path = os.path.join(self.package_folder, "bin")
            self.output.info("Appending PATH env: {}".format(bin_path))
            self.env_info.PATH.append(bin_path)

        lib_suffix = "-d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = [f"openE57{lib_suffix}", f"openE57las{lib_suffix}"]

        self.cpp_info.defines.append(f"E57_REFIMPL_REVISION_ID={self.name}-{self.version}")
        self.cpp_info.defines.append("XERCES_STATIC_LIBRARY")
        self.cpp_info.defines.append("CRCPP_INCLUDE_ESOTERIC_CRC_DEFINITIONS")
        self.cpp_info.defines.append("CRCPP_USE_CPP11")

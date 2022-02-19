from conans import ConanFile, tools, CMake
from conan.tools.microsoft import msvc_runtime_flag
from conans.errors import ConanInvalidConfiguration
import os

class Opene57Conan(ConanFile):
    name = "opene57"
    version = "1.6.3"
    description = "A C++ library for reading and writing E57 files, " \
                  "fork of the original libE57 (http://libe57.org)"
    topics = ("e57", "libe57", "3d", "astm")
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://github.com/openE57/openE57"
    license = ("MIT", "BSL-1.0")
    exports_sources = [ "src/*", "LICENSE*", "CHANGELOG.md", "*.txt"]
    settings = "os", "compiler", "arch", "build_type"
    options = { "with_tools": [True, False],
                "with_docs":  [True, False],
                "shared": [True, False],
                "fPIC": [True, False]
               }
    default_options = {
                "with_tools": False,
                "with_docs":  False,
                "shared": False,
                "fPIC": True
               }
    generators = "cmake", "cmake_find_package"
    _cmake = None

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
            "gcc": "7",
            "clang": "6",
            "apple-clang": "10",
        }

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

        self.options['xerces-c'].shared = self.options.shared

        if self.settings.os == "Linux" or tools.is_apple_os(self.settings.os):
            self.options['icu'].shared = self.options.shared

        if self.options.with_tools:
            self.options['boost'].shared = self.options.shared
            self.options['boost'].multithreading = True

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def validate(self):
        if self.options.shared:
            raise ConanInvalidConfiguration("OpenE57 cannot be built as shared library yet")
            
        if self.settings.compiler.get_safe("cppstd"):
            tools.check_min_cppstd(self, 17)

        minimum_version = self._minimum_compilers_version.get(str(self.settings.compiler), False)
        if not minimum_version:
            self.output.warn("C++17 support required. Your compiler is unknown. Assuming it supports C++17.")
        elif tools.Version(self.settings.compiler.version) < minimum_version:
            raise ConanInvalidConfiguration("C++17 support required, which your compiler does not support.")
        
    def build_requirements(self):
        if self.options.with_tools:
            self.build_requires("boost/1.78.0")

        if self.options.with_docs:
            self.build_requires("doxygen/1.9.2")

    def requirements(self):
        if self.settings.os == "Linux" or tools.is_apple_os(self.settings.os):
            self.requires("icu/70.1")

        self.requires("xerces-c/3.2.3")

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.definitions["PROJECT_VERSION"] = self.version
        self._cmake.definitions["BUILD_EXAMPLES"] = False
        self._cmake.definitions["BUILD_TOOLS"] = self.options.with_tools
        self._cmake.definitions["BUILD_TESTS"] = False
        if self.settings.os == "Windows":
            self._cmake.definitions["BUILD_WITH_MT"] = "MT" in str(msvc_runtime_flag(self))
        else:
            self._cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = self.options.get_safe("fPIC", True)
        self._cmake.configure(build_folder=self._build_subfolder)
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        self.copy(pattern="LICENSE", dst="licenses", src=self._source_subfolder)
        self.copy(pattern="LICENSE.libE57", dst="licenses", src=self._source_subfolder)
        cmake = self._configure_cmake()
        cmake.install()
        os.remove(os.path.join(self.package_folder, "CHANGELOG.md"))
        tools.remove_files_by_mask(os.path.join(self.package_folder, "bin"), "*.dll")

    def package_info(self):
        if self.options.with_tools:
            bin_path = os.path.join(self.package_folder, "bin")
            self.output.info("Appending PATH env: {}".format(bin_path))
            self.env_info.PATH.append(bin_path)

        lib_suffix = "-d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = [f"openE57{lib_suffix}", f"openE57las{lib_suffix}"]

        self.cpp_info.defines.append(f"E57_REFIMPL_REVISION_ID={self.name}-{self.version}")
        self.cpp_info.defines.append("XERCES_STATIC_LIBRARY")

        self.cpp_info.set_property("cmake_target_name", "opene57")
        self.cpp_info.set_property("cmake_target_name", "opene57")
        # TODO: To remove in conan v2 once cmake_find_package* generators removed
        self.cpp_info.names["cmake_find_package"] = "opene57"
        self.cpp_info.names["cmake_find_package_multi"] = "opene57"

# openE57

![openE57](https://github.com/madduci/openE57/workflows/openE57/badge.svg)

openE57 is a forked version of the original LibE57 (http://www.libe57.org) project, with the intent to refine and optimize the source code in a modern C++ idiomatic way and remove unnecessary dependencies (e.g. Boost) in favour of the C++ Standard Library.

The library is compiled as C++17, since some of following language intrinsics and libraries are used:

* constexpr values
* enum classes
* random (replaces boost::uuid)
* filesystem (replaces boost::filesystem)
* thread (replaces boost::thread)
* memory (replaces boost::shared_ptr and std::auto_ptr)

At the current state, Boost is still required when building the tools as in the original source code, but it will be completely removed with the release 2.0.0.

## Requirements

You need the following tools to build this library:

* A C++17 compiler (MSVC 2017+, gcc 8+, clang 8+)
* A recent version of CMake (3.15+)
* A recent version of conan (2.0+)

## Build Instructions

There are two ways to build and use openE57: building it as a conan package or as a standard CMake project.
The dependencies are now managed with conan and integrated in CMake, without the need of compiling the required libraries by yourself.

### Building as a local conan package

The following instructions will guide you to build openE57 as a local conan package (actually it is being submitted as official conan recipe in the Conan Center), so it can be used it further in other projects.

Available conan options are the following ones:

* with_tools - disabled by default
* shared - disabled by default (not supported at the moment - no symbol is exported yet)
* fPIC - enabled by default, activates the `-fPIC` flag on gcc/clang compilers on Linux/Mac

Platform independent:

```shell
git clone https://github.com/madduci/openE57.git
cd open57
conan create . --build=missing
```

### Building as local project (e.g. for development)

The following instructions will guide you to build openE57 as a standard CMake Project, downloading the external dependencies with conan, but managing all the settings with CMake.

Available CMake Options are:

* BUILD_DOCS - builds the Doxygen documentation (requires Doxygen and Graphviz)
* BUILD_EXAMPLES - builds the API examples
* BUILD_TOOLS - builds the binary tools to validate and dump E57 files
* BUILD_TESTS - actually unsupported (no tests yet available)
* BUILD_SHARED_LIBS - actually unsupported (missing exported symbols)
* BUILD_WITH_MT - instructs CMake to set the correct [`CMAKE_MSVC_RUNTIME_LIBRARY`](https://cmake.org/cmake/help/latest/variable/CMAKE_MSVC_RUNTIME_LIBRARY.html?highlight=cmake_msvc_runtime_library) flag for Visual Studio

Building with Position indipendent code on Unix can be activated with the option [`CMAKE_POSITION_INDEPENDENT_CODE`](https://cmake.org/cmake/help/latest/variable/CMAKE_POSITION_INDEPENDENT_CODE.html?highlight=cmake_position_independent_code).


#### On Linux:

```shell
git clone https://github.com/madduci/openE57.git
cd open57
mkdir build && cd build
conan install .. --build=missing
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --target install
```

#### On Windows:

```cmd
git clone https://github.com/madduci/openE57.git
cd open57
md build && cd build
conan install .. --build=missing
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_WITH_MT=ON
cmake --build . --config Release --target install
```

#### Autoformatting

The Project offers the possibility to run the auto-formatter (clang-format) on the sources. As prerequisite, you need to install `clang-format` on your machine and then run the following cmake target:

```sh
cmake --build . --target clangformat
```

#### Building the documentation

It's also possible to build the documentation for the API (still largely outdated), using Doxygen and Graphviz. You'll have to install the required packages for your distribution in order to generate proper documentation files. 

To enerate the documentation, configure the project like:

```sh
cmake .. -DBUILD_DOCS=ON
cmake --build . --target doxygen
```
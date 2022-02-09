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

At the current state, Boost is still required as in the original source code, but it will be completely removed with the release 2.0.0.

## Requirements

You need the following tools to build this library:

* A C++17 compiler (MSVC 2017+, gcc 7+, clang 7+)
* A recent version of CMake (3.15+)
* A recent version of conan (1.28+)

## How to build it

On Linux:

```shell
git clone https://github.com/madduci/openE57.git
cd open57
conan create . --build=missing
```

On Windows:

```cmd
git clone https://github.com/madduci/openE57.git
cd open57
conan create . --build=missing
```

Available conan options (but disabled by default) are the following ones:

* with_examples
* with_tools
* with_tests
* mt (MSVC Only)
* shared (Not supported at the moment - no symbol is exported yet)

The dependencies are now managed with conan and integrated in CMake, without the need of compiling the required libraries by yourself.

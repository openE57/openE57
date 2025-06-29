# openE57

## [1.7.3] - 2025-06-22

## Changed
- Skip linking ICU explicitly, if Xerces-C is dynamically linked

## [1.7.2] - 2025-04-24

## Changed
- Modernization of codebase
- Reduction of MSVC compiler warnings

## [1.7.1] - 2025-02-28

## Changed
- Fixed build for mingw-w64
- CMake Build improvements

## [1.7.0] - 2024-01-27

## Changed
- Updated Boost Libraries to 1.84.0
- Updated CRC32-Castagnoli to 1.2.0.0
- Ported the Time Conversion library from C to C++

## [1.6.5] - 2024-01-24

## Changed
- Updated support for Conan 2.0
- Updated Boost Libraries to 1.83.0
- Updated ICU to 74.1

## [1.6.4] - 2022-11-28

## Changed
- Updated Boost Libraries to 1.80.0
- Updated Xerces-C to 3.2.4
- Updated ICU to 72.1
- Updated Doxygen to 1.9.4

## [1.6.3] - 2022-03-03

## Changed
- Moved documentation on root folder
- Improving build of documentation with CMake
- Added doxygen as requirements in conan if the build option is activated
- Updated the CRC32-Castagnoli implementation to newest version

## [1.6.2] - 2022-02-19

## Changed
- Re-enabled e57unpack as tool
- Improved Conan recipe, including tools
- Added formatting target with clang-format
- Minor improvements in CMake
- Minor improvements in CPack

## Removed
- Removed custom option to build with PIC on Unix, using CMAKE_POSITION_INDEPENDENT_CODE

## [1.6.1] - 2022-02-16

## Changed
- Fixed issues on Visual Studio by setting incorrectly CMAKE_MSVC_RUNTIME_TYPE
- Improved compatibility with conan build process
- Added option to disable -fPIC on Linux (enabled by default)
- Updated README.md

## [1.6.0] - 2022-02-05

## Changed
- Added conan packaging support
- Refactored project structure
- Renamed LASReader to openE57las
- Moved time_conversion to tools folder
- Updated Boost to 1.78.0 (only used if building tools)
- Fixed WIN32 code path in OpenE57simple

## Removed
- config.h template (definition passed through the compiler)
- Removed the dependency of time_conversion from openE57
- Removed the generation of time_conversion as artifact of openE57 (used only by tools)
- Disabled building of tool "e57unpack" due to issues with Boost

## [1.5.1] - 2021-12-09

## Changed
- Updated Boost to 1.77.0
- Updated ICU to 70.1

## [1.5.0] - 2021-01-18

## Changed
- Removed the dependency of Boost in the core library, keeping it only in tools and examples.
- Converted boost smart pointers with STL ones
- Converted boost standard integers to STL ones
- Using std::numeric_limits max/lower instead of hardcoded magic values
- Switched CRC32-Castagnoli implementation from boost::crc_optimal to Daniel Bahr's one (https://github.com/d-bahr/CRCpp)
- Made Boost required only for tools and examples, switched to the newer conan cmake_find_package resolution method with conan

## Removed
- macos from CI pipeline, since Boost library from conan fails to build


## [1.4.1] - 2021-01-15

### Changed
- Updated dependencies: Boost 1.75, Xerces 3.2.3, ICU 68.2
  

## [1.4.0] - 2020-07-16
### Added
- `clang-format` as CMake target

### Changed
- Renamed references to openE57
  

## [1.3.0] - 2020-07-02
### Changed
- Reduced compiler warnings
- Minor code optimizations


## [1.2.0] - 2020-06-26
### Added
- New CMake options such as
  - BUILD_EXAMPLES
  - BUILD_TOOLS
  - BUILD_WITH_MT
- Dependency management with conan.io
- Support for Apple compiler
- LICENSE, CONTRIBUTORS files

### Changed
- Project structure with separated options for tools and examples
- Uniform code formatting with .clang-format
- Update to latest version of dependencies (Boost, Xerces, ICU)

------------

# libE57 Reference Implementation

## [1.1.350] - 2013-04-03
### Changed
- e57unpack now also can handle 2d images


## [1.1.332] - 2011-03-14
### Added
- Missing library identifier string to cmake build
- E57RefImplConfig.cmake file
- e57validate tool
- e57unpack tool

### Changed
- Update to use boost version 1.46.1
- Streamlined cmake files for better static linking
    

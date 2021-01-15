# openE57

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
    

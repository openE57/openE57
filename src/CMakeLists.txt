#
# Define Settings and targets
#

# Set a private module find path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/compiler_options.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_format.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)

set(CONFIG_PACKAGE_INSTALL_DIR lib/cmake/${PROJECT_NAME})

#
# Build submodules
#
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(${BUILD_EXAMPLES})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

if(${BUILD_TESTS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if(${BUILD_TOOLS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools)
endif()

#
# Add Clang Format
#
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_format.cmake)

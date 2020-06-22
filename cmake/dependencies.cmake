find_package(Threads REQUIRED)

# Find the Boost and Xerces libraries
  # Find the Boost and Xerces libraries
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
    set(Boost_USE_MULTITHREADED ON)
    find_package(Boost 1.73.0 COMPONENTS program_options system thread filesystem
                                         QUIET)
    if(NOT Boost_FOUND)
      set(BOOST_ROOT CACHE PATH "Location of the boost root directory")
      message(FATAL_ERROR "Unable to find boost library.
      Please set the BOOST_ROOT to point to the boost distribution files.")
    endif(NOT Boost_FOUND)
  
    set(Xerces_USE_STATIC_LIBS On)
    find_package(Xerces 3.2.2 QUIET)
    if(NOT Xerces_FOUND)
      set(XERCES_ROOT CACHE PATH "Location of the xerces library")
      message(
        FATAL_ERROR
          "Unable to find xerces library.
      Please set the the XERCES_ROOT to point to the root of the xerces directory."
      )
    endif(NOT Xerces_FOUND)
  
    set(XML_LIBRARIES ${Xerces_LIBRARY})
    set(XML_INCLUDE_DIRS ${Xerces_INCLUDE_DIR})

    list(APPEND compiler_definitions
      BOOST_ALL_NO_LIB
      XERCES_STATIC_LIBRARY
      $<$<CONFIG:RELWITHDEBINFO>:${Boost_LIB_DIAGNOSTIC_DEFINITIONS}>
      $<$<CONFIG:DEBUG>:${Boost_LIB_DIAGNOSTIC_DEFINITIONS}>)

else()

    set(Boost_INCLUDE_DIR ${CONAN_INCLUDE_DIRS_BOOST})
    set(Boost_LIBRARY_DIRS CONAN_PKG::boost)
    set(XML_LIBRARIES CONAN_PKG::xerces-c)
    set(XML_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS_XERCES-C})

endif()
  
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  list(APPEND compiler_definitions LINUX)
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    find_package(ICU 67.1 REQUIRED)
    set(XML_LIBRARIES ${XML_LIBRARIES} ${ICU_LIBRARIES})
    set(XML_INCLUDE_DIRS ${XML_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS})
  else()
    set(XML_LIBRARIES ${XML_LIBRARIES} CONAN_PKG::icu)
    set(XML_INCLUDE_DIRS ${XML_INCLUDE_DIRS} ${CONAN_INCLUDE_DIRS_ICU})
  endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
 list(APPEND compiler_definitions WINDOWS)
endif()

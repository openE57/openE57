# Find the Xerces libraries
set(Xerces_USE_STATIC_LIBS ON)
find_package(XercesC 3.2 REQUIRED)
set(XML_LIBRARIES ${XercesC_LIBRARIES})
set(XML_INCLUDE_DIRS ${XercesC_INCLUDE_DIRS})
list(APPEND compiler_definitions XERCES_STATIC_LIBRARY)
  
# Add ICU in Linux Systems
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Apple")
  list(APPEND compiler_definitions LINUX)
  find_package(ICU REQUIRED)
  set(XML_LIBRARIES ${XML_LIBRARIES} ${ICU_LIBRARIES})
  set(XML_INCLUDE_DIRS ${XML_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  list(APPEND compiler_definitions WINDOWS)
endif()

# Find Boost (Required by Tools and Examples)
if(BUILD_TOOLS OR BUILD_EXAMPLES)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME ON)
  set(Boost_USE_MULTITHREADED ON)
  find_package(Boost 1.70.0 COMPONENTS program_options system thread filesystem REQUIRED)
  list(APPEND compiler_definitions
      BOOST_ALL_NO_LIB
      $<$<CONFIG:RELWITHDEBINFO>:${Boost_LIB_DIAGNOSTIC_DEFINITIONS}>
      $<$<CONFIG:DEBUG>:${Boost_LIB_DIAGNOSTIC_DEFINITIONS}>)
endif()
include(FetchContent)

function(is_dynamic_lib lib_path result)
  set(${result} FALSE PARENT_SCOPE)
  get_filename_component(lib_ext "${lib_path}" EXT)
  if(lib_ext STREQUAL ".dylib" OR lib_ext STREQUAL ".so" OR lib_path MATCHES "\\.so\\.[0-9]+" OR lib_ext STREQUAL ".dll")
    set(${result} TRUE PARENT_SCOPE)
  endif()
endfunction()

function(all_dynamic_libs libs result)
  set(all_dynamic TRUE)
  foreach(lib IN LISTS ${libs})
    is_dynamic_lib(${lib} is_dynamic)
    if(NOT is_dynamic)
      set(all_dynamic FALSE)
    endif()
  endforeach ()
  set(${result} ${all_dynamic} PARENT_SCOPE)
endfunction()

# Find the Xerces libraries
set(Xerces_USE_STATIC_LIBS ON)
find_package(XercesC 3.2 QUIET)
if (XercesC_FOUND)
  set(XML_LIBRARIES ${XercesC_LIBRARIES})
  set(XML_INCLUDE_DIRS ${XercesC_INCLUDE_DIRS})
  # Add ICU in Linux Systems
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Apple")
    all_dynamic_libs(XercesC_LIBRARIES dyn_XercesC)
    if (${dyn_XercesC})
      message("Linking dynlib xercesc: ${XercesC_LIBRARIES}, no need for linking ICU")
    else()
      find_package(ICU REQUIRED)
      set(XML_LIBRARIES ${XML_LIBRARIES} ${ICU_LIBRARIES})
      set(XML_INCLUDE_DIRS ${XML_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS})
    endif ()
  endif()
else()
  if (NOT TARGET xerces-c)
    set(XERCES_C_REPO https://github.com/apache/xerces-c.git)
    FetchContent_Declare(
      xerces-c
      EXCLUDE_FROM_ALL
      GIT_REPOSITORY ${XERCES_C_REPO}
      GIT_TAG ${XERCES_C_DEFAULT_FETCH_TAG}
      GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(xerces-c)
  endif()
  set(XML_LIBRARIES "xerces-c")
  get_target_property(XercesC_INCLUDE_DIRS xerces-c INCLUDE_DIRECTORIES)
  set(XML_INCLUDE_DIRS ${XercesC_INCLUDE_DIRS})
endif()

if(NOT BUILD_SHARED_LIBS)
  list(APPEND compiler_definitions XERCES_STATIC_LIBRARY)
endif()
  
# Find Boost (Required by Tools)
if(BUILD_TOOLS)
  if(NOT BUILD_SHARED_LIBS) 
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
  endif()
  set(Boost_USE_MULTITHREADED $<IF:$<AND:$<BOOL:${MSVC}>,$<NOT:$<BOOL:${BUILD_WITH_MT}>>>,OFF,ON>)
  find_package(Boost 1.70.0 REQUIRED)
  list(APPEND compiler_definitions
      BOOST_ALL_NO_LIB
      $<$<CONFIG:RELWITHDEBINFO>:${Boost_LIB_DIAGNOSTIC_DEFINITIONS}>
      $<$<CONFIG:DEBUG>:${Boost_LIB_DIAGNOSTIC_DEFINITIONS}>)
endif()

# Find doctest (Required by Tests)
if(BUILD_TESTS)
  find_package(doctest REQUIRED)
  enable_testing()
endif()

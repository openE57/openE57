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

include(${CMAKE_CURRENT_LIST_DIR}/xml_backend.cmake)
  
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

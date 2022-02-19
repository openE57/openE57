#
# Time conversion utilities
#
add_library(time_conversion ${LIBRARY_TYPE}
  ${CMAKE_CURRENT_SOURCE_DIR}/time_conversion/time_conversion.c
  ${CMAKE_CURRENT_SOURCE_DIR}/time_conversion/time_conversion.h
  ${CMAKE_CURRENT_SOURCE_DIR}/time_conversion/basictypes.h
  ${CMAKE_CURRENT_SOURCE_DIR}/time_conversion/constants.h
  ${CMAKE_CURRENT_SOURCE_DIR}/time_conversion/gnss_error.h)

set_target_properties(time_conversion PROPERTIES 
  DEBUG_POSTFIX "-d" 
  MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}")
target_compile_options(time_conversion PUBLIC ${compiler_options})
target_compile_definitions(time_conversion PUBLIC ${compiler_definitions})
target_link_options(time_conversion PUBLIC ${linker_flags})

target_include_directories(time_conversion 
    PRIVATE 
      ${CMAKE_CURRENT_SOURCE_DIR}/time_conversion
)

target_link_libraries(time_conversion 
  PUBLIC 
    ${Boost_LIBRARY_DIRS}
    ${XML_LIBRARIES}
    $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:dl>
    $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:m>
    $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:c>
)
target_clangformat_setup(time_conversion)

#
# Build executables
#
list(APPEND TOOLS
  e57fields
  e57unpack
  e57validate
  e57xmldump
)

foreach(TOOL ${TOOLS})
  add_executable(${TOOL} ${CMAKE_CURRENT_SOURCE_DIR}/${TOOL}.cpp)
  
  set_target_properties(${TOOL} PROPERTIES 
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}")
  target_compile_options(${TOOL} PUBLIC ${compiler_options})
  target_compile_definitions(${TOOL} PUBLIC ${compiler_definitions})
  target_link_options(${TOOL} PUBLIC ${linker_flags})
  target_include_directories(${TOOL} 
    PRIVATE 
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include
      ${XML_INCLUDE_DIRS}
      ${Boost_INCLUDE_DIR}
  )

  target_link_libraries(${TOOL}
    PRIVATE 
      ${PROJECT_NAME} 
      ${XML_LIBRARIES}
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
  )
  target_clangformat_setup(${TOOL})
endforeach()

#
# Handle las2e57 differently
#
add_executable(las2e57 ${CMAKE_CURRENT_SOURCE_DIR}/las2e57.cpp)

set_target_properties(las2e57 PROPERTIES 
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX} 
  MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}")
target_compile_options(las2e57 PUBLIC ${compiler_options})
target_compile_definitions(las2e57 PUBLIC ${compiler_definitions})
target_link_options(las2e57 PUBLIC ${linker_flags})

target_include_directories(las2e57
  PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include
    ${XML_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
)

target_link_libraries(las2e57 
    PRIVATE 
      ${PROJECT_NAME}
      ${PROJECT_NAME}las 
      time_conversion 
      ${XML_LIBRARIES}
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
)

target_clangformat_setup(las2e57)

install(
  TARGETS 
    ${TOOLS}
    las2e57
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
)
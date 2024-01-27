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
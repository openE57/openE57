list(APPEND EXAMPLES
  DemoWrite01
  DemoRead01
)

foreach(EXAMPLE ${EXAMPLES})
  add_executable(${EXAMPLE} ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE}.cpp)
  set_target_properties(${EXAMPLE}  PROPERTIES 
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX} 
    MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}")
  target_compile_options(${EXAMPLE}  PUBLIC ${compiler_options})
  target_compile_definitions(${EXAMPLE}  PUBLIC ${compiler_definitions})
  target_link_options(${EXAMPLE}  PUBLIC ${linker_flags})

  target_include_directories(${EXAMPLE} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include 
    ${XML_INCLUDE_DIRS} )
  target_link_libraries(${EXAMPLE} 
    PRIVATE 
      ${PROJECT_NAME} 
      ${XML_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
  )
  target_clangformat_setup(${EXAMPLE})
endforeach()
list(APPEND compiler_definitions
    E57_REFIMPL_REVISION_ID=${PROJECT_NAME}-${PROJECT_VERSION})

#
# The main library 
#
add_library(
  ${PROJECT_NAME} ${LIBRARY_TYPE}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openE57Simple.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openE57SimpleImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openE57.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openE57Impl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/openE57/impl/openE57Impl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/openE57/impl/openE57SimpleImpl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/openE57/openE57.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/openE57/openE57Simple.h)

generate_export_header(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES 
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}")
target_compile_options(${PROJECT_NAME} PUBLIC ${compiler_options})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${compiler_definitions})
target_link_options(${PROJECT_NAME} PUBLIC ${linker_flags})

target_include_directories(${PROJECT_NAME}
    PRIVATE 
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${XML_INCLUDE_DIRS} 
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${XML_LIBRARIES})
target_clangformat_setup(${PROJECT_NAME})

#
# Extension Library for LAS I/O support
#
add_library(${PROJECT_NAME}las ${LIBRARY_TYPE} 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}las.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/openE57/LAS/${PROJECT_NAME}las.h)

set_target_properties(${PROJECT_NAME}las PROPERTIES 
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}")
target_compile_options(${PROJECT_NAME}las PUBLIC ${compiler_options})
target_compile_definitions(${PROJECT_NAME}las PUBLIC ${compiler_definitions})
target_link_options(${PROJECT_NAME}las PUBLIC ${linker_flags})

target_include_directories(${PROJECT_NAME}las
    PRIVATE 
      $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${XML_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}las PUBLIC ${XML_LIBRARIES})
target_clangformat_setup(${PROJECT_NAME}las)

# 
# Install Artifacts
#
install(
  TARGETS 
    ${PROJECT_NAME}
    ${PROJECT_NAME}las
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib)

install(
  FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/openE57/openE57.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/openE57/openE57Simple.h 
    ${CMAKE_CURRENT_BINARY_DIR}/opene57_export.h
  DESTINATION 
    include/openE57)
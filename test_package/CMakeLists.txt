cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(opene57_example CXX)

option(BUILD_WITH_MT "Build with MultiThreaded Runtime" FALSE)

if(WIN32)
    set(MSVC_RUNTIME_TYPE $<IF:$<BOOL:${BUILD_WITH_MT}>,MultiThreaded$<$<CONFIG:Debug>:Debug>,MultiThreaded$<$<CONFIG:Debug>:Debug>>DLL)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(opene57 REQUIRED)

add_executable(${PROJECT_NAME} example.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${opene57_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${opene57_INCLUDE_DIRS})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

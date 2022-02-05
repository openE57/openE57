cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(openE57_example CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(openE57 REQUIRED)

add_executable(${PROJECT_NAME} 
    example.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} openE57::openE57)
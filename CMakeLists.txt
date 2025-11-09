cmake_minimum_required(VERSION 3.5...4.0)

project(ctu LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(ctu INTERFACE)
target_include_directories(ctu INTERFACE include/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ctu INTERFACE CTU_DEBUG)
endif()

# Enable testing
add_subdirectory(tests)

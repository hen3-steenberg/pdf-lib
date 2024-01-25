cmake_minimum_required(VERSION 3.28)

project(pdf-lib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

file(GLOB_RECURSE pdf_lib_src FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(pdf_lib STATIC ${pdf_lib_src})
target_include_directories(pdf_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(examples)

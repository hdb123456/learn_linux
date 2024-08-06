cmake_minimum_required(VERSION 3.5)

project(io_demo1)

add_compile_options(-std=c++11 -Wall)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")


add_executable(io_demo1 ${SOURCES})
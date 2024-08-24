cmake_minimum_required(VERSION 3.5)

project(cloud-tcp_v2)

add_compile_options(-std=c++11 -g -Wall)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES1 "src/tcp_server.cpp" "src/mysocket.cpp")
file(GLOB SOURCES2 "src/tcp_client.cpp" "src/client_base.cpp")

add_executable(server ${SOURCES1})
add_executable(client ${SOURCES2})

# 清理规则
add_custom_target(myclean
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMENT "Cleaning build directory"
    VERBATIM
)

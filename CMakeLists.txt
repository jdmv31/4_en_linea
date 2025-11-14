cmake_minimum_required(VERSION 3.10)

project(4enLinea)

find_package(raylib CONFIG REQUIRED)

add_executable(4enLinea main.cpp)

target_link_libraries(4enLinea PRIVATE raylib)
cmake_minimum_required(VERSION 3.10)

project(4enLinea)

find_package(raylib CONFIG REQUIRED)

add_library(LogicaFicheros_lib STATIC datos/LogicaFicheros.cpp)
add_library(LogicaTablero_lib STATIC negocios/LogicaTablero.cpp)

target_include_directories(LogicaFicheros_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/datos
    LogicaTablero_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/presentacion
)

# josue: cada .h nuevo que se agregue debe ser registrado aca para que compile el proyecto

add_executable(4enLinea main.cpp)

target_link_libraries(4enLinea 
    PRIVATE 
        raylib
        LogicaFicheros_lib
        LogicaTablero_lib
)
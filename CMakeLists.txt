cmake_minimum_required(VERSION 3.10)

project(4enLinea)

find_package(raylib CONFIG REQUIRED)

add_library(LogicaFicheros_lib STATIC datos/LogicaFicheros.cpp)
add_library(LogicaTablero_lib STATIC negocios/LogicaTablero.cpp)
add_library(LogicaFicha_lib STATIC negocios/LogicaFicha.cpp)
add_library(Jugador_lib STATIC negocios/Jugador.cpp)
add_library(Cpu_lib STATIC negocios/Cpu.cpp)
add_library(Interfaz_lib STATIC presentacion/Interfaz.cpp)

target_include_directories(
    LogicaFicheros_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/datos

    LogicaTablero_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/negocios

    LogicaFicha_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/negocios

    Jugador_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/negocios

    Cpu_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/negocios

    Interfaz_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/presentacion
)

# josue: cada .h nuevo que se agregue debe ser registrado aca para que compile el proyecto

add_executable(4enLinea main.cpp)
target_link_libraries(Interfaz_lib PUBLIC raylib)


target_link_libraries(4enLinea 
    PRIVATE 
        raylib
        LogicaFicheros_lib
        LogicaTablero_lib
        LogicaFicha_lib
        Jugador_lib
        Cpu_lib
        Interfaz_lib
)
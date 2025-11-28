cmake_minimum_required(VERSION 3.10)

project(4enLinea)

find_package(raylib CONFIG REQUIRED)

add_library (CapaDatos STATIC
    datos/LogicaFicheros.cpp
)

add_library(CapaNegocios STATIC 
    negocios/LogicaTablero.cpp
    negocios/LogicaFicha.cpp
    negocios/Jugador.cpp
    negocios/Cpu.cpp
)

add_library(CapaInterfaz STATIC 
    presentacion/Interfaz.cpp
)

target_include_directories(CapaNegocios PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CapaInterfaz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CapaDatos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CapaInterfaz PUBLIC CapaNegocios raylib)


add_executable(4enLinea main.cpp recursos.rc)

target_link_libraries(4enLinea PRIVATE CapaInterfaz)
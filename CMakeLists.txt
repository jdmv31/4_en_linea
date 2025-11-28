cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(4enLinea)

find_package(raylib CONFIG REQUIRED)

add_library (CapaDatos STATIC
    datos/LogicaFicheros.cpp
)

target_compile_definitions(CapaDatos PRIVATE 
    RUTA_DATOS="${CMAKE_CURRENT_SOURCE_DIR}/datos"
)

add_library(CapaNegocios STATIC 
    negocios/LogicaTablero.cpp
    negocios/LogicaFicha.cpp
    negocios/Jugador.cpp
    negocios/Cpu.cpp
)

add_library(CapaInterfaz STATIC 
    presentacion/Interfaz.cpp
    presentacion/Boton.cpp
)


target_include_directories(CapaNegocios PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CapaInterfaz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CapaDatos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CapaNegocios PUBLIC CapaDatos)

target_link_libraries(CapaInterfaz PUBLIC CapaNegocios raylib)


add_executable(4enLinea main.cpp recursos.rc)

target_link_libraries(4enLinea PRIVATE CapaInterfaz CapaDatos)
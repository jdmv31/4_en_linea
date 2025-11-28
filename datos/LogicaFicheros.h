#pragma once
#include "negocios/LogicaTablero.h"
#include <string>

#ifndef RUTA_DATOS
#define RUTA_DATOS "./"
#endif

// josue: esa cosa viene directamente desde el archivo cmakelists y es necesario para que las partidas se guarden
// en la carpeta partidasGuardadas, de lo contrario se guardara en un archivo especial del build de cmake

class GestorFicheros{
    private:
        int contadorPartidas;
        std::string nombrePartida;
    public:
        GestorFicheros();
        int getContador ();
        void guardarTablero(Tablero &tablero, int contador, std::string nombre);
        void leerTablero (Tablero &tablero,std::string nombre);
        void registrarPartida();
        int obtenerPartidas();
};
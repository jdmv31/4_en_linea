#pragma once
#include "negocios/LogicaTablero.h"
#include <string>
#include <vector>

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
        GestorFicheros(void);
        int getContador (void);
        void setContador (int cont);
        void guardarTablero(Tablero &tablero, int contador, std::string nombre, bool turno, int modoJuego, int modalidad,int vic1, int vic2);
        void leerTablero (Tablero &tablero,std::string nombre,bool &turno,int &modoJuego, int &modalidad, int &vic1, int &vic2);
        void registrarPartida(void);
        int obtenerPartidas(void);
        std::vector<std::string> obtenerPartidasGuardadas();
};
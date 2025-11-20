#pragma once
#include "logicaFicha.h"

const int filas = 6;
const int columnas = 7;

class Tablero{
    private:
        int tablero [filas] [columnas];
    public:
        Tablero (void);
        void insertarFicha(int pY,Ficha ficha);
        bool hayFicha(int pX, int pY);
        bool estadoTablero(int param);
        bool comprobarVictoria(void);
};
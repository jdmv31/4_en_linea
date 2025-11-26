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
        int getValor (int pX,int pY);
        void deshacerJugada(int c);
        bool hayFicha(int pX, int pY);
        bool estadoTablero(int param);
        int comprobarVictoria(void);
        bool columnaLibre(int c);
        void limpiarTablero(void);
};
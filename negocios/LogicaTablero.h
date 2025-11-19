#pragma once

const int filas = 6;
const int columnas = 7;

class Tablero{
    private:
        int tablero [filas] [columnas];
    public:
        Tablero (void);
        bool hayFicha(int pX, int pY);
        bool estadoTablero(int param);
        bool comprobarVictoria(void);
};
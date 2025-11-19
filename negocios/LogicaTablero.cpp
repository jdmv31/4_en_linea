#include "LogicaTablero.h"

Tablero::Tablero(void){
    for (int i = 0; i < filas; i++){
        for (int j = 0; j < columnas;j++){
            tablero[i][j] = 0;
        }
    }
}

bool Tablero::hayFicha(int pX, int pY){
    return (tablero[pX][pY] != 0);
}

bool Tablero::estadoTablero(int param){
    // josue: la variable param va a determinar si se desea comprobar si el tablero esta vacio o lleno
    // 0 - vacio, 1 - lleno

    for (int i = 0; i < filas;i++){
        for (int j = 0; j < columnas;j++){
            if (param == 0 && tablero[i][j] != 0)
                return false;
            else if (param == 1 && tablero[i][j] == 0)
                return false;
        }
    }
    return true;
}
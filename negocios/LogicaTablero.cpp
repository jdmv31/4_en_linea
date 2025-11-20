#include "LogicaTablero.h"
#include "LogicaFicha.h"

Tablero::Tablero(void){
    for (int i = 0; i < filas; i++){
        for (int j = 0; j < columnas;j++){
            tablero[i][j] = 0;
        }
    }
}

void Tablero::insertarFicha(int pY, Ficha ficha){
    int posicion = filas;
    for (int i = 0; i < filas; i++){
        if (i == 0 && hayFicha (0,pY))
            break;
            // aca deberia ir un mensaje de error indicando que esa columna ya esta llena

        if (hayFicha(i,pY)){
            posicion = i - 1;
            tablero[posicion][pY] = ficha.getColor();
        }
    }

    if (posicion == filas)
        tablero[filas][pY] = ficha.getColor();
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
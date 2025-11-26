#include "LogicaTablero.h"
#include "LogicaFicha.h"

Tablero::Tablero(void){
    for (int i = 0; i < filas; i++){
        for (int j = 0; j < columnas;j++){
            tablero[i][j] = 0;
        }
    }
}

int Tablero::getValor (int pX,int pY){
    return tablero[pX][pY];
}

bool Tablero::columnaLibre(int c){
    return tablero[0][c] == 0;
}

void Tablero::deshacerJugada(int c) {
    for (int i = 0; i < filas; i++) {
        if (tablero[i][c] != 0) {
            tablero[i][c] = 0;
            break;
        }
    }
}

void Tablero::insertarFicha(int pY, Ficha ficha) {
    // Verificamos si la columna está llena antes de nada
    if (hayFicha(0, pY)) {
        return; // O lanzar error
    }

    bool colocada = false;

    // Buscamos de arriba (0) a abajo (filas)
    for (int i = 0; i < filas; i++) {
        if (hayFicha(i, pY)) {
            // Encontramos una ficha en 'i', así que colocamos la nuestra encima (i-1)
            tablero[i - 1][pY] = ficha.getColor();
            colocada = true;
            break; // <--- ESTE BREAK ES EL QUE FALTABA
        }
    }

    // Si llegamos aquí y no se ha colocado, significa que la columna estaba vacía
    // o no encontramos obstáculos, así que va al fondo.
    if (!colocada) {
        tablero[filas - 1][pY] = ficha.getColor();
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

// En LogicaTablero.cpp

// Devuelve: 0 (Nadie gana), 1 (Gana Jugador 1), 2 (Gana CPU)
int Tablero::comprobarVictoria(void) {
    // 1 = Humano, 2 = CPU
    for (int jugador = 1; jugador <= 2; jugador++) {
        // Horizontal
        for (int r = 0; r < filas; r++) {
            for (int c = 0; c < columnas - 3; c++) {
                if (tablero[r][c] == jugador && tablero[r][c+1] == jugador && 
                    tablero[r][c+2] == jugador && tablero[r][c+3] == jugador)
                    return jugador;
            }
        }
        // Vertical
        for (int r = 0; r < filas - 3; r++) {
            for (int c = 0; c < columnas; c++) {
                if (tablero[r][c] == jugador && tablero[r+1][c] == jugador && 
                    tablero[r+2][c] == jugador && tablero[r+3][c] == jugador)
                    return jugador;
            }
        }
        // Diagonal /
        for (int r = 0; r < filas - 3; r++) {
            for (int c = 0; c < columnas - 3; c++) {
                if (tablero[r][c] == jugador && tablero[r+1][c+1] == jugador && 
                    tablero[r+2][c+2] == jugador && tablero[r+3][c+3] == jugador)
                    return jugador;
            }
        }
        // Diagonal \ (Corrección: c debe empezar en 3 para no salir del array)
        for (int r = 0; r < filas - 3; r++) {
            for (int c = 3; c < columnas; c++) {
                if (tablero[r][c] == jugador && tablero[r+1][c-1] == jugador && 
                    tablero[r+2][c-2] == jugador && tablero[r+3][c-3] == jugador)
                    return jugador;
            }
        }
    }
    return 0; // Nadie ha ganado aun
}
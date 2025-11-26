#include "Cpu.h"
#include "LogicaFicha.h"
#include "LogicaTablero.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


Cpu::Cpu(string n, int v, Ficha f) : Jugador(n, v, f) {} // constructor de clase hija hacia clase padre

int Cpu::minimax(Tablero& tablero, int profundidad, bool esMaximizando, int alpha, int beta) {
    if (profundidad == 0 || esNodoTerminal(tablero)) {
        return evaluarTablero(tablero);
    }

    if (esMaximizando) {
        int maxEval = -100000000;
        for (int col = 0; col < 7; col++) {
            if (tablero.columnaLibre(col)) {
                tablero.insertarFicha(col, this->ficha); // Juega CPU
                int eval = minimax(tablero, profundidad - 1, false, alpha, beta);
                tablero.deshacerJugada(col);
                
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break; // Poda
            }
        }
        return maxEval;
    } else { 
        int minEval = 100000000;
        Ficha fichaHumano; 
        fichaHumano.setColor(this->ficha.getColor() == 2);

        for (int col = 0; col < 7; col++) {
            if (tablero.columnaLibre(col)) {
                tablero.insertarFicha(col, fichaHumano);
                int eval = minimax(tablero, profundidad - 1, true, alpha, beta);
                tablero.deshacerJugada(col);
                
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break; // Poda
            }
        }
        return minEval;
    }
}

int Cpu::evaluarTablero(Tablero& tablero) {
    int puntaje = 0;
    
    // Analizar horizontal, vertical y diagonales buscando patrones
    // Recorremos el tablero creando "ventanas" de 4 espacios
    
    // Horizontal
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 4; c++) {
            vector<int> ventana;
            for (int k = 0; k < 4; k++) ventana.push_back(tablero.getValor(r, c+k));
            puntaje += evaluarVentana(ventana);
        }
    }
    // Vertical
    for (int c = 0; c < 7; c++) {
        for (int r = 0; r < 3; r++) {
            vector<int> ventana;
            for (int k = 0; k < 4; k++) ventana.push_back(tablero.getValor(r+k, c));
            puntaje += evaluarVentana(ventana);
        }
    }
    // Diagonal positiva /
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 4; c++) {
            vector<int> ventana;
            for (int k = 0; k < 4; k++) ventana.push_back(tablero.getValor(r+k, c+k));
            puntaje += evaluarVentana(ventana);
        }
    }
    // Diagonal negativa \ 
    for (int r = 0; r < 3; r++) {
        for (int c = 3; c < 7; c++) {
            vector<int> ventana;
            for (int k = 0; k < 4; k++) ventana.push_back(tablero.getValor(r+k, c-k));
            puntaje += evaluarVentana(ventana);
        }
    }
    return puntaje;
}

int Cpu::evaluarVentana(vector<int> ventana) {
    // ... (tu código de declaración de variables igual) ...
    int puntaje = 0;
    int miColor = this->ficha.getColor(); 
    int opColor = (miColor == 1) ? 2 : 1; 

    // Contadores (igual que antes)
    int misFichas = 0, opFichas = 0, vacios = 0;
    for (int val : ventana) {
        if (val == miColor) misFichas++;
        else if (val == opColor) opFichas++;
        else vacios++;
    }

    // --- CORRECCIÓN DE PRIORIDADES ---
    
    // 1. GANAR (Yo tengo 4)
    if (misFichas == 4) return 10000; 

    // 2. PERDER (El rival tiene 4) -> ¡ESTO FALTABA!
    // Si no pones esto, a la IA le da igual que le ganes.
    else if (opFichas == 4) return -10000; 

    // 3. BLOQUEO DE EMERGENCIA (Rival tiene 3 y hueco)
    else if (opFichas == 3 && vacios == 1) return -5000; 

    // 4. Armar jugada ofensiva
    else if (misFichas == 3 && vacios == 1) return 100;
    
    // 5. Desarrollo
    else if (misFichas == 2 && vacios == 2) return 5;

    return 0;
}

// Verifica si el juego acabó para cortar la recursión
bool Cpu::esNodoTerminal(Tablero& tablero) {
    // Debe devolver true si el tablero está lleno O si alguien ya ganó (1 o 2)
    // Asumo que tu clase Tablero tiene comprobarVictoria(), si no, úsalo del código anterior.
    return tablero.estadoTablero(1) || tablero.comprobarVictoria() != 0;
}

int Cpu::determinarJugada(Tablero& tablero) {
    int mejorPuntaje = -100000000;
    int mejorColumna = -1;
    
    vector<int> prioridadColumnas = {3, 2, 4, 1, 5, 0, 6};
    // va a priorizar las columnas que estan al centro del tablero dejando de lado los q estan a los bordes6

    for (int col : prioridadColumnas) {
        if (tablero.columnaLibre(col)) {
            Ficha fichaTemp = this->ficha;
            tablero.insertarFicha(col, fichaTemp); 

            int puntaje = minimax(tablero, PROFUNDIDAD, false, -100000000, 100000000);

            tablero.deshacerJugada(col);

            if (puntaje > mejorPuntaje) {
                mejorPuntaje = puntaje;
                mejorColumna = col;
            }
        }
    }
    // Si no encuentra nada (raro), devuelve una válida cualquiera
    if (mejorColumna == -1) {
        for(int i=0; i<7; i++) if(tablero.columnaLibre(i)) return i;
    }
    return mejorColumna;
}
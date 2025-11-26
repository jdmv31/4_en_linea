#pragma once
#include "Jugador.h"
#include "LogicaFicha.h"
#include "LogicaTablero.h"
#include <string>
#include <vector>

class Cpu : public Jugador{
    // josue: se aplica herencia desde la clase jugador para usar sus metodos y atributos y ademas colocar los algoritmos de la IA
    private:
        const int PROFUNDIDAD = 4; // josue: esto basicamente significa el grado de inteligencia de la Cpu
        int minimax(Tablero& tablero, int profundidad, bool esMaximizando, int alpha, int beta);
        int evaluarTablero(Tablero& tablero);
        int evaluarVentana(std::vector<int> ventana);
        bool esNodoTerminal(Tablero& tablero);

    public:
        Cpu (std::string, int,Ficha);
        int determinarJugada(Tablero &tablero);
        int calcularFichasConsecutivas();
};
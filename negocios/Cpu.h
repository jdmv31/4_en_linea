#pragma once
#include "Jugador.h"
#include "LogicaFicha.h"
#include "LogicaTablero.h"
#include <string>

class Cpu : public Jugador{
// josue: se aplica herencia desde la clase jugador para usar sus metodos y atributos y ademas colocar los algoritmos de la IA
    public:
        Cpu (std::string, int,Ficha);
    private:
        void deteminarJugada(Tablero);
        int calcularFichasConsecutivas();
};
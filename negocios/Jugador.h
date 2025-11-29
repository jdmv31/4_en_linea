#pragma once
#include <string>
#include "logicaFicha.h"

class Jugador{
    protected:
        std::string nombreJugador;
        int victoriasJugador;
        Ficha ficha;
    public:
        Jugador(std::string,int,Ficha);
        void incrementarVictorias(void);
        int getVictorias(void);
        void setNombre(std::string);
        void setVictorias (int num);
        std::string getNombre(void);
        // el nombre servira para las impresiones en la interfaz sobre a quien le corresponde jugar
};
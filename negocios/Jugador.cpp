#include "Jugador.h"
#include <string>
#include "logicaFicha.h"

Jugador::Jugador(std::string n, int v,Ficha f){
    nombreJugador = n;
    victoriasJugador = v;
    ficha = f;
}

void Jugador::incrementarVictorias(void){
    victoriasJugador += 1;
}

int Jugador::getVictorias(void){
    return victoriasJugador;
}

void Jugador::setNombre(std::string n){
    nombreJugador = n;
}
#include "Jugador.h"
#include <string>
#include "logicaFicha.h"
#include "LogicaTablero.h"
#include <iostream>

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

std::string Jugador::getNombre(void){
    return nombreJugador;
}
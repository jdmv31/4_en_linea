#include "LogicaFicha.h"

Ficha::Ficha(void){
    Ficha::colorFicha(ROJO);
}

void Ficha::setColor(bool turno){
    if (turno)
        fichaJugada = ROJO;
    else
        fichaJugada = MORADO; 
}

int Ficha::getColor (){
    if (fichaJugada == ROJO)
        return 1;
    else
        return 2;
}
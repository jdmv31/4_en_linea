#pragma once

class Ficha{
    public:
        enum colorFicha {ROJO, AMARILLO};
    private:
        colorFicha fichaJugada;
    public:
        Ficha(void);
        void setColor (bool turno);
        // josue: con este metodo se modificara el color de la ficha a tirar, por cada turno que suceda
        // se le asignara un valor a las fichas rojas (jugador) y a las fichas moradas (cpu) otro
        // 1 - jugador 2 - cpu
        int getColor(void);
        // 1 = ficha roja
        // 2 = ficha morada
};
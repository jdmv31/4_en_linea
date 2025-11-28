#pragma once
#include "negocios/LogicaTablero.h"

typedef enum { 
    MENU_PRINCIPAL, 
    MENU_SELECCION_MODO,  
    MENU_VARIANTE, 
    JUEGO_EN_MARCHA,      
    VENTANA_CONTINUAR    
}Pantalla;

class Interfaz{
    private:
        Tablero matrizVisual;
    public:
        void mainloop(void);
        void dibujarMatriz(Tablero &tablero,int mX, int mY);
        bool mostrarError(const char* titulo, const char* mensaje);
};
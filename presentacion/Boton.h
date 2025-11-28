#pragma once
#include <raylib.h>


class Boton{
    private:
        Rectangle rect;
        Color colorBase;
        Color colorHover; 
        const char* texto;
        int tamanoTexto;
    public:
        Boton(float x, float y, float width, float height, const char* text, Color color);
        void Dibujar();
        bool FueClickeado();
};
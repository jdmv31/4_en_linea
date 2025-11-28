#include "Boton.h"
#include <raylib.h>

Boton::Boton(float x, float y, float width, float height, const char* text, Color color) {
    rect = { x, y, width, height };
    texto = text;
    colorBase = color;
    colorHover = Fade(color, 0.8f); 
    tamanoTexto = 20;
}

void Boton::Dibujar(void) {
    Vector2 mouse = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mouse, rect);
    DrawRectangleRec(rect, isHover ? colorHover : colorBase);
    DrawRectangleLinesEx(rect, 2, DARKGRAY);

    int anchoTexto = MeasureText(texto, tamanoTexto);
    int posX = (int)(rect.x + (rect.width - anchoTexto) / 2);
    int posY = (int)(rect.y + (rect.height - tamanoTexto) / 2);

    DrawText(texto, posX, posY, tamanoTexto, BLACK);
}

bool Boton::FueClickeado(void) {
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, rect)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    return false;
}
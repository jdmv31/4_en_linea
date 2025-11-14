#include "raylib.h"

const int anchoPantalla = 800;
const int altoPantalla = 450;

int main(){
    InitWindow(anchoPantalla,altoPantalla, "4 en linea");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("HOLA TONOTOS", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
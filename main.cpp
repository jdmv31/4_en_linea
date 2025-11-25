#include <raylib.h>
#include <iostream>
#include "negocios/LogicaTablero.h"
#include "datos/LogicaFicheros.h"
#include "presentacion/Interfaz.h"

using namespace std;

const int anchoPantalla = 800;
const int altoPantalla = 450;

int main(){
    InitWindow(anchoPantalla,altoPantalla, "4 en lineaa");

    while (!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("HOLA TONOTOS TONTOS", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
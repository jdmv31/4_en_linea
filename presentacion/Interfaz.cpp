#include "Interfaz.h"
#include <raylib.h>

void mainloop(void){
    const int anchoPantalla = 800;
    const int altoPantalla = 450;
    InitWindow(anchoPantalla,altoPantalla, "4 en lineaa");

    while (!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("tontos", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
}




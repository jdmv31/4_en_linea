#include "Interfaz.h"
#include <raylib.h>
#include "negocios/LogicaTablero.h"
#include "negocios/LogicaFicha.h"
#include "negocios/Jugador.h"

#include <iostream>
using namespace std;
// eso se borra luego


#define TAMANO_CELDA 80
#define RADIO_FICHA 35


Tablero matrizVisual;

class Boton {
private:
    Rectangle rect;
    Color colorBase;
    Color colorHover; 
    const char* texto;
    int tamanoTexto;

public:
    Boton(float x, float y, float width, float height, const char* text, Color color) {
        rect = { x, y, width, height };
        texto = text;
        colorBase = color;
        colorHover = Fade(color, 0.8f); 
        tamanoTexto = 20;
    }

    void Dibujar() {
        Vector2 mouse = GetMousePosition();
        bool isHover = CheckCollisionPointRec(mouse, rect);
        DrawRectangleRec(rect, isHover ? colorHover : colorBase);
        DrawRectangleLinesEx(rect, 2, DARKGRAY);

        int anchoTexto = MeasureText(texto, tamanoTexto);
        int posX = (int)(rect.x + (rect.width - anchoTexto) / 2);
        int posY = (int)(rect.y + (rect.height - tamanoTexto) / 2);

        DrawText(texto, posX, posY, tamanoTexto, BLACK);
    }

    bool FueClickeado() {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, rect)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                return true;
            }
        }
        return false;
    }
};

typedef enum { 
    MENU_PRINCIPAL, 
    MENU_SELECCION_MODO,  
    JUEGO_EN_MARCHA,      
    VENTANA_CONTINUAR    
} Pantalla;


void dibujarMatriz(Tablero &tablero, int mX, int mY) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            int cx = mX + (j * TAMANO_CELDA) + (TAMANO_CELDA / 2);
            int cy = mY + (i * TAMANO_CELDA) + (TAMANO_CELDA / 2);
            
            int val = matrizVisual.getValor(i,j);     
            if (val == 0) 
                DrawCircle(cx, cy, RADIO_FICHA, BLACK); 
            else if (val == 1)
                DrawCircle(cx, cy, RADIO_FICHA, RED);
            else
                DrawCircle(cx, cy, RADIO_FICHA, PURPLE);   
        }
    }
}

void mainloop(void) { 
    const int anchoPantalla = 1280; 
    const int altoPantalla = 720;
    
    InitWindow(anchoPantalla, altoPantalla, "4 en Linea");
    SetTargetFPS(60); 

    Color colorMorado = GetColor(0x4525A2FF);

    Pantalla pantallaActual = MENU_PRINCIPAL;
    int modoJuego = 0; // 1=HvH, 2=RvR, 3=RvH
    bool turno = true; // true - ficha roja false - ficha morada
    
    Ficha ficha1;
    ficha1.setColor(true);
    Ficha ficha2;
    ficha2.setColor(false);
    Jugador j1 = Jugador("Josue",0,ficha1);
    Jugador j2 = Jugador("Nicole",0,ficha2);

    // Cálculos para centrar el tablero
    int anchoTableroPx = columnas * TAMANO_CELDA;
    int altoTableroPx = filas * TAMANO_CELDA;
    int margenX = (anchoPantalla - anchoTableroPx) / 2;
    int margenY = (altoPantalla - altoTableroPx) / 2 + 50;

    // --- INSTANCIACIÓN DE OBJETOS (BOTONES) ---
    Boton btnIniciar(300, 300, 350, 80, "INICIAR PARTIDA", LIGHTGRAY);
    Boton btnContinuar(680, 300, 350, 80, "CONTINUAR", LIGHTGRAY);

    // Selección de Modo
    Boton btnHvH(490, 200, 300, 60, "JUGADOR VS JUGADOR", SKYBLUE);
    Boton btnRvR(490, 300, 300, 60, "CPU VS CPU", ORANGE);
    Boton btnRvH(490, 400, 300, 60, "JUGADOR VS CPU", LIME);

    // Común
    Boton btnVolver(50, 50, 150, 50, "VOLVER", RED);
    Boton btnSalir(50, 50, 150, 50, "SALIR", RED);

    while (!WindowShouldClose()){
        
        Vector2 mousePoint = GetMousePosition();
        bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        switch(pantallaActual) {
            
            case MENU_PRINCIPAL:
                if (btnIniciar.FueClickeado()) {
                    pantallaActual = MENU_SELECCION_MODO;
                }
                if (btnContinuar.FueClickeado()) {
                    pantallaActual = VENTANA_CONTINUAR;
                }
                break;

            case MENU_SELECCION_MODO:
                if (btnHvH.FueClickeado() || btnRvR.FueClickeado() || btnRvH.FueClickeado()) 
                {
                    if (btnHvH.FueClickeado()) modoJuego = 1;
                    else if (btnRvR.FueClickeado()) modoJuego = 2;
                    else if (btnRvH.FueClickeado()) modoJuego = 3;

                    matrizVisual.limpiarTablero();
                    pantallaActual = JUEGO_EN_MARCHA;
                }
                
                if (btnVolver.FueClickeado()) {
                    pantallaActual = MENU_PRINCIPAL;
                }
                break;

            case JUEGO_EN_MARCHA:
                if (click) {
                    if (mousePoint.x >= margenX && mousePoint.x < margenX + anchoTableroPx &&
                        mousePoint.y >= margenY && mousePoint.y < margenY + altoTableroPx) {
                        
                        int col = (int)(mousePoint.x - margenX) / TAMANO_CELDA;
                        
                        if (col >= 0 && col < columnas) {
                            if (modoJuego == 1){
                                if (turno)
                                    matrizVisual.insertarFicha(col,ficha1);
                                else
                                    matrizVisual.insertarFicha(col,ficha2);

                                turno = !turno;
                            }
                        }
                    }
                }

                if (btnSalir.FueClickeado()) {
                    pantallaActual = MENU_PRINCIPAL;
                    modoJuego = 0;
                }
                break;

            case VENTANA_CONTINUAR:
                if (btnVolver.FueClickeado()) {
                    pantallaActual = MENU_PRINCIPAL;
                }
                break;
        }

        // ============================
        //     DIBUJADO (DRAW)
        // ============================
        BeginDrawing();
        ClearBackground(colorMorado);

        switch(pantallaActual) {
            case MENU_PRINCIPAL:
                DrawText("4 EN LINEA - MENU", 480, 100, 40, WHITE);
                btnIniciar.Dibujar();
                btnContinuar.Dibujar();
                break;

            case MENU_SELECCION_MODO:
                DrawText("SELECCIONA MODO DE JUEGO", 380, 100, 40, WHITE);
                btnHvH.Dibujar();
                btnRvR.Dibujar();
                btnRvH.Dibujar();
                btnVolver.Dibujar();
                break;

            case JUEGO_EN_MARCHA:
                ClearBackground(BLACK); 

                // Tablero Fondo
                DrawRectangle(margenX - 10, margenY - 10, anchoTableroPx + 20, altoTableroPx + 20, BLUE);
                
                // Círculos y Fichas
                dibujarMatriz(matrizVisual, margenX, margenY);
                
                if (modoJuego == 1) DrawText("MODO: HUMANO vs HUMANO", 50, 50, 20, WHITE);
                if (modoJuego == 2) DrawText("MODO: ROBOT vs ROBOT", 50, 50, 20, WHITE);
                if (modoJuego == 3) DrawText("MODO: ROBOT vs HUMANO", 50, 50, 20, WHITE);

                btnSalir.Dibujar();
                break;

            case VENTANA_CONTINUAR:
                DrawText("CONTINUAR PARTIDA", 400, 300, 30, LIGHTGRAY);
                btnVolver.Dibujar();
                break;
        }

        int estado = matrizVisual.comprobarVictoria();
        if (estado == 1){
            cout<<"Ganador: "<<j1.getNombre();
            matrizVisual.limpiarTablero();
        }
        else if (estado == 2){
            cout<<"Ganador: "<<j2.getNombre();
            matrizVisual.limpiarTablero();
        }

        EndDrawing();
    }

    CloseWindow();
}
#include "Interfaz.h"
#include <raylib.h>



// 1. CONSTANTES DEL TABLERO
#define FILAS 6
#define COLUMNAS 7
#define TAMANO_CELDA 80
#define RADIO_FICHA 35

// --- CLASE BOTÓN (POO) ---
// Encapsulamos la lógica visual y de interacción para no repetir código
class Boton {
private:
    Rectangle rect;
    Color colorBase;
    Color colorHover; // Color cuando pasas el mouse por encima
    const char* texto;
    int tamanoTexto;

public:
    // Constructor
    Boton(float x, float y, float width, float height, const char* text, Color color) {
        rect = { x, y, width, height };
        texto = text;
        colorBase = color;
        // Creamos un color un poco más claro para el efecto hover
        colorHover = Fade(color, 0.8f); 
        tamanoTexto = 20;
    }

    // Método para dibujar el botón (debe llamarse dentro de BeginDrawing)
    void Dibujar() {
        Vector2 mouse = GetMousePosition();
        bool isHover = CheckCollisionPointRec(mouse, rect);

        // Dibujar base (cambia de color si el mouse está encima)
        DrawRectangleRec(rect, isHover ? colorHover : colorBase);
        
        // Dibujar borde decorativo
        DrawRectangleLinesEx(rect, 2, DARKGRAY);

        // Calcular posición para centrar el texto
        int anchoTexto = MeasureText(texto, tamanoTexto);
        int posX = (int)(rect.x + (rect.width - anchoTexto) / 2);
        int posY = (int)(rect.y + (rect.height - tamanoTexto) / 2);

        DrawText(texto, posX, posY, tamanoTexto, BLACK);
    }

    // Método lógico: devuelve true si se hizo clic en el botón
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

void mainloop(void) { 
    
    // --- CONFIGURACIÓN VENTANA ---
    const int anchoPantalla = 1280; 
    const int altoPantalla = 720;
    
    InitWindow(anchoPantalla, altoPantalla, "4 en Linea - Integrado con POO");
    SetTargetFPS(60); 

    Color colorMorado = GetColor(0x4525A2FF);

    Pantalla pantallaActual = MENU_PRINCIPAL;
    int modoJuego = 0; // 1=HvH, 2=RvR, 3=RvH

    // --- VARIABLES DEL JUEGO ---
    int matrizVisual[FILAS][COLUMNAS] = {0};

    // Cálculos para centrar el tablero
    int anchoTableroPx = COLUMNAS * TAMANO_CELDA;
    int altoTableroPx = FILAS * TAMANO_CELDA;
    int margenX = (anchoPantalla - anchoTableroPx) / 2;
    int margenY = (altoPantalla - altoTableroPx) / 2 + 50;

    // --- INSTANCIACIÓN DE OBJETOS (BOTONES) ---
    // Ahora creamos objetos de la clase Boton
    
    // Menu Principal
    Boton btnIniciar(300, 300, 350, 80, "INICIAR PARTIDA", LIGHTGRAY);
    Boton btnContinuar(680, 300, 350, 80, "CONTINUAR", LIGHTGRAY);

    // Selección de Modo
    Boton btnHvH(490, 200, 300, 60, "HUMANO vs HUMANO", SKYBLUE);
    Boton btnRvR(490, 300, 300, 60, "ROBOT vs ROBOT", ORANGE);
    Boton btnRvH(490, 400, 300, 60, "ROBOT vs HUMANO", LIME);

    // Común
    Boton btnVolver(50, 50, 150, 50, "VOLVER", RED);
    Boton btnSalir(50, 50, 150, 50, "SALIR", RED); // Mismo botón pero con otro texto para el juego

    while (!WindowShouldClose()){
        
        Vector2 mousePoint = GetMousePosition();
        bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        // ============================
        //     LÓGICA (UPDATE)
        // ============================
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
                // AL SELECCIONAR MODO
                if (btnHvH.FueClickeado() || btnRvR.FueClickeado() || btnRvH.FueClickeado()) 
                {
                    if (btnHvH.FueClickeado()) modoJuego = 1;
                    else if (btnRvR.FueClickeado()) modoJuego = 2;
                    else if (btnRvH.FueClickeado()) modoJuego = 3;

                    // Reiniciar matriz
                    for(int i=0; i<FILAS; i++) for(int j=0; j<COLUMNAS; j++) matrizVisual[i][j] = 0;
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
                        int fila = (int)(mousePoint.y - margenY) / TAMANO_CELDA;
                        matrizVisual[fila][col] = 1; 
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

                // Tablero
                DrawRectangle(margenX - 10, margenY - 10, anchoTableroPx + 20, altoTableroPx + 20, BLUE);
                for (int i = 0; i < FILAS; i++) {
                    for (int j = 0; j < COLUMNAS; j++) {
                        int cx = margenX + (j * TAMANO_CELDA) + (TAMANO_CELDA / 2);
                        int cy = margenY + (i * TAMANO_CELDA) + (TAMANO_CELDA / 2);
                        if (matrizVisual[i][j] == 0) DrawCircle(cx, cy, RADIO_FICHA, BLACK); 
                        else DrawCircle(cx, cy, RADIO_FICHA, RED);   
                    }
                }

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

        EndDrawing();
    }

    CloseWindow();
 
}
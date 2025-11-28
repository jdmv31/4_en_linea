#include "Interfaz.h"
#include <raylib.h>
#include "negocios/LogicaTablero.h"
#include "negocios/LogicaFicha.h"
#include "negocios/Jugador.h"
#include "negocios/Cpu.h"
#include <string>

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
    MENU_VARIANTE, 
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
                DrawCircle(cx, cy, RADIO_FICHA, YELLOW);   
        }
    }
}

void mainloop(void) { 
    const int anchoPantalla = 1280; 
    const int altoPantalla = 720;
    
    Image icon = LoadImage("presentacion/icono.ico");
        
    InitWindow(anchoPantalla, altoPantalla, "4 en Linea");
    SetTargetFPS(60); 

    Color colorMorado = GetColor(0x4525A2FF);
    Color colorTextoVictoria = YELLOW; 

    Pantalla pantallaActual = MENU_PRINCIPAL;
    
    int modoJuego = 0;
    int varianteJuego = 0;  // 1=Clásico (Primer 4), 2=Por Puntos (Tablero Lleno) <-- NUEVA VARIABLE
    bool turno = true; 

    std::string textoTurno = ""; 
    std::string textoVictoria = "";

    float tiempoEsperaCpu = 0.0f; 
    bool juegoTerminado = false;
    int ganadorID = 0;

    Ficha ficha1; ficha1.setColor(true);
    Ficha ficha2; ficha2.setColor(false);

    Jugador j1 = Jugador("Josue", 0, ficha1);
    Jugador j2 = Jugador("Nicole", 0, ficha2);
    Cpu bot1 ("Skynet", 0, ficha2);
    Cpu bot2 ("Eva", 0, ficha1); 

    int anchoTableroPx = columnas * TAMANO_CELDA;
    int altoTableroPx = filas * TAMANO_CELDA;
    int margenX = (anchoPantalla - anchoTableroPx) / 2;
    int margenY = (altoPantalla - altoTableroPx) / 2 + 50;

    //botones principales
    Boton btnIniciar(300, 300, 350, 80, "INICIAR PARTIDA", LIGHTGRAY);
    Boton btnContinuar(680, 300, 350, 80, "CONTINUAR", LIGHTGRAY);
    //botones de modo de juego
    Boton btnHvH(490, 200, 300, 60, "JUGADOR VS JUGADOR", SKYBLUE);
    Boton btnRvR(490, 300, 300, 60, "JUGADOR VS BOT", ORANGE);
    Boton btnRvH(490, 400, 300, 60, "BOT VS BOT", LIME);
    //botones de modalidad del juego
    Boton btnVarClasico(440, 250, 400, 60, "CLASICO (GANAR CON 4)", GOLD);
    Boton btnVarPuntos(440, 350, 400, 60, "POR PUNTOS (LLENAR TABLERO)", PINK);
    //botones comunes
    Boton btnVolver(50, 50, 150, 50, "VOLVER", RED);
    Boton btnSalir(50, 50, 150, 50, "SALIR", RED);
    Boton btnReiniciar(anchoPantalla / 2 - 100, altoPantalla / 2 + 50, 200, 60, "REINICIAR", GOLD);
    Boton btnGuardar(anchoPantalla - 250, 50, 230, 50, "GUARDAR PARTIDA", ORANGE);

    while (!WindowShouldClose()){
        
        Vector2 mousePoint = GetMousePosition();
        bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        switch(pantallaActual) {
            
            case MENU_PRINCIPAL:
                if (btnIniciar.FueClickeado()) pantallaActual = MENU_SELECCION_MODO;
                if (btnContinuar.FueClickeado()) pantallaActual = VENTANA_CONTINUAR;
                break;

            case MENU_SELECCION_MODO:
                if (btnHvH.FueClickeado() || btnRvR.FueClickeado() || btnRvH.FueClickeado()) 
                {
                    if (btnHvH.FueClickeado()) modoJuego = 1;
                    else if (btnRvR.FueClickeado()) modoJuego = 2; 
                    else if (btnRvH.FueClickeado()) modoJuego = 3; 

                    matrizVisual.limpiarTablero(); 
                    turno = true; 
                    juegoTerminado = false;
                    ganadorID = 0;
                    pantallaActual = MENU_VARIANTE;
                }
                if (btnVolver.FueClickeado()) pantallaActual = MENU_PRINCIPAL;
                break;

            case MENU_VARIANTE: 
                // modalidad del juego
                if (btnVarClasico.FueClickeado() || btnVarPuntos.FueClickeado()) {
                    
                    if (btnVarClasico.FueClickeado()) varianteJuego = 1;
                    else varianteJuego = 2;

                    // Reinicio de partida
                    matrizVisual.limpiarTablero(); 
                    turno = true; 
                    juegoTerminado = false;
                    ganadorID = 0;
                    tiempoEsperaCpu = 0;
                    
                    pantallaActual = JUEGO_EN_MARCHA;
                }

                if (btnVolver.FueClickeado()) pantallaActual = MENU_SELECCION_MODO; // Volver atrás
                break;

            case JUEGO_EN_MARCHA:
                if (!juegoTerminado) {
                    //LOGICA DEL BOTON GUARDAR yoshua
                     if (btnGuardar.FueClickeado()) {
                        // alo? backend aqui
                        // Ejemplo: gestorArchivos.guardarPartida(matrizVisual, turno, modoJuego, varianteJuego);
                        TraceLog(LOG_INFO, "Boton Guardar presionado"); // Mensaje en consola por ahora
                    }
                    switch (modoJuego){
                        case 1:
                            if (turno) 
                                textoTurno = "TURNO DE: " + j1.getNombre();
                            else       
                                textoTurno = "TURNO DE: " + j2.getNombre();
                        break;

                        case 2:
                            if (turno) textoTurno = "TURNO DE: " + j1.getNombre();
                            else       textoTurno = "TURNO DE: " + bot1.getNombre();
                        break;

                        case 3:
                            if (turno) textoTurno = "TURNO DE: " + bot2.getNombre();
                            else       textoTurno = "TURNO DE: " + bot1.getNombre();
                        break;
                    }

                    switch(modoJuego){
                        case 1:
                            if (click) {
                                if (mousePoint.x >= margenX && mousePoint.x < margenX + anchoTableroPx &&
                                    mousePoint.y >= margenY && mousePoint.y < margenY + altoTableroPx) {
                                    int col = (int)(mousePoint.x - margenX) / TAMANO_CELDA;
                                    if (col >= 0 && col < columnas) {
                                        if (turno) matrizVisual.insertarFicha(col, ficha1);
                                        else       matrizVisual.insertarFicha(col, ficha2);
                                        turno = !turno;
                                    }
                                }
                            }
                        break;

                        case 2:
                            if (turno) { 
                                if (click) {
                                    if (mousePoint.x >= margenX && mousePoint.x < margenX + anchoTableroPx &&
                                        mousePoint.y >= margenY && mousePoint.y < margenY + altoTableroPx) {
                                        int col = (int)(mousePoint.x - margenX) / TAMANO_CELDA;
                                        if (col >= 0 && col < columnas) {
                                            matrizVisual.insertarFicha(col, ficha1);
                                            turno = !turno; 
                                            tiempoEsperaCpu = 0;
                                        }
                                    }
                                }
                            } else {
                                tiempoEsperaCpu += GetFrameTime();
                                if (tiempoEsperaCpu > 0.5f) {
                                    int col = bot1.determinarJugada(matrizVisual);
                                    matrizVisual.insertarFicha(col, ficha2);
                                    turno = !turno;
                                }
                            }                    
                        break;

                        case 3:
                            tiempoEsperaCpu += GetFrameTime();
                            if (tiempoEsperaCpu > 0.5f) {
                                int col;
                                if (turno) {
                                    col = bot2.determinarJugada(matrizVisual);
                                    matrizVisual.insertarFicha(col, ficha1);
                                } else {
                                    col = bot1.determinarJugada(matrizVisual);
                                    matrizVisual.insertarFicha(col, ficha2);
                                }
                                turno = !turno;
                                tiempoEsperaCpu = 0;
                            }                    
                        break;
                    }
                     // Nota: Aquí deberás implementar tu lógica backend de "varianteJuego"
                     //varianteJuego es la modalidad del juego yoshua
                
                    int estado = matrizVisual.comprobarVictoria();
                    if (estado == 1 || estado == 2) {
                        juegoTerminado = true;
                        ganadorID = estado;
                        
                        if (estado == 1) colorTextoVictoria = RED;
                        else colorTextoVictoria = YELLOW;

                        if (modoJuego == 1) {
                            textoVictoria = (estado == 1) ? "GANADOR: " + j1.getNombre() : "GANADOR: " + j2.getNombre();
                        } else if (modoJuego == 2) {
                            textoVictoria = (estado == 1) ? "GANADOR: " + j1.getNombre() : "GANADOR: " + bot1.getNombre();
                        } else {
                            textoVictoria = (estado == 1) ? "GANADOR: " + bot2.getNombre() : "GANADOR: " + bot1.getNombre();
                        }
                    }
                } else {
                    if (btnReiniciar.FueClickeado()) {
                        matrizVisual.limpiarTablero();
                        turno = true;
                        juegoTerminado = false;
                        ganadorID = 0;
                        tiempoEsperaCpu = 0;
                    }
                    if (btnSalir.FueClickeado()) {
                        pantallaActual = MENU_PRINCIPAL;
                        modoJuego = 0;
                        juegoTerminado = false;
                    }
                }

                if (!juegoTerminado && btnSalir.FueClickeado()) {
                    pantallaActual = MENU_PRINCIPAL;
                    modoJuego = 0;
                }
                break;

            case VENTANA_CONTINUAR:
                if (btnVolver.FueClickeado()) pantallaActual = MENU_PRINCIPAL;
                break;
        }

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
                
            case MENU_VARIANTE: // nueva pantalla
                DrawText("SELECCIONA MODALIDAD DE JUEGO", 420, 150, 40, WHITE);
                btnVarClasico.Dibujar();
                btnVarPuntos.Dibujar();
                btnVolver.Dibujar();
                break;

            case JUEGO_EN_MARCHA:
                ClearBackground(colorMorado);
                DrawRectangle(margenX - 10, margenY - 10, anchoTableroPx + 20, altoTableroPx + 20, BLUE);
                
                dibujarMatriz(matrizVisual, margenX, margenY);

                
                if (modoJuego == 1) DrawText("MODO: HUMANO vs HUMANO", 500, 50, 20, WHITE);
                if (modoJuego == 2) DrawText("MODO: JUGADOR vs BOT", 500, 50, 20, WHITE);
                if (modoJuego == 3) DrawText("MODO: BOT vs BOT", 500, 50, 20, WHITE);

                // Texto de Variante/modalidad juego
                if (varianteJuego == 1) DrawText("(CLASICO)", 500, 75, 20, YELLOW);
                if (varianteJuego == 2) DrawText("(POR PUNTOS)", 500, 75, 20, PINK);

                if (!juegoTerminado) {
                    if (turno)
                        DrawText(textoTurno.c_str(), 500, 100, 20, RED);              
                    else 
                        DrawText(textoTurno.c_str(), 500, 100, 20, YELLOW);
                    
                    btnSalir.Dibujar();
                    btnGuardar.Dibujar();
                } else {
                    DrawRectangle(0, 0, anchoPantalla, altoPantalla, Fade(BLACK, 0.7f));
                    
                    int textWidth = MeasureText(textoVictoria.c_str(), 40);
                     DrawText(textoVictoria.c_str(), (anchoPantalla - textWidth) / 2, altoPantalla / 2 - 50, 40, colorTextoVictoria);
                    
                    btnReiniciar.Dibujar();
                    btnSalir.Dibujar();
                }
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
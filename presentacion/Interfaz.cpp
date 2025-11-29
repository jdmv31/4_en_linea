#include "Interfaz.h"
#include "negocios/LogicaTablero.h"
#include "negocios/LogicaFicha.h"
#include "negocios/Jugador.h"
#include "negocios/Cpu.h"
#include "datos/LogicaFicheros.h"
#include "Boton.h"
#include <raylib.h>
#include <vector>
#include <string>

#define TAMANO_CELDA 80
#define RADIO_FICHA 35

void Interfaz::dibujarMatriz(Tablero &tablero, int mX, int mY) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            int cx = mX + (j * TAMANO_CELDA) + (TAMANO_CELDA / 2);
            int cy = mY + (i * TAMANO_CELDA) + (TAMANO_CELDA / 2);
            
            int val = this->matrizVisual.getValor(i,j);     
            if (val == 0) 
                DrawCircle(cx, cy, RADIO_FICHA, BLACK); 
            else if (val == 1)
                DrawCircle(cx, cy, RADIO_FICHA, RED);
            else
                DrawCircle(cx, cy, RADIO_FICHA, YELLOW);   
        }
    }
}

bool Interfaz::mostrarError(const char* titulo, const char* mensaje) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    int ancho = 400;
    int alto = 200;
    int x = (GetScreenWidth() - ancho) / 2;
    int y = (GetScreenHeight() - alto) / 2;

    DrawRectangle(x, y, ancho, alto, RAYWHITE);
    DrawRectangleLinesEx({(float)x, (float)y, (float)ancho, (float)alto}, 3, RED);

    DrawText(titulo, x + 20, y + 20, 25, RED);
    DrawText(mensaje, x + 20, y + 80, 20, DARKGRAY);

    Boton btnCerrar((float)(x + 100), (float)(y + 130), 200, 50, "OK", LIGHTGRAY);
    btnCerrar.Dibujar();

    return btnCerrar.FueClickeado();
}

void Interfaz::mainloop(void) { 
    const int anchoPantalla = 1280; 
    const int altoPantalla = 720;
    
    Image icon = LoadImage("presentacion/icono.ico");
    GestorFicheros archivos;
        
    InitWindow(anchoPantalla, altoPantalla, "4 en Linea");
    SetTargetFPS(60); 

    Color colorMorado = GetColor(0x4525A2FF);
    Color colorTextoVictoria = YELLOW; 

    Pantalla pantallaActual = MENU_PRINCIPAL;

    int modoJuego = 0;
    int varianteJuego = 0; 
    bool turno = true; 

    std::string textoTurno = ""; 
    std::string textoVictoria = "";

    const char* mensaje1 = "";
    const char* mensaje2 = "";

    float tiempoEsperaCpu = 0.0f; 
    bool juegoTerminado = false;
    int ganadorID = 0;

    bool error = false;
    bool piezaColocada = false; 
    
    float scrollY = 0.0f;

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
    
    int contPartidas = archivos.obtenerPartidas();
    archivos.setContador(contPartidas);

    std::vector <std::string> lista = archivos.obtenerPartidasGuardadas();

    Boton btnIniciar(300, 300, 350, 80, "INICIAR PARTIDA", LIGHTGRAY);
    Boton btnContinuar(680, 300, 350, 80, "CONTINUAR", LIGHTGRAY);
    Boton btnHvH(490, 200, 300, 60, "JUGADOR VS JUGADOR", SKYBLUE);
    Boton btnRvR(490, 300, 300, 60, "JUGADOR VS BOT", ORANGE);
    Boton btnRvH(490, 400, 300, 60, "BOT VS BOT", LIME);
    Boton btnVarClasico(440, 250, 400, 60, "CLASICO (GANAR CON 4)", GOLD);
    Boton btnVarPuntos(440, 350, 400, 60, "POR PUNTOS (LLENAR TABLERO)", PINK);
    Boton btnVolver(50, 50, 150, 50, "VOLVER", RED);
    Boton btnSalir(50, 50, 150, 50, "SALIR", RED);
    Boton btnReiniciar((float)(anchoPantalla / 2 - 100), (float)(altoPantalla / 2 + 50), 200, 60, "REINICIAR", GOLD);
    Boton btnGuardar((float)(anchoPantalla - 250), 50, 230, 50, "GUARDAR PARTIDA", ORANGE); 

    while (!WindowShouldClose()){
        
        Vector2 mousePoint = GetMousePosition();
        bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        float rueda = GetMouseWheelMove();

        switch(pantallaActual) {
            
            case MENU_PRINCIPAL:
                if (!error) {
                    if (btnIniciar.FueClickeado()) 
                        pantallaActual = MENU_SELECCION_MODO;
                    
                    if (btnContinuar.FueClickeado()) {
                        lista = archivos.obtenerPartidasGuardadas();
                        
                        if (lista.empty()) {
                            error = true;
                            mensaje1 = "NO HAY PARTIDAS GUARDADAS";
                            mensaje2 = "Debe registrar al menos una partida para continuarla.";
                        } else {
                            pantallaActual = VENTANA_CONTINUAR;
                        }
                    }
                }
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
                    error = false;
                    pantallaActual = MENU_VARIANTE;
                }
                if (btnVolver.FueClickeado()) pantallaActual = MENU_PRINCIPAL;
                break;

            case MENU_VARIANTE: 
                if (btnVarClasico.FueClickeado() || btnVarPuntos.FueClickeado()) {
                    
                    if (btnVarClasico.FueClickeado()) varianteJuego = 1;
                    else varianteJuego = 2;

                    matrizVisual.limpiarTablero(); 
                    turno = true; 
                    juegoTerminado = false;
                    ganadorID = 0;
                    tiempoEsperaCpu = 0;
                    error = false;
                    
                    pantallaActual = JUEGO_EN_MARCHA;
                }

                if (btnVolver.FueClickeado()) pantallaActual = MENU_SELECCION_MODO; 
                break;

            case JUEGO_EN_MARCHA:
                if (!juegoTerminado && !error) {
                    if (btnGuardar.FueClickeado()) {
                        if (matrizVisual.tableroVacio()){
                            error = true;
                            mensaje1 = "ERROR DE GUARDADO";
                            mensaje2 = "No puedes guardar un tablero vacio.";
                        } 
                        else if (piezaColocada){
                            archivos.registrarPartida();
                            contPartidas += 1;
                            archivos.setContador(contPartidas);
                            archivos.guardarTablero(matrizVisual,contPartidas,"Partida ",turno,modoJuego,varianteJuego);

                            error = true;
                            mensaje1 = "PARTIDA GUARDADA";
                            mensaje2 = "Tu progreso ha sido almacenado correctamente.";
                            
                            pantallaActual = MENU_PRINCIPAL; 
                        }
                        else{
                            error = true;
                            mensaje1 = "SIN MOVIMIENTOS";
                            mensaje2 = "No puedes guardar un tablero duplicado.";
                        }
                    }  
                    switch (modoJuego){
                        case 1:
                            if (turno) 
                                textoTurno = "TURNO DE: " + j1.getNombre();
                            else       
                                textoTurno = "TURNO DE: " + j2.getNombre();
                        break;
                        case 2:
                            if (turno) 
                                textoTurno = "TURNO DE: " + j1.getNombre();
                            else       
                                textoTurno = "TURNO DE: " + bot1.getNombre();
                        break;
                        case 3:
                            if (turno) 
                                textoTurno = "TURNO DE: " + bot2.getNombre();
                            else       
                                textoTurno = "TURNO DE: " + bot1.getNombre();
                        break;
                    }

                    switch(modoJuego){
                        case 1:
                            if (click) {
                                if (mousePoint.x >= margenX && mousePoint.x < margenX + anchoTableroPx &&
                                    mousePoint.y >= margenY && mousePoint.y < margenY + altoTableroPx) {
                                    int col = (int)(mousePoint.x - margenX) / TAMANO_CELDA;
                                    if (col >= 0 && col < columnas) {
                                        if (turno) 
                                            matrizVisual.insertarFicha(col, ficha1);
                                        else       
                                            matrizVisual.insertarFicha(col, ficha2);
                                        turno = !turno;
                                        piezaColocada = true;
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
                                            piezaColocada = true;
                                        }
                                    }
                                }
                            } else {
                                tiempoEsperaCpu += GetFrameTime();
                                if (tiempoEsperaCpu > 0.5f) {
                                    int col = bot1.determinarJugada(matrizVisual);
                                    matrizVisual.insertarFicha(col, ficha2);
                                    turno = !turno;
                                    piezaColocada = true;
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
                                piezaColocada = true;
                            }                    
                        break;
                    }
                
                    int estado = matrizVisual.comprobarVictoria();
                    if (estado == 1 || estado == 2) {
                        juegoTerminado = true;
                        ganadorID = estado;
                        
                        if (estado == 1) 
                            colorTextoVictoria = RED;
                        else 
                            colorTextoVictoria = YELLOW;

                        if (modoJuego == 1) {
                            textoVictoria = (estado == 1) ? "GANADOR: " + j1.getNombre() : "GANADOR: " + j2.getNombre();
                        } else if (modoJuego == 2) {
                            textoVictoria = (estado == 1) ? "GANADOR: " + j1.getNombre() : "GANADOR: " + bot1.getNombre();
                        } else {
                            textoVictoria = (estado == 1) ? "GANADOR: " + bot2.getNombre() : "GANADOR: " + bot1.getNombre();
                        }
                    }
                } else {
                    if (juegoTerminado) {
                        if (btnReiniciar.FueClickeado()) {
                            matrizVisual.limpiarTablero();
                            turno = true;
                            juegoTerminado = false;
                            ganadorID = 0;
                            tiempoEsperaCpu = 0;
                            error = false;
                        }
                        if (btnSalir.FueClickeado()) {
                            pantallaActual = MENU_PRINCIPAL;
                            modoJuego = 0;
                            juegoTerminado = false;
                            error = false;
                        }
                    }
                }
                if (!juegoTerminado && !error && btnSalir.FueClickeado()) {
                    pantallaActual = MENU_PRINCIPAL;
                    modoJuego = 0;
                }
                break;

            case VENTANA_CONTINUAR:
                if (btnVolver.FueClickeado()) {
                    pantallaActual = MENU_PRINCIPAL;
                    scrollY = 0.0f;
                }
                
                float alturaContenido = lista.size() * 70.0f;
                float alturaVisible = 450.0f;

                if (alturaContenido > alturaVisible) {
                    scrollY -= rueda * 20.0f;
                    if (scrollY < 0) scrollY = 0;
                    if (scrollY > alturaContenido - alturaVisible) 
                        scrollY = alturaContenido - alturaVisible;
                } else {
                    scrollY = 0;
                }
                break;
        }

        BeginDrawing();
        ClearBackground(colorMorado);
        
        switch(pantallaActual) {
            case MENU_PRINCIPAL:
                DrawText("4 EN LINEA - MENU", 480, 100, 40, WHITE);
                btnIniciar.Dibujar();
                btnContinuar.Dibujar();
                
                if (error)
                    if (mostrarError(mensaje1, mensaje2))
                        error = false;
                break;

            case MENU_SELECCION_MODO:
                DrawText("SELECCIONA MODO DE JUEGO", 380, 100, 40, WHITE);
                btnHvH.Dibujar();
                btnRvR.Dibujar();
                btnRvH.Dibujar();
                btnVolver.Dibujar();
                break;
                
            case MENU_VARIANTE: 
                DrawText("SELECCIONA MODALIDAD DE JUEGO", 420, 150, 40, WHITE);
                btnVarClasico.Dibujar();
                btnVarPuntos.Dibujar();
                btnVolver.Dibujar();
                break;

            case JUEGO_EN_MARCHA:
                ClearBackground(colorMorado);
                DrawRectangle(margenX - 10, margenY - 10, anchoTableroPx + 20, altoTableroPx + 20, BLUE);
                
                dibujarMatriz(matrizVisual, margenX, margenY);

                if (modoJuego == 1) 
                    DrawText("MODO: HUMANO vs HUMANO", 500, 50, 20, WHITE);
                if (modoJuego == 2) 
                    DrawText("MODO: JUGADOR vs BOT", 500, 50, 20, WHITE);
                if (modoJuego == 3) 
                    DrawText("MODO: BOT vs BOT", 500, 50, 20, WHITE);

                if (varianteJuego == 1) 
                    DrawText("(CLASICO)", 500, 75, 20, YELLOW);
                if (varianteJuego == 2) 
                    DrawText("(POR PUNTOS)", 500, 75, 20, PINK);

                if (!juegoTerminado) {
                    if (turno)
                        DrawText(textoTurno.c_str(), 500, 100, 20, RED);              
                    else 
                        DrawText(textoTurno.c_str(), 500, 100, 20, YELLOW);
                    
                    btnSalir.Dibujar();
                    btnGuardar.Dibujar();
                    if (error)
                        if (mostrarError(mensaje1,mensaje2))
                            error = false;

                } else {
                    DrawRectangle(0, 0, anchoPantalla, altoPantalla, Fade(BLACK, 0.7f));
                    
                    int textWidth = MeasureText(textoVictoria.c_str(), 40);
                    DrawText(textoVictoria.c_str(), (anchoPantalla - textWidth) / 2, altoPantalla / 2 - 50, 40, colorTextoVictoria);
                    
                    btnReiniciar.Dibujar();
                    btnSalir.Dibujar();
                }
                break;

            case VENTANA_CONTINUAR:
                DrawText("SELECCIONA UNA PARTIDA", 400, 80, 30, WHITE);
                btnVolver.Dibujar();

                BeginScissorMode(0, 120, 1280, 500); 

                for (int i = 0; i < lista.size(); i++) {
                    float posY = 150.0f + (i * 70.0f) - scrollY; 
                    
                    if (posY > 100 && posY < 650) {
                        Boton btnPartida(440.0f, posY, 400.0f, 60.0f, lista[i].c_str(), SKYBLUE);
                        btnPartida.Dibujar();
                        
                        if (btnPartida.FueClickeado()){
                            std::string nombre = lista[i];
                            archivos.leerTablero(matrizVisual, nombre, turno, modoJuego, varianteJuego);
                            pantallaActual = JUEGO_EN_MARCHA;
                            break; 
                        }
                    }
                }

                EndScissorMode();

                if (lista.size() * 70.0f > 450.0f) {
                    DrawRectangle(1260, 120, 10, 500, Fade(BLACK, 0.3f));
                    float porcentaje = scrollY / (lista.size() * 70.0f - 450.0f);
                    DrawRectangle(1260, 120 + (int)(porcentaje * 450), 10, 50, WHITE); 
                }
            break;
        }

        EndDrawing();
    }
    
    CloseWindow();
}
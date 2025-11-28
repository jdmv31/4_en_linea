#include "LogicaFicheros.h"
#include <fstream>
#include <filesystem>
#include "negocios/LogicaTablero.h"

namespace fs = std::filesystem;


GestorFicheros::GestorFicheros(){
    contadorPartidas = 0;
    nombrePartida = "";
}

int GestorFicheros::getContador(void){
    return contadorPartidas;
}

// se debe pasar como parametro Partida en el nombre
void GestorFicheros::guardarTablero(Tablero &tablero, int contador, std::string nombre){
    nombre += std::to_string(contador);
    fs::path rutaBase(RUTA_DATOS); 
    /*
        el comando / es propio de la libreria filesystem, no es el signo de division que se usa normalmente
        este es para crear rutas relativas y poder guardar los datos en la carpeta partidasGuardadas
    */
    fs::path rutaCarpeta = rutaBase / "partidasGuardadas";
    
    if (!fs::exists(rutaCarpeta)) {
        fs::create_directories(rutaCarpeta);
    }

    fs::path rutaArchivo = rutaCarpeta / (nombre + ".bin");

    std::ofstream archivo(rutaArchivo, std::ios::out | std::ios::binary);

    if (archivo.is_open()){
        archivo.write(reinterpret_cast<char*>(&tablero), sizeof(Tablero));
        archivo.close();
    }
}

void GestorFicheros::leerTablero(Tablero &tablero,std::string nombre){
    nombre += ".bin";
    fs::path ruta = fs::path("partidasGuardadas") / nombre;

    std::ifstream archivo (ruta,std::ios::in | std::ios::binary);
    if (archivo.is_open()){
        archivo.read(reinterpret_cast<char*>(&tablero), sizeof(Tablero));
        archivo.close();
    }
}

void GestorFicheros::registrarPartida(void){
    std::ofstream archivo ("hola.txt");
    if (archivo.is_open()){
        archivo << "todos tontos";
        archivo.close();
    }
}
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

void GestorFicheros::setContador (int cont){
    contadorPartidas = cont;
}

// se debe pasar como parametro Partida en el nombre
void GestorFicheros::guardarTablero(Tablero &tablero, int contador, std::string nombre, bool turno, int modoJuego, int modalidad,int vic1, int vic2){
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
        archivo.write(reinterpret_cast<char*>(&turno),sizeof(bool));
        archivo.write(reinterpret_cast<char*>(&modoJuego),sizeof(int));
        archivo.write(reinterpret_cast<char*>(&modalidad),sizeof(int));
        archivo.write(reinterpret_cast<char*>(&vic1),sizeof(int));
        archivo.write(reinterpret_cast<char*>(&vic2),sizeof(int));
        archivo.write(reinterpret_cast<char*>(&tablero), sizeof(Tablero));
        archivo.close();
    }
}

void GestorFicheros::leerTablero(Tablero &tablero,std::string nombre,bool &turno,int &modoJuego, int &modalidad,int &vic1, int &vic2){
    fs::path rutaBase (RUTA_DATOS);
    fs::path ruta = rutaBase/ "partidasGuardadas" / nombre;
    std::ifstream archivo (ruta,std::ios::in | std::ios::binary);

    if (archivo.is_open()){
        archivo.read(reinterpret_cast<char*>(&turno),sizeof(bool));
        archivo.read(reinterpret_cast<char*>(&modoJuego),sizeof(int));
        archivo.read(reinterpret_cast<char*>(&modalidad),sizeof(int));
        archivo.read(reinterpret_cast<char*>(&vic1),sizeof(int));
        archivo.read(reinterpret_cast<char*>(&vic2),sizeof(int));
        archivo.read(reinterpret_cast<char*>(&tablero), sizeof(Tablero));
        archivo.close();
    }
}

void GestorFicheros::registrarPartida(void){
    int num = 0;
    fs::path rutaBase (RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorPartidas";
    if (!fs::exists(carpeta)){
        fs::create_directories(carpeta);
    }

    fs::path rutaArchivo = carpeta/"contador.txt";
    std::ofstream archivo (rutaArchivo);
    if (archivo.is_open()){
        num = getContador();
        archivo << num + 1;
        archivo.close();
    }
}

int GestorFicheros::obtenerPartidas(void){
    int num = 0;
    fs::path rutaBase (RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorPartidas";
    if (!fs::exists(carpeta)){
        fs::create_directories(carpeta);
    }

    fs::path rutaArchivo = carpeta/"contador.txt";
    std::ifstream archivo (rutaArchivo);
    if (archivo.is_open()){
        archivo >> num;
        archivo.close();
    }

    return num;
}

std::vector<std::string> GestorFicheros::obtenerPartidasGuardadas(void){
    std::vector<std::string> lista;

    fs::path ruta (RUTA_DATOS);
    fs::path carpeta = ruta / "partidasGuardadas";

    if(fs::exists(carpeta)){
        for (const auto & entrada : fs::directory_iterator(carpeta))
            lista.push_back(entrada.path().filename().string());
    }
    return lista;
}
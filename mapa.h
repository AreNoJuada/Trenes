#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <map>
#include <utility>
#include <iostream>
using namespace std;

struct Ciudad {
    char nombre;
    int x;
    int y;
};

struct Trayecto {
    char origen;
    char destino;
    int longitud;
    string color;
    bool completado;
    int dueno;
    vector<pair<int, int>> posiciones;
};

class Mapa {
private:
    static const int WIDTH = 19;
    static const int HEIGHT = 14;
    int celdas[HEIGHT][WIDTH];
    map<char, pair<int, int>> posicionesCiudades;
    map<int, string> coloresTrayectos;
    vector<Trayecto> trayectosActivos;

public:
    Mapa();
    void mostrar();
    vector<pair<char, string>> obtenerTrayectosDesdeCiudad(char ciudad);
    int calcularLongitudTrayecto(char origen, char destino);
    bool existeTrayecto(char origen, char destino);
    bool completarTrayecto(char origen, char destino, int jugador_id);
};

#endif

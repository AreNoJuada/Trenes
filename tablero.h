// tablero.h
#ifndef TABLERO_H
#define TABLERO_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Trayecto {
    char origen;
    char destino;
    int longitud;
    string color;
    bool ocupado;

    Trayecto(char o, char d, int l, string c) 
        : origen(o), destino(d), longitud(l), color(c), ocupado(false) {}
};

void mostrarTablero(const vector<Trayecto> &trayectos) {
    cout << "\n=================== TABLERO DE TRENES ===================\n\n";
    cout << "   [A]====(4 ROJO)====[B]====(3 AZUL)====[C]\n";
    cout << "    |                  |                   |\n";
    cout << " (2 VERDE)        (5 MORADO)          (3 CAFE)\n";
    cout << "    |                  |                   |\n";
    cout << "   [D]====(2 NARANJA)==[E]====(6 ROJO)====[F]\n\n";
    cout << "Trayectos ocupados:\n";
    for (const auto &t : trayectos) {
        if (t.ocupado) {
            cout << "  " << t.origen << " - " << t.destino << " (" << t.longitud << " " << t.color << ") [OCUPADO]\n";
        }
    }
    cout << "\n=========================================================\n";
}

vector<Trayecto> obtenerTrayectos() {
    return {
        Trayecto('A','B',4,"Rojo"),
        Trayecto('B','C',3,"Azul"),
        Trayecto('A','D',2,"Verde"),
        Trayecto('B','E',5,"Morado"),
        Trayecto('C','F',3,"Cafe"),
        Trayecto('D','E',2,"Naranja"),
        Trayecto('E','F',6,"Rojo")
    };
}

#endif

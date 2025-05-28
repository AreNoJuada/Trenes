#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cctype>

#include "tablero.h"  // Incluir el archivo del tablero

using namespace std;

const int NUM_COLORES = 6;
const int CARTAS_POR_COLOR = 12;
const int VAGONES_POR_JUGADOR = 18;

string colores[] = {"Rojo", "Azul", "Verde", "Morado", "Cafe", "Naranja"};

struct Jugador {
    int id;
    vector<string> mano;
    int vagones;
    int puntos;
};

vector<string> crearMazo() {
    vector<string> mazo;
    for (int i = 0; i < NUM_COLORES; ++i) {
        for (int j = 0; j < CARTAS_POR_COLOR; ++j) {
            mazo.push_back(colores[i]);
        }
    }
    random_shuffle(mazo.begin(), mazo.end());
    return mazo;
}

void robarCartas(Jugador &jugador, vector<string> &mazo, vector<string> &descarte) {
    for (int i = 0; i < 2 && !mazo.empty(); ++i) {
        jugador.mano.push_back(mazo.back());
        mazo.pop_back();
    }
    if (mazo.empty()) {
        mazo = descarte;
        descarte.clear();
        random_shuffle(mazo.begin(), mazo.end());
    }
}

int calcularPuntos(int longitud) {
    switch (longitud) {
        case 2: return 1;
        case 3: return 2;
        case 4: return 4;
        case 5: return 6;
        case 6: return 9;
        default: return 0;
    }
}

bool realizarTrayecto(Jugador &jugador, vector<Trayecto> &trayectos, vector<string> &descarte, char o, char d) {
    for (auto &t : trayectos) {
        if (!t.ocupado && ((t.origen == o && t.destino == d) || (t.origen == d && t.destino == o))) {
            int cantidad = count(jugador.mano.begin(), jugador.mano.end(), t.color);
            if (cantidad >= t.longitud && jugador.vagones >= t.longitud) {
                jugador.puntos += calcularPuntos(t.longitud);
                jugador.vagones -= t.longitud;

                int removidos = 0;
                for (auto it = jugador.mano.begin(); it != jugador.mano.end() && removidos < t.longitud;) {
                    if (*it == t.color) {
                        descarte.push_back(*it);
                        it = jugador.mano.erase(it);
                        ++removidos;
                    } else {
                        ++it;
                    }
                }

                t.ocupado = true;
                cout << "Trayecto completado entre " << t.origen << " y " << t.destino << " (" << t.color << " - " << t.longitud << ")\n";
                return true;
            } else {
                cout << "No tienes suficientes cartas o vagones.\n";
                return false;
            }
        }
    }
    cout << "Ese trayecto no existe o ya está ocupado.\n";
    return false;
}

int main() {
    srand(time(0));
    int numJugadores;
    cout << "Ingrese el número de jugadores (2-4): ";
    cin >> numJugadores;

    vector<Jugador> jugadores;
    for (int i = 0; i < numJugadores; ++i) {
        jugadores.push_back({i + 1, {}, VAGONES_POR_JUGADOR, 0});
    }

    vector<string> mazo = crearMazo();
    vector<string> descarte;

    // Obtener los trayectos del archivo tablero.h
    vector<Trayecto> trayectos = obtenerTrayectos();

    bool juegoTerminado = false;
    while (!juegoTerminado) {
        for (int i = 0; i < numJugadores; ++i) {
            Jugador &jug = jugadores[i];

            cout << "\nTurno del Jugador " << jug.id << " - Vagones: " << jug.vagones << ", Puntos: " << jug.puntos << endl;
            mostrarTablero(trayectos);

            cout << "1. Robar cartas\n2. Realizar trayecto\nSeleccione una opción: ";
            int opcion;
            cin >> opcion;

            if (opcion == 1) {
                robarCartas(jug, mazo, descarte);
                cout << "Cartas en mano: ";
                for (auto &c : jug.mano) cout << c << " ";
                cout << endl;
            } else if (opcion == 2) {
                char origen, destino;
                cout << "Ingrese estación de origen (A-F): ";
                cin >> origen;
                cout << "Ingrese estación de destino (A-F): ";
                cin >> destino;
                realizarTrayecto(jug, trayectos, descarte, toupper(origen), toupper(destino));
            }

            if (jug.vagones < 5) juegoTerminado = true;
        }
    }

    cout << "\n=== Fin del juego ===\nPuntajes:\n";
    for (auto &j : jugadores) {
        cout << "Jugador " << j.id << ": " << j.puntos << " puntos\n";
    }

    return 0;
}

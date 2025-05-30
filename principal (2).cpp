#include "juego.h"
#include <iostream>
using namespace std;

int main() {
    cout << "=== JUEGO DE TRENES ===" << endl;
    int num_jugadores;
    do {
        cout << "Número de jugadores (2-4): ";
        cin >> num_jugadores;
    } while (num_jugadores < 2 || num_jugadores > 4);
    JuegoDeTrenes juego(num_jugadores);
    while (!juego.estaTerminado()) {
        juego.turnoJugador();
    }
    return 0;
}

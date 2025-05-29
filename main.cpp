#include "JuegoDeTrenes.h"
#include <iostream>

int main() {
    std::cout << "=== JUEGO DE TRENES ===" << std::endl;
    
    int num_jugadores;
    do {
        std::cout << "Número de jugadores (2-4): ";
        std::cin >> num_jugadores;
    } while (num_jugadores < 2 || num_jugadores > 4);
    
    JuegoDeTrenes juego(num_jugadores);
    
    while (!juego.estaTerminado()) {
        juego.turnoJugador();
    }
    
    return 0;
}
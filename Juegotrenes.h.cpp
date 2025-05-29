#include "JuegoDeTrenes.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <limits>
#include <iomanip>

JuegoDeTrenes::JuegoDeTrenes(int num_jugadores) {
    colores = {"rojo", "azul", "verde", "morado", "cafe", "naranja"};
    colores_ansi = {
        {"rojo", "\033[31m"},
        {"azul", "\033[34m"},
        {"verde", "\033[32m"},
        {"morado", "\033[35m"},
        {"cafe", "\033[33m"},
        {"naranja", "\033[38;5;208m"},
        {"reset", "\033[0m"}
    };
    colores_jugadores = {
        "\033[31m", "\033[34m", "\033[32m", "\033[35m"
    };
    
    inicializarMapa();
    inicializarBaraja();
    inicializarJugadores(num_jugadores);
    jugador_actual = 0;
    juego_terminado = false;
}

void JuegoDeTrenes::inicializarMapa() {
    ciudades = {
        {"A", 5, 5},
        {"B", 15, 5},
        {"C", 5, 10},
        {"D", 15, 10},
        {"E", 10, 15}
    };

    trayectos = {
        {"A", "B", 4, "rojo", false, 0},
        {"A", "C", 3, "azul", false, 0},
        {"B", "D", 2, "verde", false, 0},
        {"C", "D", 5, "morado", false, 0},
        {"C", "E", 4, "cafe", false, 0},
        {"D", "E", 3, "naranja", false, 0}
    };
}

// Resto de las implementaciones de métodos...
// [Aquí irían todas las demás funciones de la clase JuegoDeTrenes]
#ifndef JUEGO_H
#define JUEGO_H

#include "mapa.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

struct Jugador {
    int id;
    vector<string> cartas;
    int vagones_restantes;
    int puntos;
    string color_consola;
};

class JuegoDeTrenes {
private:
    Mapa mapa;
    vector<string> baraja;
    vector<string> descarte;
    vector<Jugador> jugadores;
    int jugador_actual;
    bool juego_terminado;
    vector<string> colores;
    map<string, string> colores_ansi;
    vector<string> colores_jugadores;

public:
    JuegoDeTrenes(int num_jugadores);
    void inicializarBaraja();
    void inicializarJugadores(int num_jugadores);
    void robarCartas(int jugador_id);
    bool intentarCompletarTrayecto(int jugador_id, char origen, char destino);
    void mostrarTrayectosDisponibles();
    void mostrarTrayectosCompletados();
    void mostrarEstado();
    void siguienteTurno();
    void terminarJuego();
    void turnoJugador();
    bool estaTerminado() const;
};

#endif

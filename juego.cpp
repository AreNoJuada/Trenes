#include "juego.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <limits>
using namespace std;

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
        "\033[31m", // Rojo
        "\033[34m", // Azul
        "\033[32m", // Verde
        "\033[35m"  // Morado
    };
    inicializarBaraja();
    inicializarJugadores(num_jugadores);
    jugador_actual = 0;
    juego_terminado = false;
}

void JuegoDeTrenes::inicializarBaraja() {
    for (const auto& color : colores) {
        for (int i = 0; i < 12; ++i) {
            baraja.push_back(color);
        }
    }
    shuffle(baraja.begin(), baraja.end(), default_random_engine(time(0)));
}

void JuegoDeTrenes::inicializarJugadores(int num_jugadores) {
    for (int i = 0; i < num_jugadores; ++i) {
        Jugador j;
        j.id = i + 1;
        j.vagones_restantes = 18;
        j.puntos = 0;
        j.color_consola = colores_jugadores[i % colores_jugadores.size()];
        jugadores.push_back(j);
    }
}

void JuegoDeTrenes::robarCartas(int jugador_id) {
    if (baraja.size() < 2) {
        cout << "Barajando cartas del descarte para formar nueva baraja..." << endl;
        baraja = descarte;
        descarte.clear();
        shuffle(baraja.begin(), baraja.end(), default_random_engine(time(0)));
    }
    if (baraja.size() >= 2) {
        for (int i = 0; i < 2; ++i) {
            string carta = baraja.back();
            baraja.pop_back();
            jugadores[jugador_id - 1].cartas.push_back(carta);
            cout << "Jugador " << jugador_id << " robó una carta " << carta << endl;
        }
    } else {
        cout << "No hay suficientes cartas para robar." << endl;
    }
}

bool JuegoDeTrenes::intentarCompletarTrayecto(int jugador_id, char origen, char destino) {
    if (!mapa.existeTrayecto(origen, destino)) {
        cout << "No hay trayecto entre " << origen << " and " << destino << endl;
        cout << "Trayectos disponibles desde " << origen << ":\n";
        auto disponibles = mapa.obtenerTrayectosDesdeCiudad(origen);
        for (const auto& t : disponibles) {
            cout << "- " << origen << " a " << t.first << " (color " << t.second << ")\n";
        }
        return false;
    }
    int longitud = mapa.calcularLongitudTrayecto(origen, destino);
    string color_necesario;
    auto trayectos = mapa.obtenerTrayectosDesdeCiudad(origen);
    for (const auto& t : trayectos) {
        if (t.first == destino) {
            color_necesario = t.second;
            break;
        }
    }
    Jugador& jugador = jugadores[jugador_id - 1];
    vector<string> cartas_temp = jugador.cartas;
    int cartas_necesarias = longitud;
    int cartas_del_color = count(cartas_temp.begin(), cartas_temp.end(), color_necesario);
    if (cartas_del_color >= cartas_necesarias) {
        cartas_temp.erase(
            remove_if(cartas_temp.begin(), cartas_temp.end(),
                [&](const string& c) {
                    if (c == color_necesario && cartas_necesarias > 0) {
                        cartas_necesarias--;
                        descarte.push_back(c);
                        return true;
                    }
                    return false;
                }),
            cartas_temp.end()
        );
        jugador.cartas = cartas_temp;
        mapa.completarTrayecto(origen, destino, jugador_id);
        jugador.vagones_restantes -= longitud;
        if (longitud == 2) jugador.puntos += 1;
        else if (longitud == 3) jugador.puntos += 2;
        else if (longitud == 4) jugador.puntos += 4;
        else if (longitud == 5) jugador.puntos += 6;
        else if (longitud == 6) jugador.puntos += 9;
        cout << "Jugador " << jugador_id << " completó el trayecto " << origen
             << " a " << destino << " (" << longitud << " vagones, color "
             << color_necesario << ")" << endl;
        mapa.mostrar();
        if (jugador.vagones_restantes < 5) {
            juego_terminado = true;
            cout << "Jugador " << jugador_id << " tiene menos de 5 vagones. Juego terminando..." << endl;
        }
        return true;
    } else {
        cout << "No tienes suficientes cartas " << color_necesario << " para este trayecto." << endl;
        cout << "Necesitas " << longitud << " cartas " << color_necesario << ", pero solo tienes "
             << cartas_del_color << endl;
        return false;
    }
}

void JuegoDeTrenes::mostrarTrayectosDisponibles() {
    cout << "\nTrayectos disponibles:\n";
    vector<char> ciudades = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S'};
    for (char ciudad : ciudades) {
        auto trayectos = mapa.obtenerTrayectosDesdeCiudad(ciudad);
        for (const auto& t : trayectos) {
            if (!mapa.existeTrayecto(ciudad, t.first)) continue;
            bool completado = false;
            for (const auto& j : jugadores) {
                if (j.id == t.second[0] - '0') {
                    completado = true;
                    break;
                }
            }
            if (!completado) {
                cout << "- " << ciudad << " a " << t.first << " ("
                     << mapa.calcularLongitudTrayecto(ciudad, t.first)
                     << " vagones, color " << t.second << ")\n";
            }
        }
    }
}

void JuegoDeTrenes::mostrarTrayectosCompletados() {
    cout << "\nTrayectos completados:\n";
    vector<char> ciudades = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S'};
    for (char ciudad : ciudades) {
        auto trayectos = mapa.obtenerTrayectosDesdeCiudad(ciudad);
        for (const auto& t : trayectos) {
            for (const auto& j : jugadores) {
                if (j.id == t.second[0] - '0') {
                    cout << "- " << ciudad << " a " << t.first << " ("
                         << mapa.calcularLongitudTrayecto(ciudad, t.first)
                         << " vagones) - Jugador " << j.id << "\n";
                }
            }
        }
    }
}

void JuegoDeTrenes::mostrarEstado() {
    cout << "\n=== ESTADO ACTUAL ===" << endl;
    for (const auto& jugador : jugadores) {
        cout << "Jugador " << jugador.id << ": " << jugador.puntos << " puntos, "
             << jugador.vagones_restantes << " vagones restantes, "
             << jugador.cartas.size() << " cartas en mano" << endl;
        cout << " Cartas: ";
        for (const auto& carta : jugador.cartas) {
            cout << colores_ansi[carta] << carta << colores_ansi["reset"] << " ";
        }
        cout << endl;
    }
    cout << "\nBaraja: " << baraja.size() << " cartas" << endl;
    cout << "Descarte: " << descarte.size() << " cartas" << endl;
}

void JuegoDeTrenes::siguienteTurno() {
    if (juego_terminado) {
        terminarJuego();
        return;
    }
    jugador_actual = (jugador_actual + 1) % jugadores.size();
    cout << "\nTurno del Jugador " << jugadores[jugador_actual].id << endl;
}

void JuegoDeTrenes::terminarJuego() {
    cout << "\n=== JUEGO TERMINADO ===" << endl;
    cout << "Puntuaciones finales:" << endl;
    int max_puntos = 0;
    int ganador = 0;
    for (const auto& jugador : jugadores) {
        cout << "Jugador " << jugador.id << ": " << jugador.puntos << " puntos" << endl;
        if (jugador.puntos > max_puntos) {
            max_puntos = jugador.puntos;
            ganador = jugador.id;
        } else if (jugador.puntos == max_puntos) {
            if (jugador.vagones_restantes > jugadores[ganador-1].vagones_restantes) {
                ganador = jugador.id;
            }
        }
    }
    cout << "\n¡Jugador " << ganador << " gana con " << max_puntos << " puntos!" << endl;
}

void JuegoDeTrenes::turnoJugador() {
    if (juego_terminado) return;
    Jugador& jugador = jugadores[jugador_actual];
    cout << "\nTurno del Jugador " << jugador.id << " (" << jugador.color_consola << "■\033[0m)" << endl;
    cout << "Vagones restantes: " << jugador.vagones_restantes << endl;
    cout << "Cartas en mano (" << jugador.cartas.size() << "): ";
    for (const auto& carta : jugador.cartas) {
        cout << colores_ansi[carta] << carta << colores_ansi["reset"] << " ";
    }
    cout << endl;
    int opcion;
    while (true) {
        cout << "\nElige una opción:\n";
        cout << "1. Robar 2 cartas\n";
        cout << "2. Completar trayecto\n";
        cout << "3. Mostrar mapa\n";
        cout << "4. Mostrar trayectos disponibles\n";
        cout << "5. Mostrar trayectos completados\n";
        cout << "6. Mostrar estado del juego\n";
        cout << "7. Terminar turno\n";
        cout << "Opción: ";
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Intenta de nuevo.\n";
            continue;
        }
        switch (opcion) {
            case 1:
                robarCartas(jugador.id);
                siguienteTurno();
                return;
            case 2: {
                char origen, destino;
                cout << "Ciudad origen (letra): ";
                cin >> origen;
                origen = toupper(origen);
                cout << "Ciudad destino (letra): ";
                cin >> destino;
                destino = toupper(destino);
                if (intentarCompletarTrayecto(jugador.id, origen, destino)) {
                    siguienteTurno();
                }
                return;
            }
            case 3:
                mapa.mostrar();
                break;
            case 4:
                mostrarTrayectosDisponibles();
                break;
            case 5:
                mostrarTrayectosCompletados();
                break;
            case 6:
                mostrarEstado();
                break;
            case 7:
                siguienteTurno();
                return;
            default:
                cout << "Opción inválida. Intenta de nuevo.\n";
        }
    }
}

bool JuegoDeTrenes::estaTerminado() const {
    return juego_terminado;
}

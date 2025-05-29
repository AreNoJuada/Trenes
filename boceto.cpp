#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

// Estructura para representar una ciudad en el mapa
struct Ciudad {
    string nombre;
    int x; // coordenada x en el mapa
    int y; // coordenada y en el mapa
};

// Estructura para representar un trayecto
struct Trayecto {
    string origen;
    string destino;
    int longitud;
    string color;
    bool completado;
    int dueno;
};

// Estructura para representar un jugador
struct Jugador {
    int id;
    vector<string> cartas;
    int vagones_restantes;
    int puntos;
    string color_consola; // Para mostrar en colores
};

class JuegoDeTrenes {
private:
    vector<Ciudad> ciudades;
    vector<Trayecto> trayectos;
    vector<string> baraja;
    vector<string> descarte;
    vector<Jugador> jugadores;
    int jugador_actual;
    bool juego_terminado;

    // Colores disponibles
    vector<string> colores = {"rojo", "azul", "verde", "morado", "cafe", "naranja"};
    
    // Colores ANSI para la consola
    map<string, string> colores_ansi = {
        {"rojo", "\033[31m"},
        {"azul", "\033[34m"},
        {"verde", "\033[32m"},
        {"morado", "\033[35m"},
        {"cafe", "\033[33m"},
        {"naranja", "\033[38;5;208m"},
        {"reset", "\033[0m"}
    };
    
    // Colores para jugadores
    vector<string> colores_jugadores = {
        "\033[31m", // Rojo
        "\033[34m", // Azul
        "\033[32m", // Verde
        "\033[35m"  // Morado
    };

public:
    JuegoDeTrenes(int num_jugadores) {
        inicializarMapa();
        inicializarBaraja();
        inicializarJugadores(num_jugadores);
        jugador_actual = 0;
        juego_terminado = false;
    }

    void inicializarMapa() {
        // Definir ciudades y sus posiciones
        ciudades = {
            {"A", 5, 5},
            {"B", 15, 5},
            {"C", 5, 10},
            {"D", 15, 10},
            {"E", 10, 15}
        };

        // Definir trayectos (origen, destino, longitud, color)
        trayectos = {
            {"A", "B", 4, "rojo", false, 0},
            {"A", "C", 3, "azul", false, 0},
            {"B", "D", 2, "verde", false, 0},
            {"C", "D", 5, "morado", false, 0},
            {"C", "E", 4, "cafe", false, 0},
            {"D", "E", 3, "naranja", false, 0}
        };
    }

    void inicializarBaraja() {
        // 12 cartas de cada color
        for (const auto& color : colores) {
            for (int i = 0; i < 12; ++i) {
                baraja.push_back(color);
            }
        }
        
        // Barajar las cartas
        shuffle(baraja.begin(), baraja.end(), default_random_engine(time(0)));
    }

    void inicializarJugadores(int num_jugadores) {
        for (int i = 0; i < num_jugadores; ++i) {
            Jugador j;
            j.id = i + 1;
            j.vagones_restantes = 18;
            j.puntos = 0;
            j.color_consola = colores_jugadores[i % colores_jugadores.size()];
            jugadores.push_back(j);
        }
    }

    void robarCartas(int jugador_id) {
        if (baraja.size() < 2) {
            // Si no hay suficientes cartas, barajar el descarte
            cout << "Barajando cartas del descarte para formar nueva baraja..." << endl;
            baraja = descarte;
            descarte.clear();
            shuffle(baraja.begin(), baraja.end(), default_random_engine(time(0)));
        }
        
        if (baraja.size() >= 2) {
            // Robar 2 cartas
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

    bool intentarCompletarTrayecto(int jugador_id, string origen, string destino) {
        // Buscar el trayecto
        Trayecto* trayecto = nullptr;
        for (auto& t : trayectos) {
            if ((t.origen == origen && t.destino == destino) || 
                (t.origen == destino && t.destino == origen)) {
                trayecto = &t;
                break;
            }
        }
        
        if (!trayecto) {
            cout << "No hay trayecto entre " << origen << " y " << destino << endl;
            return false;
        }
        
        if (trayecto->completado) {
            cout << "Este trayecto ya ha sido completado." << endl;
            return false;
        }
        
        // Verificar si el jugador tiene las cartas necesarias
        Jugador& jugador = jugadores[jugador_id - 1];
        vector<string> cartas_temp = jugador.cartas;
        int cartas_necesarias = trayecto->longitud;
        string color_necesario = trayecto->color;
        
        int cartas_del_color = count(cartas_temp.begin(), cartas_temp.end(), color_necesario);
        
        if (cartas_del_color >= cartas_necesarias) {
            // Eliminar las cartas usadas
            cartas_temp.erase(
                remove_if(cartas_temp.begin(), cartas_temp.end(), 
                    [&](const string& c) { 
                        if (c == color_necesario && cartas_necesarias > 0) {
                            cartas_necesarias--;
                            descarte.push_back(c); // Añadir al descarte
                            return true;
                        }
                        return false;
                    }),
                cartas_temp.end()
            );
            
            jugador.cartas = cartas_temp;
            trayecto->completado = true;
            trayecto->dueno = jugador_id;
            
            // Actualizar vagones
            jugador.vagones_restantes -= trayecto->longitud;
            
            // Calcular puntos según la longitud del trayecto
            if (trayecto->longitud == 2) jugador.puntos += 1;
            else if (trayecto->longitud == 3) jugador.puntos += 2;
            else if (trayecto->longitud == 4) jugador.puntos += 4;
            else if (trayecto->longitud == 5) jugador.puntos += 6;
            else if (trayecto->longitud == 6) jugador.puntos += 9;
            
            cout << "Jugador " << jugador_id << " completó el trayecto " << trayecto->origen 
                 << " a " << trayecto->destino << " (" << trayecto->longitud << " vagones, color " 
                 << trayecto->color << ")" << endl;
            
            // Verificar si el juego debe terminar
            if (jugador.vagones_restantes < 5) {
                juego_terminado = true;
                cout << "Jugador " << jugador_id << " tiene menos de 5 vagones. Juego terminando..." << endl;
            }
            
            return true;
        } else {
            cout << "No tienes suficientes cartas " << color_necesario << " para este trayecto." << endl;
            return false;
        }
    }

    void mostrarMapa() {
        const int ANCHO = 30;
        const int ALTO = 20;
        
        // Crear un buffer para el mapa
        vector<vector<string>> mapa(ALTO, vector<string>(ANCHO, " "));
        
        // Dibujar trayectos primero
        for (const auto& t : trayectos) {
            auto ciudad_origen = find_if(ciudades.begin(), ciudades.end(), 
                [&t](const Ciudad& c) { return c.nombre == t.origen; });
            auto ciudad_destino = find_if(ciudades.begin(), ciudades.end(), 
                [&t](const Ciudad& c) { return c.nombre == t.destino; });
                
            if (ciudad_origen != ciudades.end() && ciudad_destino != ciudades.end()) {
                int x1 = ciudad_origen->x;
                int y1 = ciudad_origen->y;
                int x2 = ciudad_destino->x;
                int y2 = ciudad_destino->y;
                
                // Dibujar línea entre ciudades
                string color = t.completado ? colores_jugadores[t.dueno - 1] : colores_ansi[t.color];
                string simbolo = t.completado ? "■" : "─";
                
                // Línea horizontal
                if (y1 == y2) {
                    for (int x = min(x1, x2); x <= max(x1, x2); x++) {
                        if (x >= 0 && x < ANCHO && y1 >= 0 && y1 < ALTO) {
                            mapa[y1][x] = color + simbolo + colores_ansi["reset"];
                        }
                    }
                }
                // Línea vertical
                else if (x1 == x2) {
                    for (int y = min(y1, y2); y <= max(y1, y2); y++) {
                        if (x1 >= 0 && x1 < ANCHO && y >= 0 && y < ALTO) {
                            mapa[y][x1] = color + "│" + colores_ansi["reset"];
                        }
                    }
                }
                // Línea diagonal
                else {
                    int dx = (x2 > x1) ? 1 : -1;
                    int dy = (y2 > y1) ? 1 : -1;
                    int x = x1;
                    int y = y1;
                    
                    while (x != x2 && y != y2) {
                        if (x >= 0 && x < ANCHO && y >= 0 && y < ALTO) {
                            mapa[y][x] = color + "╱" + colores_ansi["reset"];
                        }
                        x += dx;
                        y += dy;
                    }
                }
            }
        }
        
        // Dibujar ciudades
        for (const auto& c : ciudades) {
            if (c.x >= 0 && c.x < ANCHO && c.y >= 0 && c.y < ALTO) {
                mapa[c.y][c.x] = "○";
            }
        }
        
        // Dibujar nombres de ciudades
        for (const auto& c : ciudades) {
            if (c.x + 1 >= 0 && c.x + 1 < ANCHO && c.y >= 0 && c.y < ALTO) {
                mapa[c.y][c.x+1] = c.nombre;
            }
        }
        
        // Imprimir el mapa
        cout << "\n=== MAPA DEL JUEGO ===\n";
        cout << "Leyenda:\n";
        for (const auto& color : colores) {
            cout << colores_ansi[color] << "■ " << color << colores_ansi["reset"] << "  ";
        }
        cout << "\nTrayectos completados:\n";
        for (size_t i = 0; i < jugadores.size(); ++i) {
            cout << colores_jugadores[i] << "■ Jugador " << (i+1) << colores_ansi["reset"] << "  ";
        }
        cout << "\n\n";
        
        for (int y = 0; y < ALTO; ++y) {
            for (int x = 0; x < ANCHO; ++x) {
                cout << mapa[y][x];
            }
            cout << endl;
        }
    }

    void mostrarEstado() {
        cout << "\n=== ESTADO ACTUAL ===" << endl;
        for (const auto& jugador : jugadores) {
            cout << "Jugador " << jugador.id << ": " << jugador.puntos << " puntos, "
                 << jugador.vagones_restantes << " vagones restantes, "
                 << jugador.cartas.size() << " cartas en mano" << endl;
        }
        
        cout << "\nBaraja: " << baraja.size() << " cartas" << endl;
        cout << "Descarte: " << descarte.size() << " cartas" << endl;
    }

    void siguienteTurno() {
        if (juego_terminado) {
            terminarJuego();
            return;
        }
        
        jugador_actual = (jugador_actual + 1) % jugadores.size();
        cout << "\nTurno del Jugador " << jugadores[jugador_actual].id << endl;
    }

    void terminarJuego() {
        cout << "\n=== JUEGO TERMINADO ===" << endl;
        cout << "Puntuaciones finales:" << endl;
        
        int max_puntos = 0;
        int ganador = 0;
        
        for (const auto& jugador : jugadores) {
            cout << "Jugador " << jugador.id << ": " << jugador.puntos << " puntos" << endl;
            if (jugador.puntos > max_puntos) {
                max_puntos = jugador.puntos;
                ganador = jugador.id;
            }
        }
        
        cout << "\n¡Jugador " << ganador << " gana con " << max_puntos << " puntos!" << endl;
    }

    void turnoJugador() {
        if (juego_terminado) return;
        
        Jugador& jugador = jugadores[jugador_actual];
        cout << "\nTurno del Jugador " << jugador.id << endl;
        cout << "Vagones restantes: " << jugador.vagones_restantes << endl;
        cout << "Cartas en mano: ";
        for (const auto& carta : jugador.cartas) {
            cout << carta << " ";
        }
        cout << endl;
        
        int opcion;
        while (true) {
            cout << "\nElige una opción:\n";
            cout << "1. Robar 2 cartas\n";
            cout << "2. Completar trayecto\n";
            cout << "3. Mostrar mapa\n";
            cout << "4. Mostrar estado del juego\n";
            cout << "5. Terminar turno\n";
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
                    string origen, destino;
                    cout << "Ciudad origen: ";
                    cin >> origen;
                    cout << "Ciudad destino: ";
                    cin >> destino;
                    
                    if (intentarCompletarTrayecto(jugador.id, origen, destino)) {
                        siguienteTurno();
                    }
                    return;
                }
                case 3:
                    mostrarMapa();
                    break;
                case 4:
                    mostrarEstado();
                    break;
                case 5:
                    siguienteTurno();
                    return;
                default:
                    cout << "Opción inválida. Intenta de nuevo.\n";
            }
        }
    }

    bool estaTerminado() const {
        return juego_terminado;
    }
};

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
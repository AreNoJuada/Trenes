#include "mapa.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include <cctype>

Mapa::Mapa() {
    // Mapa inicial con ciudades como caracteres y colores como enteros
    // ATENCIÓN: Cambia los valores de ciudades (letras) a enteros si es necesario,
    // pero aquí se mantienen como char y se convierten a int al usar isalpha().
    // Para evitar confusión, los colores siempre son enteros.
    int mapaInicial[HEIGHT][WIDTH] = {
        {0,0,0,'D',2,2,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,2,0,0,0,0,'J',1,1,'N',4,4,0,0,0},
        {0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,4,0,0,0},
        {0,0,0,0,0,'E',0,0,0,5,5,5,5,5,5,'O',3,3,3},
        {1,'B',0,0,0,4,0,0,0,'I',0,0,0,0,0,6,0,0,3},
        {1,0,0,0,0,4,0,0,0,3,3,3,0,0,0,6,0,0,'R'},
        {1,0,0,0,0,4,0,0,0,0,0,3,0,0,0,6,0,0,0},
        {'A',5,5,5,5,'F',6,6,'H',5,5,'K',4,4,4,'P',0,0,0},
        {0,0,0,0,0,6,0,0,1,0,0,6,0,0,0,4,0,0,0},
        {0,0,0,0,0,6,0,0,1,0,0,6,0,0,0,4,0,0,'S'},
        {0,0,0,0,0,6,0,0,1,0,0,6,0,0,0,4,0,0,2},
        {0,0,2,2,2,'G',0,0,1,1,1,'L',0,0,0,'Q',2,2,2},
        {0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0},
        {0,0,'C',0,0,0,0,0,0,0,0,0,'M',3,3,3,0,0,0}
    };

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            celdas[y][x] = mapaInicial[y][x];
            if (isalpha(mapaInicial[y][x])) {
                posicionesCiudades[mapaInicial[y][x]] = make_pair(x, y);
            }
        }
    }

    coloresTrayectos = {
        {1, "rojo"},
        {2, "azul"},
        {3, "verde"},
        {4, "morado"},
        {5, "cafe"},
        {6, "naranja"}
    };

    // Inicializar trayectos con posiciones correctas (ejemplo, revisa que sean correctas para tu mapa)
    trayectosActivos = {
        {'A', 'B', 4, "rojo", false, 0, {{0,7}, {1,7}, {2,7}, {3,7}}},
        {'A', 'F', 5, "cafe", false, 0, {{4,7}, {5,7}, {6,7}, {7,7}, {8,7}}},
        {'A', 'H', 3, "cafe", false, 0, {{0,7}, {0,6}, {0,5}, {0,4}}}, // ¡Revisa que las posiciones sean correctas!
        {'B', 'I', 4, "rojo", false, 0, {{1,4}, {1,5}, {1,6}, {1,7}}},
        {'C', 'M', 3, "verde", false, 0, {{2,13}, {3,13}, {4,13}, {5,13}}},
        {'D', 'E', 2, "azul", false, 0, {{3,0}, {4,0}, {5,0}}},
        {'E', 'O', 3, "verde", false, 0, {{5,3}, {5,4}, {5,5}, {5,6}}},
        {'F', 'G', 2, "naranja", false, 0, {{5,7}, {5,8}, {5,9}, {5,10}, {5,11}}},
        {'F', 'K', 4, "morado", false, 0, {{5,7}, {6,7}, {7,7}, {8,7}, {9,7}}},
        {'G', 'L', 3, "azul", false, 0, {{5,11}, {6,11}, {7,11}, {8,11}}},
        {'G', 'Q', 2, "azul", false, 0, {{5,11}, {5,12}, {5,13}}},
        {'J', 'N', 2, "rojo", false, 0, {{10,1}, {11,1}, {12,1}}},
        {'K', 'P', 4, "morado", false, 0, {{11,7}, {11,8}, {11,9}, {11,10}, {11,11}}},
        {'L', 'Q', 3, "rojo", false, 0, {{11,11}, {12,11}, {13,11}, {14,11}}},
        {'P', 'Q', 2, "morado", false, 0, {{15,7}, {15,8}, {15,9}, {15,10}, {15,11}}}
    };
}

void Mapa::mostrar() {
    cout << "\n=== MAPA DEL JUEGO ===\n";
    map<pair<int, int>, int> posicionesJugadores;
    for (const auto& t : trayectosActivos) {
        if (t.completado) {
            for (const auto& pos : t.posiciones) {
                posicionesJugadores[pos] = t.dueno; // (x,y)
            }
        }
    }
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int c = celdas[y][x];
            if (c == 0) {
                cout << "  ";
            } else if (isalpha(c)) {
                cout << "\033[1;37m" << (char)c << "\033[0m ";
            } else {
                auto pos = make_pair(x, y);
                int jugador = 0;
                if (posicionesJugadores.find(pos) != posicionesJugadores.end()) {
                    jugador = posicionesJugadores[pos];
                }
                string color_code;
                switch(c) {
                    case 1: color_code = "\033[31m"; break; // rojo
                    case 2: color_code = "\033[34m"; break; // azul
                    case 3: color_code = "\033[32m"; break; // verde
                    case 4: color_code = "\033[35m"; break; // morado
                    case 5: color_code = "\033[33m"; break; // cafe
                    case 6: color_code = "\033[38;5;208m"; break; // naranja
                    default: color_code = "\033[0m";
                }
                if (jugador > 0) {
                    cout << color_code << jugador << jugador << "\033[0m ";
                } else {
                    cout << color_code << "■\033[0m ";
                }
            }
        }
        cout << endl;
    }
    cout << "\nLeyenda:\n";
    for (const auto& par : coloresTrayectos) {
        string color_code;
        switch(par.first) {
            case 1: color_code = "\033[31m"; break;
            case 2: color_code = "\033[34m"; break;
            case 3: color_code = "\033[32m"; break;
            case 4: color_code = "\033[35m"; break;
            case 5: color_code = "\033[33m"; break;
            case 6: color_code = "\033[38;5;208m"; break;
            default: color_code = "\033[0m";
        }
        cout << color_code << "■ " << par.second << "\033[0m\n";
    }
    cout << "Números indican jugador que completó el trayecto\n";
}

vector<pair<char, string>> Mapa::obtenerTrayectosDesdeCiudad(char ciudad) {
    vector<pair<char, string>> trayectos;
    for (const auto& t : trayectosActivos) {
        if (t.origen == ciudad) {
            trayectos.push_back({t.destino, t.color});
        } else if (t.destino == ciudad) {
            trayectos.push_back({t.origen, t.color});
        }
    }
    return trayectos;
}

int Mapa::calcularLongitudTrayecto(char origen, char destino) {
    for (const auto& t : trayectosActivos) {
        if ((t.origen == origen && t.destino == destino) ||
            (t.origen == destino && t.destino == origen)) {
            return t.longitud;
        }
    }
    return 0;
}

bool Mapa::existeTrayecto(char origen, char destino) {
    for (const auto& t : trayectosActivos) {
        if ((t.origen == origen && t.destino == destino) ||
            (t.origen == destino && t.destino == origen)) {
            return true;
        }
    }
    return false;
}

bool Mapa::completarTrayecto(char origen, char destino, int jugador_id) {
    for (auto& t : trayectosActivos) {
        if ((t.origen == origen && t.destino == destino) ||
            (t.origen == destino && t.destino == origen)) {
            if (!t.completado) {
                t.completado = true;
                t.dueno = jugador_id;
                return true;
            }
        }
    }
    return false;
}

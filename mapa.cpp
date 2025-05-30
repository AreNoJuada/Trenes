#include "mapa.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include <cctype>

Mapa::Mapa() {
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

    // TRAYECTOS EXACTOS SEGÚN TU ESPECIFICACIÓN
    trayectosActivos = {
        // D-E: 4 vagones azules
        {'D', 'E', 4, "azul", false, 0, {{3,0}, {4,0}, {4,1}, {5,1}}},
        
        // E-F: 3 vagones morados  
        {'E', 'F', 3, "morado", false, 0, {{5,3}, {5,4}, {5,5}}},
        
        // F-A: 5 vagones cafés
        {'F', 'A', 5, "cafe", false, 0, {{0,7}, {1,7}, {2,7}, {3,7}, {4,7}}},
        
        // A-B: 3 vagones rojos
        {'A', 'B', 3, "rojo", false, 0, {{0,4}, {0,5}, {0,6}}},
        
        // F-H: 2 vagones naranjas
        {'F', 'H', 2, "naranja", false, 0, {{6,7}, {7,7}}},
        
        // H-K: 2 vagones cafés
        {'H', 'K', 2, "cafe", false, 0, {{8,7}, {9,7}}},
        
        // K-L: 3 vagones naranjas
        {'K', 'L', 3, "naranja", false, 0, {{11,7}, {11,8}, {11,9}}},
        
        // L-M: 4 vagones rojos (asumo que L-H era L-M)
        {'L', 'M', 4, "rojo", false, 0, {{11,11}, {12,11}, {12,12}, {12,13}}},
        
        // K-P: 3 vagones morados
        {'K', 'P', 3, "morado", false, 0, {{12,7}, {13,7}, {14,7}}},
        
        // P-Q: 3 vagones morados
        {'P', 'Q', 3, "morado", false, 0, {{15,7}, {15,8}, {15,9}}},
        
        // Q-M: 4 vagones verdes
        {'Q', 'M', 4, "verde", false, 0, {{15,11}, {15,12}, {14,13}, {13,13}}},
        
        // Q-S: 4 vagones azules
        {'Q', 'S', 4, "azul", false, 0, {{16,11}, {17,11}, {18,10}, {18,9}}},
        
        // P-O: 3 vagones naranjas
        {'P', 'O', 3, "naranja", false, 0, {{15,7}, {15,4}, {15,3}}},
        
        // O-R: 4 vagones verdes
        {'O', 'R', 4, "verde", false, 0, {{16,3}, {17,3}, {18,4}, {18,5}}},
        
        // O-I: 4 vagones cafés
        {'O', 'I', 4, "cafe", false, 0, {{15,3}, {14,4}, {13,4}, {12,4}}},
        
        // I-K: 4 vagones verdes
        {'I', 'K', 4, "verde", false, 0, {{9,4}, {10,5}, {10,6}, {11,7}}},
        
        // O-N: 3 vagones morados
        {'O', 'N', 3, "morado", false, 0, {{15,3}, {14,2}, {13,1}}},
        
        // N-J: 2 vagones rojos
        {'N', 'J', 2, "rojo", false, 0, {{12,1}, {11,1}}}
    };
}

void Mapa::mostrar() {
    cout << "\n=== MAPA DEL JUEGO ===\n";
    map<pair<int, int>, int> posicionesJugadores;
    for (const auto& t : trayectosActivos) {
        if (t.completado) {
            for (const auto& pos : t.posiciones) {
                posicionesJugadores[pos] = t.dueno;
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
                    case 1: color_code = "\033[31m"; break;
                    case 2: color_code = "\033[34m"; break;
                    case 3: color_code = "\033[32m"; break;
                    case 4: color_code = "\033[35m"; break;
                    case 5: color_code = "\033[33m"; break;
                    case 6: color_code = "\033[38;5;208m"; break;
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

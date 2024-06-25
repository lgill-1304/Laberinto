#include <iostream> // para ingresar y dar salida de datos (imprimir)
#include <vector> // para hacer matriz estructura datos
#include <stack> // DFS LIFO pilas
#include <cstdlib> // random
#include <ctime> // para leer la hora

using namespace std;

// Direcciones posibles de movimiento
const int DIRECCIONES[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

// Función para verificar si una celda es válida para moverse
bool esValido(int x, int y, int ancho, int alto, const vector<vector<char>>& laberinto) {
    if (x < 0 || x >= ancho || y < 0 || y >= alto) return false;
    return laberinto[y][x] == ' ' || laberinto[y][x] == 'S';
}

// Función para verificar si una celda es válida durante la generación del laberinto
bool esValidoParaGeneracion(int x, int y, int ancho, int alto, const vector<vector<char>>& laberinto) {
    if (x <= 0 || x >= ancho-1 || y <= 0 || y >= alto-1) return false;
    if (laberinto[y][x] == ' ') return false;
    int paredes = 0;
    for (const auto& dir : DIRECCIONES) {
        if (laberinto[y + dir[1]][x + dir[0]] == '|') paredes++;
    }
    return paredes >= 3;
}

// Función para generar el laberinto utilizando DFS
void generarLaberinto(int ancho, int alto, vector<vector<char>>& laberinto) {
    stack<pair<int, int>> pila;
    pila.push({1, 1});
    laberinto[1][1] = ' ';

    srand(time(nullptr));
    while (!pila.empty()) {
        auto [x, y] = pila.top();
        vector<pair<int, int>> vecinos;

        for (const auto& dir : DIRECCIONES) {
            int nx = x + dir[0] * 2;
            int ny = y + dir[1] * 2;
            if (esValidoParaGeneracion(nx, ny, ancho, alto, laberinto)) {
                vecinos.push_back({nx, ny});
            }
        }

        if (!vecinos.empty()) {
            auto [nx, ny] = vecinos[rand() % vecinos.size()];
            laberinto[ny - (ny - y) / 2][nx - (nx - x) / 2] = ' ';
            laberinto[ny][nx] = ' ';
            pila.push({nx, ny});
        } else {
            pila.pop();
        }
    }
}

// Función para imprimir el laberinto
void imprimirLaberinto(const vector<vector<char>>& laberinto) {
    for (const auto& fila : laberinto) {
        for (char celda : fila) {
            if (celda == '*') {
                cout << "\033[1;32m" << celda << "\033[0m"; // Amarillo
            } else if (celda == 'X') {
                cout << "\033[1;31m" << celda << "\033[0m"; // Rojo
            } else {
                cout << celda;
            }
        }
        cout << '\n';
    }
}

// Función para resolver el laberinto usando backtracking
bool resolverLaberintoBacktracking(int x, int y, vector<vector<char>>& laberinto, vector<vector<bool>>& visitado) {
    if (laberinto[y][x] == 'S') return true;
    if (!esValido(x, y, laberinto[0].size(), laberinto.size(), laberinto) || visitado[y][x]) return false;

    visitado[y][x] = true;

    for (const auto& dir : DIRECCIONES) {
        int nx = x + dir[0];
        int ny = y + dir[1];
        if (resolverLaberintoBacktracking(nx, ny, laberinto, visitado)) {
            laberinto[y][x] = '*';
            return true;
        }
    }

    laberinto[y][x] = 'X'; // Marca como visitado pero no forma parte del camino final
    return false;
}

int main() {
    int ancho, alto;

    // Pedir al usuario el ancho y alto del laberinto
    do {
        cout << "Ingrese el ancho del laberinto (>= 3): ";
        cin >> ancho;
        if (ancho < 3) {
            cout << "El ancho debe ser un número mayor o igual a 3.\n";
        }
    } while (ancho < 3);

    do {
        cout << "Ingrese el alto del laberinto (>= 3): ";
        cin >> alto;
        if (alto < 3) {
            cout << "El alto debe ser un número mayor o igual a 3.\n";
        }
    } while (alto < 3);

    vector<vector<char>> laberinto(alto, vector<char>(ancho, '|'));

    generarLaberinto(ancho, alto, laberinto);

    // Establecer entrada y salida
    laberinto[1][0] = 'E';
    laberinto[alto-2][ancho-1] = 'S';

    cout << "Laberinto generado:\n";
    imprimirLaberinto(laberinto);

    // Resolver el laberinto usando backtracking
    vector<vector<bool>> visitado(alto, vector<bool>(ancho, false));
    if (resolverLaberintoBacktracking(1, 1, laberinto, visitado)) {
        cout << "\nSolucion encontrada usando backtracking:\n";
        laberinto[1][0] = 'E';  // Asegurar que la entrada se mantenga
        laberinto[alto-2][ancho-1] = 'S';  // Asegurar que la salida se mantenga
        imprimirLaberinto(laberinto);
    } else {
        cout << "\nNo se encontró solucion usando backtracking.\n";
        imprimirLaberinto(laberinto); // Imprimir el laberinto para ver los caminos visitados
    }

    return 0;
}


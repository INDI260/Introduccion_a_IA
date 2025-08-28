// =========================================================================
// Proyecto 1: Solución al problema de las jarras usando búsqueda BFS con 
// heurística de distancia de Manhattan
// @author Alejandro Caicedo Caicedo
// @author Carlos David Mercado Gallego
// @author Andrés Jerónimo Parrado Gutiérrez
// =========================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

const vector<int> CAPACIDADES = {4, 5, 6}; // Capacidades de las jarras
const tuple<int, int, int> ESTADO_INICIAL = {0, 0, 0};
const tuple<int, int, int> OBJETIVO = {3, 5, 4};

// ------------------------------------------------------------------------------------------------------------
// Calcula la heurística (distancia Manhattan) entre un estado y el objetivo.
int heuristica(const tuple<int, int, int>& estado)
{
    return abs(get<0>(estado) - get<0>(OBJETIVO)) +
           abs(get<1>(estado) - get<1>(OBJETIVO)) +
           abs(get<2>(estado) - get<2>(OBJETIVO));
}

// ------------------------------------------------------------------------------------------------------------
// Genera todos los estados vecinos posibles a partir de un estado dado y la acción realizada.
vector<pair<tuple<int, int, int>, string>> generar_vecinos(const tuple<int, int, int>& estado)
{
    vector<pair<tuple<int, int, int>, string>> vecinos;
    int a = get<0>(estado), b = get<1>(estado), c = get<2>(estado);
    vector<int> valores = {a, b, c};
    // Llenar
    for (int i = 0; i < 3; ++i)
    {
        if (valores[i] < CAPACIDADES[i])
        {
            vector<int> nuevo = valores;
            nuevo[i] = CAPACIDADES[i];
            vecinos.push_back({make_tuple(nuevo[0], nuevo[1], nuevo[2]), "Llenar jarra " + to_string(i+1)});
        } // end if
    } // end for

    // Vaciar
    for (int i = 0; i < 3; ++i)
    {
        if (valores[i] > 0)
        {
            vector<int> nuevo = valores;
            nuevo[i] = 0;
            vecinos.push_back({make_tuple(nuevo[0], nuevo[1], nuevo[2]), "Vaciar jarra " + to_string(i+1)});
        } // end if
    } // end for

    // Transvasar jarra i -> j
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (i == j)
            {
                continue;
            }
            if (valores[i] > 0 && valores[j] < CAPACIDADES[j])
            {
                int transferible = min(valores[i], CAPACIDADES[j] - valores[j]);
                vector<int> nuevo = valores;
                nuevo[i] -= transferible;
                nuevo[j] += transferible;
                vecinos.push_back({make_tuple(nuevo[0], nuevo[1], nuevo[2]), "Transvasar jarra " + to_string(i+1) + " -> jarra " + to_string(j+1)});
            } // end if
        } // end for
    } // end for
    return vecinos;
}

// ------------------------------------------------------------------------------------------------------------
// Construye el grafo de todos los estados posibles y sus transiciones.
map<tuple<int, int, int>, vector<pair<tuple<int, int, int>, string>>> construir_grafo()
{
    map<tuple<int, int, int>, vector<pair<tuple<int, int, int>, string>>> grafo;
    for (int a = 0; a <= CAPACIDADES[0]; ++a)
    {
        for (int b = 0; b <= CAPACIDADES[1]; ++b)
        {
            for (int c = 0; c <= CAPACIDADES[2]; ++c)
            {
                tuple<int, int, int> estado = make_tuple(a, b, c);
                grafo[estado] = generar_vecinos(estado);
            } // end for
        } // end for
    } // end for
    return grafo;
}

// ------------------------------------------------------------------------------------------------------------
// Realiza una búsqueda BFS con heurística para encontrar el camino desde el estado inicial al objetivo.
pair<vector<tuple<int, int, int>>, vector<string>> bfs_con_heuristica(
    const map<tuple<int, int, int>, vector<pair<tuple<int, int, int>, string>>>& grafo,
    tuple<int, int, int> inicio,
    tuple<int, int, int> objetivo
)
{
    queue<tuple<int, int, int>> cola;
    cola.push(inicio);
    map<tuple<int, int, int>, pair<tuple<int, int, int>, string>> visitado;
    visitado[inicio] = {make_tuple(-1, -1, -1), ""};
    while (!cola.empty())
    {
        auto actual = cola.front(); cola.pop();
        if (actual == objetivo)
        {
            // reconstruir camino (Backtracking)
            vector<tuple<int, int, int>> estados;
            vector<string> pasos;
            auto estado = objetivo;
            while (estado != make_tuple(-1, -1, -1))
            {
                estados.push_back(estado);
                pasos.push_back(visitado[estado].second);
                estado = visitado[estado].first;
            } // end while
            reverse(estados.begin(), estados.end());
            reverse(pasos.begin(), pasos.end());
            pasos.erase(pasos.begin()); // El primero es ""
            return {estados, pasos};
        } // end if
        auto vecinos = grafo.at(actual);
        sort(vecinos.begin(), vecinos.end(), [](auto& x, auto& y)
        {
            return heuristica(x.first) < heuristica(y.first);
        });
        for (auto& [siguiente, accion] : vecinos)
        {
            if (visitado.find(siguiente) == visitado.end())
            {
                visitado[siguiente] = {actual, accion};
                cola.push(siguiente);
            } // end if
        } // end for
    } // end while
    return {{}, {}};
}

// ------------------------------------------------------------------------------------------------------------
// Muestra la solución encontrada paso a paso en consola.
void mostrar_solucion(const vector<tuple<int, int, int>>& camino, const vector<string>& pasos)
{
    cout << "Solución encontrada:" << endl;
    for (size_t i = 0; i < camino.size(); ++i)
    {
        int a = get<0>(camino[i]), b = get<1>(camino[i]), c = get<2>(camino[i]);
        if (i == 0)
        {
            cout << "Paso " << i << ": (" << a << ", " << b << ", " << c << ") Estado Inicial" << endl;
        }
        else
        {
            cout << "Paso " << i << ": (" << a << ", " << b << ", " << c << ") - " << pasos[i-1] << endl;
        }
    } // end for
    cout << "Total de pasos: " << camino.size() - 1 << endl;
}

// ------------------------------------------------------------------------------------------------------------
// Función principal: construye el grafo, ejecuta la búsqueda y muestra la solución.
int main()
{
    auto grafo = construir_grafo();
    auto resultado = bfs_con_heuristica(grafo, ESTADO_INICIAL, OBJETIVO);
    auto camino = resultado.first;
    auto acciones = resultado.second;
    if (!camino.empty()) {
        mostrar_solucion(camino, acciones);
    } else {
        cout << "No se encontró solución." << endl;
    }
    return 0;
}

// eof - proyecto1.cpp
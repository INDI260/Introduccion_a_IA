# =========================================================================
# Proyecto 1: Solución al problema de las jarras usando búsqueda BFS con 
# heurística de distancia de Manhattan
# @author Alejandro Caicedo Caicedo
# @author Carlos David Mercado Gallego
# @author Andrés Jerónimo Parrado Gutiérrez
# =========================================================================

from collections import deque
from typing import Tuple, List, Dict

CAPACIDADES = [4, 5, 6] # Capacidades de las jarras
ESTADO_INICIAL = (0, 0, 0)
OBJETIVO = (3, 5, 4)

# ------------------------------------------------------------------------------------------------------------
# Calcula la heurística (distancia Manhattan) entre un estado y el objetivo.
def heuristica(estado: Tuple[int, int, int]) -> int:
    return abs(estado[0] - OBJETIVO[0]) + \
           abs(estado[1] - OBJETIVO[1]) + \
           abs(estado[2] - OBJETIVO[2])

# ------------------------------------------------------------------------------------------------------------
# Genera todos los estados vecinos posibles a partir de un estado dado y la acción realizada.
def generar_vecinos(estado: Tuple[int, int, int]) -> List[Tuple[Tuple[int, int, int], str]]:
    vecinos = []
    valores = list(estado)
    # Llenar
    for i in range(3):
        if valores[i] < CAPACIDADES[i]:
            nuevo = valores.copy()
            nuevo[i] = CAPACIDADES[i]
            vecinos.append(((nuevo[0], nuevo[1], nuevo[2]), f"Llenar jarra {i+1}"))
        # end if
    # end for

    # Vaciar
    for i in range(3):
        if valores[i] > 0:
            nuevo = valores.copy()
            nuevo[i] = 0
            vecinos.append(((nuevo[0], nuevo[1], nuevo[2]), f"Vaciar jarra {i+1}"))
        # end if
    # end for

    # Transvasar jarra i -> j
    for i in range(3):
        for j in range(3):
            if i == j:
                continue
            if valores[i] > 0 and valores[j] < CAPACIDADES[j]:
                transferible = min(valores[i], CAPACIDADES[j] - valores[j])
                nuevo = valores.copy()
                nuevo[i] -= transferible
                nuevo[j] += transferible
                vecinos.append(((nuevo[0], nuevo[1], nuevo[2]), f"Transvasar jarra {i+1} -> jarra {j+1}"))
            # end if
        # end for
    # end for
    return vecinos

# ------------------------------------------------------------------------------------------------------------
# Construye el grafo de todos los estados posibles y sus transiciones.
def construir_grafo() -> Dict[Tuple[int, int, int], List[Tuple[Tuple[int, int, int], str]]]:
    grafo = {}
    for a in range(CAPACIDADES[0] + 1):
        for b in range(CAPACIDADES[1] + 1):
            for c in range(CAPACIDADES[2] + 1):
                estado = (a, b, c)
                grafo[estado] = generar_vecinos(estado)
            # end for
        # end for
    # end for
    return grafo

# ------------------------------------------------------------------------------------------------------------
# Realiza una búsqueda BFS con heurística para encontrar el camino desde el estado inicial al objetivo.
def bfs_con_heuristica(
    grafo: Dict[Tuple[int, int, int], List[Tuple[Tuple[int, int, int], str]]],
    inicio: Tuple[int, int, int],
    objetivo: Tuple[int, int, int]
) -> Tuple[List[Tuple[int, int, int]], List[str]]:
    cola = deque()
    cola.append(inicio)
    visitado = {}
    visitado[inicio] = ((-1, -1, -1), "")
    while cola:
        actual = cola.popleft()
        if actual == objetivo:
            # reconstruir camino (Backtracking)
            estados = []
            pasos = []
            estado = objetivo
            while estado != (-1, -1, -1):
                estados.append(estado)
                pasos.append(visitado[estado][1])
                estado = visitado[estado][0]
            # end while
            estados.reverse()
            pasos.reverse()
            pasos.pop(0) # El primero es ""
            return estados, pasos
        # end if
        vecinos = grafo[actual]
        vecinos.sort(key=lambda x: heuristica(x[0]))
        for siguiente, accion in vecinos:
            if siguiente not in visitado:
                visitado[siguiente] = (actual, accion)
                cola.append(siguiente)
            # end if
        # end for
    # end while
    return [], []

# ------------------------------------------------------------------------------------------------------------
# Muestra la solución encontrada paso a paso en consola.
def mostrar_solucion(camino: List[Tuple[int, int, int]], pasos: List[str]) -> None:
    print("Solución encontrada:")
    for i in range(len(camino)):
        a, b, c = camino[i]
        if i == 0:
            print(f"Paso {i}: ({a}, {b}, {c}) Estado Inicial")
        else:
            print(f"Paso {i}: ({a}, {b}, {c}) - {pasos[i-1]}")
    # end for
    print(f"Total de pasos: {len(camino) - 1}")

# ------------------------------------------------------------------------------------------------------------
# Función principal: construye el grafo, ejecuta la búsqueda y muestra la solución.
def main():
    grafo = construir_grafo()
    camino, acciones = bfs_con_heuristica(grafo, ESTADO_INICIAL, OBJETIVO)
    if camino:
        mostrar_solucion(camino, acciones)
    else:
        print("No se encontró solución.")

if __name__ == "__main__":
    main()

# eof - proyecto1.py (Código generado automáticamente por GPT-4.1 27/08/2025 a partir del código en C++ en proyecto1.cpp)
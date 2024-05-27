Problema: 8-puzzles.
===
---
**Descripcion**
---

8-puzzle es un juego de rompecabezas que consiste en un marco de 3x3(nueve casillas) donde ocho de las casillas contienen bloques enumerados del 1 al 8, y una casilla esta vacia.El objetivo del juego es mover los bloques alrededor de la casilla vacía para alcanzar una configuración específica, generalmente ordenando los números en un patrón secuencial.

el objetivo en este caso es llegar a la siguiente forma:
![image](image.png)

Compilacion y ejecion del codigo
--
- Para lo siguiente es necesario tener un compilador de C compatible con las bibliotecas estandar de C.

---
Funcionalidades
---
- Funcion que retorna los estados adyacentes: esta funcion retorna estados con el valor 0 en una posicion diferente, guardando los diferentes estados conseguidos en una lista.
- Funcion DFS(busqueda en profundidad): implementa un algoritmo de busqueda en profundidad. Recibe una lista de los estados adyacentes y los procesa. Este algoritmo explora tan lejos como sea posible a lo largo de cada rama antes de retroceder.
- Funcion BFS(busqueda por anchura): Implementa el algoritmo de busqueda por anchura. Recibiendo tambien una lista de estados de adyacentes. Este algoritmo explora todos los nodos vecinos a la vez  antes de pasar a los nodos siguientes.
- Funcion bestFirst(buqueda mejor-primero): Implementa el algoritmo de busqueda mejor-primero. Utilizando una heuiristica para determinar que estado expandir primero. Los estados sin priorizados segun la estimacion de cuan cerca estan del objetivos(valor dado por la funcion distancia_L1).


---
Ejemplos de uso
---
**- Tomaremos de ejemplo la funcion BFS (opcion 2):**
![image](image2.png)

**- La funcion se encargara de ir tomando estados a traves del algoritmo de busqueda por anchura hasta encontrar el estado final (como se puede ver en la imagen al inicio del README)**
![image](image3.png)
**-Gracias a la lista de estados visitados, la funcion cumple de manera eficiente, generando en tan solo 1400 iteraciones el estado final (en el caso que se quiera ver todos los pasos, osea, sin implementar la lista de estados visitados, la funcion fue capaz de llegar a mas de 110 mil iteraciones!)**

---
Problemas a considerar
---
***Las funciones dfs y best_first no logran dar su cometido de llegar al final_state, ya que por algun motivo que desconocemos, el programa itera mucho tiempo y demasiadas veces, por lo que para que el programa no se fuera a infinito, se ha puesto un limite de iteraciones para que el codigo no se fuera al infinito, dando como resultado lo que se muestra en la imagen:**
![image](image4.png)

![image](image5.png)

---
Contribuciones
---
**Daniela Vallejos:**
- Implementacion de las funciones:
  - copy_state
  - adj_states
  - impAdj (que no se encuentra en uso)
  - finalState
  - dfs
  - state_priority
  - best_first
- Implementacion de limites de iteraciones en las funciones principales
- Diseño y redaccion del README

**Hugo Gonzalez:**
- Implementacion de las funciones:
  - stateToString
  - isVisited
  - addVisited
  - bfs
- Correciones de la funcion dfs
- Redaccion del README
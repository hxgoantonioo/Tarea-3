#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/stack.h"
#include <string.h>


// Definición de la estructura para el estado del puzzle
typedef struct {
    int square[3][3]; // Matriz 3x3 que representa el tablero
    int x;    // Posición x del espacio vacío
    int y;    // Posición y del espacio vacío
    List* actions; //Secuencia de movimientos para llegar al estado
    int steps;
} State;

int distancia_L1(State* state) {
    int ev=0;
    int k=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            int val=state->square[i][j];
            if (val==0) continue;
            int ii=(val)/3;
            int jj=(val)%3;
            ev+= abs(ii-i) + abs(jj-j);
        }
    return ev;
}

// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}


// Función que hace una copia del estado
void copyState(int src[3][3], int dest[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// Función para obtener los estados adyacentes
List* adjStates(State *state) {
    // Lista donde se guardarán los estados adyacentes
    List* adj_states = list_create();
    int movs[4][2] = {
        {-1, 0}, // arriba
        {1, 0},  // abajo
        {0, -1}, // izquierda
        {0, 1}   // derecha
    };

    // Bucle que itera sobre los posibles movimientos
    for (int i = 0; i < 4; i++) {
        int nx = state->x + movs[i][0];
        int ny = state->y + movs[i][1];

        // Se verifica que el movimiento sea válido, o sea, que esté dentro de los límites del tablero
        if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
            State* newState = (State*)malloc(sizeof(State));
            copyState(state->square, newState->square);

            // Se intercambia el espacio vacío con el espacio adyacente
            newState->square[state->x][state->y] = newState->square[nx][ny];
            newState->square[nx][ny] = 0;
            newState->x = nx;
            newState->y = ny;
            newState->actions = list_create(); // Inicializar la lista de acciones

            // Se agrega el nuevo estado a la lista de estados adyacentes
            list_pushBack(adj_states, newState);
        }
    }
    return adj_states;
}

// Fun

/*Función para imprimir los estados adyacentes
void impAdj(State* state) {
    List* adj = adjStates(state);
    int index = 1;
    State* adj_state = list_first(adj);
    while (adj_state != NULL) {
        printf("Estado adyacente %d:\n", index);
        imprimirEstado(adj_state);
        printf("\n");
        adj_state = list_next(adj);
        index++;
    }

    // Liberar memoria de los estados adyacentes
    adj_state = list_first(adj);
    while (adj_state != NULL) {
        State* next = list_next(adj);
        list_clean(adj_state->actions);
        free(adj_state);
        adj_state = next;
    }
    list_clean(adj);
}*/


//Funcion para convertir el estado a una cadena unica
void stateToString(State* state, char* str)
{
    int index = 0;
    for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                {
                    index += sprintf(&str[index], "%d", state->square[i][j]);
                }
        }
    str[index] = '\0';
}

//Funcion para vetrificar si un estado ya fue visitado
bool isVisited(const List *visited, const State *state)
{
    State *visitedState = list_first(visited);
    while (visitedState != NULL)
        {
            bool igual = true;
            for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                        {
                            if (visitedState->square[i][j] != state->square[i][j])
                            {
                                igual = false;
                                break;
                            }
                        }
                    if (!igual)
                    {
                        break;
                    }
                }
            if (igual)
            {
                return true;
            }
            visitedState = list_next(visited);
        }
    return false;
}

//Funcion para agregar un estado a la lista de visitados
void addVisited(List *visited, State *state)
{
    State *newState = (State *)malloc(sizeof(State));
    copyState(state->square, newState->square);
    newState->x = state->x;
    newState->y = state->y;
    list_pushBack(visited, newState);
}

//Funcion que verificara si se encuentra en estado final
int finalState(State *state){
  int isfinal[3][3] = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} };
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (state->square[i][j] != isfinal[i][j]) return 0;
    }
  }
  return 1;
}
void dfs(State *state) {
    Stack *pila = stack_create();
    List *visitedStates = list_create();

    stack_push(pila, state);
    addVisited(visitedStates, state);

    int iteraciones = 1;

    while (!stack_is_empty(pila)) {
        State *currentState = stack_top(pila);
        stack_pop(pila);

        printf("Iteracion DFS: %d\n", iteraciones);
        printf("Estado actual:\n");
        imprimirEstado(currentState);

        if (finalState(currentState)) {
            printf("Se encontró el estado final:\n");
            imprimirEstado(currentState);
            stack_clean(pila);
            list_clean(visitedStates);
            return;
        }
        List* adj_states = adjStates(currentState);
        State* adj_state = list_first(adj_states);
        while (adj_state != NULL) {
            if(!isVisited(visitedStates, adj_state)) {
                stack_push(pila, adj_state);
                addVisited(visitedStates, adj_state);
            }
            adj_state = list_next(adj_states);
        }
        list_clean(adj_states);
        printf("Numero de estados en la pila: %d\n", list_size(pila));
        iteraciones++;

        // Opcional: Agregar un límite de profundidad
        if(iteraciones >= 3000) {
            printf("Se alcanzó el límite de profundidad.\n");
            break;
        }
    }
    printf("No se encontró solución.\n");
    stack_clean(pila);
}


void bfs(State *initial_state) {
    List *queue = list_create();
    List *visited = list_create();
    
    list_pushBack(queue, initial_state);
    addVisited(visited, initial_state);

    int iteracion = 1; // Variable para contar las iteraciones

    while (list_size(queue) > 0) {
        State *currentState = (State *)list_popFront(queue);

        printf("Iteración BFS: %d\n", iteracion);
        printf("Estado actual:\n");
        imprimirEstado(currentState);

        if (finalState(currentState)) {
            printf("Se encontró el estado final:\n");
            imprimirEstado(currentState);
            list_clean(queue);
            list_clean(visited);
            return;
        }

        List* adj_states = adjStates(currentState);
        State* adj_state = list_first(adj_states);
        while (adj_state != NULL) {
            if (!isVisited(visited, adj_state))
            {
                list_pushBack(queue, adj_state);
                addVisited(visited, adj_state);
            }
            else
            {
                list_clean(adj_state->actions);
                free(adj_state);
            }
            adj_state = list_next(adj_states);
        }

        list_clean(adj_states);

        printf("Número de estados en la cola: %d\n", list_size(queue));

        iteracion++;
    }

    printf("No se encontró solución.\n");
    list_clean(queue);
    list_clean(visited);
}

int state_priority(const State* state) {
    return state->steps + distancia_L1(state);
}

void bestFirst(State *state) {
    Heap* heap = heap_create();
    List* visitedStates = list_create();

    state->steps = 0;
    heap_push(heap, state, state_priority(state));
    addVisited(visitedStates, state);

    int iteraciones = 1; // Contador de iteraciones para el límite de profundidad
    const int MAX_DEPTH = 100; // Definir el límite de profundidad máximo

    while (heap_top(heap) != NULL) {
        State* currentState = (State*)heap_top(heap);
        heap_pop(heap);

        printf("Estado actual en best_first:\n");
        imprimirEstado(currentState);

        if (finalState(currentState)) {
            printf("Estado final: \n");
            imprimirEstado(currentState);
            list_clean(visitedStates);
            return;
        }

        List* adj_states = adjStates(currentState);
        State* adj_state = list_first(adj_states);

        while (adj_state != NULL) {
            adj_state->steps = currentState->steps + 1;
            if (!isVisited(visitedStates, adj_state)) {
                heap_push(heap, adj_state, state_priority(adj_state));
                addVisited(visitedStates, adj_state);
            } 
            else {
                list_clean(adj_state->actions);
                free(adj_state);
            }
            adj_state = list_next(adj_states);
        }
        list_clean(adj_states);

        // Verificar el límite de profundidad
        if (iteraciones >= MAX_DEPTH) {
            printf("Límite de profundidad alcanzado.\n");
            break;
        }
        iteraciones++;
    }
    printf("No se encontró solución.\n");
    list_clean(visitedStates);
}

int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}, // Tercera fila
         },  
        0, 0   // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.actions = list_create();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));

    //Ejemplo de heap (cola con prioridad)
    printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
    Heap* heap = heap_create();
    char* data = strdup("Cinco");
    printf("Insertamos el elemento %s con prioridad -5\n", data);
    heap_push(heap, data, -5 /*prioridad*/);
    data = strdup("Seis");
    printf("Insertamos el elemento %s con prioridad -6\n", data);
    heap_push(heap, data, -6 /*prioridad*/);
    data = strdup("Siete");
    printf("Insertamos el elemento %s con prioridad -7\n", data);
    heap_push(heap, data, -7 /*prioridad*/);

    printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
    while (heap_top(heap) != NULL){
        printf("Top: %s\n", (char*) heap_top(heap));      
        heap_pop(heap);
    }
    printf("No hay más elementos en el Heap\n");

    char opcion;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
          dfs(&estado_inicial);
          break;
        case '2':
          bfs(&estado_inicial);
          break;
        case '3':
          //impAdj(&estado_inicial);
          bestFirst(&estado_inicial);
          break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

  } while (opcion != '4');

  return 0;
}

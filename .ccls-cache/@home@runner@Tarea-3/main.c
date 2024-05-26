#include <stdio.h>
#include <stdlib.h>
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


/* Función para imprimir los estados adyacentes
void impAdj(State* state) {
    List* adj = adjEstates(state);
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
}
*/

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

// Función de búsqueda en profundidad (DFS)
void dfs(State *state) {
    Stack *pila = stack_create();
    stack_push(pila, state);

    while (stack_top(pila) != NULL) {
        State *currentState = stack_top(pila);
        stack_pop(pila);

        if (finalState(currentState)) {
            printf("Se encontró el estado final:\n");
            imprimirEstado(currentState);
            stack_clean(pila);
            return;
        }

        List* adj_states = adjStates(currentState);
        State* adj_state = list_first(adj_states);
        while (adj_state != NULL) {
            stack_push(pila, adj_state);
            adj_state = list_next(adj_states);
        }

        // Liberar memoria de los estados adyacentes
        adj_state = list_first(adj_states);
        while (adj_state != NULL) {
            State* next = list_next(adj_states);
            list_clean(adj_state->actions);
            free(adj_state);
            adj_state = next;
        }
        list_clean(adj_states);
    }

    printf("No se encontró solución.\n");
    stack_clean(pila);
}
int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}, // Tercera fila
         },  
        0, 1   // Posición del espacio vacío (fila 0, columna 1)
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
          //bfs(estado_inicial);
          break;
        case '3':
          //best_first(estado_inicial);
          break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

  } while (opcion != '4');

  return 0;
}

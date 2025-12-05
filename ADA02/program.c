#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Para manejar el tiempo
#include <unistd.h> // Para la función sleep

// Definición de la estructura del cliente (sin id)
typedef struct {
    int transacciones; // Tiempo de servicio
    int cola;          // Cola asignada
    time_t llegada;    // Tiempo de llegada
    time_t salida;     // Tiempo de salida;
} Cliente;

// Definición de la estructura de la cola
typedef struct {
    Cliente *array;
    int frente;
    int final;
    int capacidad;
} Cola;

// Funciones para la cola
Cola* crearCola(int capacidad) {
    Cola* cola = (Cola*)malloc(sizeof(Cola));// Crear la cola
    cola->capacidad = capacidad;// Capacidad máxima de la cola
    cola->frente = cola->final = -1; // Inicializar frente y final
    cola->array = (Cliente*)malloc(cola->capacidad * sizeof(Cliente));// Crear array de clientes
    return cola;
}

// Funciones para verificar si la cola está vacía o llena
int estaVacia(Cola* cola) {
    return cola->frente == cola->final;// Si frente y final son iguales, la cola está vacía
}

int estaLlena(Cola* cola) {
    return cola->final == cola->capacidad - 1;// Si final alcanza la capacidad máxima, la cola está llena
}

// Funciones para encolar y desencolar clientes
void encolar(Cola* cola, Cliente cliente) {
    if (estaLlena(cola)) {// Si la cola está llena, no se puede encolar
        printf("Cola llena\n");
        return;
    }
    cola->final++;// Incrementar el final
    cola->array[cola->final] = cliente;// Añadir el cliente al final
}

Cliente desencolar(Cola* cola) {
    if (estaVacia(cola)) { 
        //printf("Cola vacia\n");
        Cliente cliente_vacio = {-1, 0, 0, 0};// Cliente vacío
        return cliente_vacio;// Retornar cliente vacío si la cola está vacía
    }
    cola->frente++;// Incrementar el frente
    return cola->array[cola->frente];// Retornar el cliente en el frente
}

// Funciones para la simulación
int generarTransacciones() {
    return rand() % 10 + 1; // Entre 1 y 10 transacciones
}

int asignarCola() {
    return rand() % 3; // Asignar aleatoriamente a una de las 3 colas
}

// Función para calcular el tiempo de espera
double calcularTiempoEspera(Cliente cliente) {
    return difftime(cliente.salida, cliente.llegada);// Diferencia en segundos entre salida y llegada
}

int main() {
    srand(time(NULL)); // Inicializar la semilla para números aleatorios

    // Crear las colas
    Cola* colas[3];// Tres colas
    // Inicializar cada cola
    for (int i = 0; i < 3; i++) {
        colas[i] = crearCola(100); // Capacidad de clientes por cola
    }

    int num_clientes = 15; // Simular 15 clientes
    Cliente clientes[num_clientes];// Array para almacenar los clientes

    // Simular la llegada de clientes
    for (int i = 0; i < num_clientes; i++) {
        clientes[i].transacciones = generarTransacciones(); // Generar transacciones
        clientes[i].cola = asignarCola();// Asignar cola
        time(&clientes[i].llegada); // Tiempo de llegada

        encolar(colas[clientes[i].cola], clientes[i]);// Encolar el cliente en la cola asignada
        printf("Cliente asignado a la cola %d con %d transacciones\n", 
               clientes[i].cola + 1, clientes[i].transacciones);
    }

    // Simular la atención de los clientes y calcular estadísticas
    double tiempoEsperaTotal[3] = {0, 0, 0};// Tiempo total de espera por cola
    int clientesAtendidos[3] = {0, 0, 0};// Contador de clientes atendidos por cola

    // Atender a los clientes en cada cola
    for (int i = 0; i < 3; i++) {
        printf("\nAtendiendo cola %d:\n", i + 1);
        // Mientras la cola no esté vacía, atender a los clientes
        while (!estaVacia(colas[i])) {
            Cliente cliente = desencolar(colas[i]);// Desencolar un cliente
            if (cliente.transacciones == 0) break; // Si la cola está vacía

            time(&cliente.salida); // Tiempo de salida
            printf("Atendiendo cliente de la cola %d, transacciones: %d\n", 
                   i + 1, cliente.transacciones);
            sleep(cliente.transacciones); // Simula el tiempo de servicio

            tiempoEsperaTotal[i] += calcularTiempoEspera(cliente);// Acumular el tiempo de espera
            clientesAtendidos[i]++;// Incrementar el contador de clientes atendidos
        }

        // Calcular el tiempo promedio de espera para la cola actual
        if (clientesAtendidos[i] > 0) {
            double tiempoPromedio = tiempoEsperaTotal[i] / clientesAtendidos[i];// Tiempo promedio de espera
            printf("Tiempo promedio de espera en la cola %d: %.2f segundos\n", i + 1, tiempoPromedio);
        } else {
            printf("No hubo clientes en la cola %d\n", i + 1);
        }

        printf("Clientes atendidos en la cola %d: %d\n", i + 1, clientesAtendidos[i]);
    }

    // Liberar la memoria de las colas
    for (int i = 0; i < 3; i++) {
        free(colas[i]->array);
        free(colas[i]);
    }

    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

//definicion de la estructura de nodo para la pelicula
struct Node {
    int ID; //identificador de la pelicula 
    char title[50]; //titulo de la pelicula
    double calificacion; //rating de la pelicula
    int duracion; //tiempo que dura la pelicula
    
    struct Node* next; //puntero al siguiente nodo
    struct Node* prev; //puntero al nodo anterior
};

typedef struct Node mov; //alias para declarar mov en vez de struct node
mov* head = NULL; //puntero global al primer nodo de la lista (cabeza)
mov* tail = NULL; //puntero global al ultimo nodo de la lista (cola)

//funcion para crear un nuevo nodo
mov* createNode(int ID, const char* title, double calificacion, int duracion) {
    mov* newNode = malloc(sizeof *newNode);
    if (!newNode) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    newNode->ID = ID;
    strncpy(newNode->title, title, sizeof newNode->title - 1);
    newNode->title[sizeof newNode->title - 1] = '\0';
    newNode->calificacion = calificacion;
    newNode->duracion = duracion;
    newNode->next = newNode->prev = NULL;
    return newNode;
}
//funcion para cargar los datos del archivo al DLL
int loadFromFile(FILE* file) { //funcion que lee linea por linea y crea los nodos 
    char line[200];
     rewind(file);  // Asegura que empiece desde el inicio
    int cont = 0;//contador de peliculas cargadas
    while (fgets(line, sizeof(line), file)) {
        int ID;
        char title[50];
        int duracion;
        double calificacion;

        //usando expresiones regulares para parsear el archivo CSV
       /* sscanf(line, "%d,%99[^,],%lf,%*[^,],%*[^,],%*[^,],%*[^,],%d", &ID, title, &calificacion, &duracion);
        //Se parsea la "line" con sscanf*/
        int n = sscanf(line, "%d,%49[^,],%lf,%*[^,],%*[^,],%*[^,],%*[^,],%d", &ID, title, &calificacion, &duracion);
        if (n != 4) {
            // línea mal formateada, la saltamos
            continue;
        } 
        mov* newNode = createNode(ID, title, calificacion, duracion); //crea el nodo usando los valores parseados 

        //si la lista esta vacia se apunta al nuevo nodo
        if (!head) {
            head = newNode;
            tail = newNode;
            //si la lista ya tiene nodos 
        } else {
            tail->next = newNode; //enlaza el ultimo con el nuevo
            newNode->prev = tail; //enlaza desde atras del nuevo al antiguo
            tail = newNode; //actualizacion para que sea el nuevo nodo
        }
    cont++;
    }
    //-----da ca corr
    return cont;
}
//Convertir a minusculas
void toLowerStr(char* str) {
    for (int i = 0; str[i]; i++)
        str[i] = (char)tolower((unsigned char)str[i]);
}

//funcion para buscar peliculas que coincidan con el nombre
void searchByName(const char* name) {
    mov* current = head; //inicia el recorrido desde la cabeza
    int found = 0; //se usa para saber si encontro al menos alguna coincidencia
    char searchLower[50];
    strncpy(searchLower, name, sizeof searchLower - 1);
    searchLower[sizeof searchLower - 1] = '\0';
    toLowerStr(searchLower);
    
    
    while (current) { //se recorre la lista mientras current no sea null
       char titleLower[50];
        strncpy(titleLower, current->title, sizeof titleLower - 1);
        titleLower[sizeof titleLower - 1] = '\0';
        toLowerStr(titleLower);
         
         if (strstr(titleLower, searchLower)) {
            //Datos en tabla
            printf("%d | %s | %d min | %.2f\n", current->ID, current->title, current->duracion, current->calificacion);
            found = 1;
        }
        current = current->next; //se avanza al siquiente nodo
    }
    //si no se encuentra ninguna coincidencia 
    if (!found) { 
        printf("No se encontraron peliculas con el nombre: %s\n", name);
    }
}


//funcion para añadir una nueva pelicula
void addMovie(int ID, char* title,  int duracion, double calificacion) {
    mov* newNode = createNode(ID, title, calificacion, duracion);
   //si la lista esta vacia
    if (!head) {
        head = newNode;
        tail = newNode;
    } else {
        //si ya hay elementos
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    printf("Pelicula añadida correctamente.\n");
}
// Función para eliminar una película por título (coincidencia parcial)
void deleteMovieByTitle(const char* name) {
    if (!head) {
        printf("No hay películas para eliminar.\n");
        return;
    }

    char searchLower[50];
    strncpy(searchLower, name, sizeof(searchLower) - 1);
    searchLower[sizeof(searchLower) - 1] = '\0';
    toLowerStr(searchLower); // convertir búsqueda a minúsculas

    mov* current = head;
    int found = 0;

    while (current) {
        char titleLower[50];
        strncpy(titleLower, current->title, sizeof(titleLower) - 1);
        titleLower[sizeof(titleLower) - 1] = '\0';
        toLowerStr(titleLower);

        if (strstr(titleLower, searchLower)) { // si coincide parcialmente
            found = 1;
            printf("\n--- Película encontrada ---\n");
            printf("ID: %d\nTítulo: %s\nDuración: %d min\nCalificación: %.2f\n",
                   current->ID, current->title, current->duracion, current->calificacion);
            
            printf("¿Desea eliminar esta película? (s/n): ");
            char respuesta;
            scanf(" %c", &respuesta);
            while (getchar() != '\n'); // limpiar buffer

            if (tolower(respuesta) == 's') {
                // Reajustar punteros de la lista
                if (current->prev)
                    current->prev->next = current->next;
                else
                    head = current->next; // si eliminamos la cabeza
                
                if (current->next)
                    current->next->prev = current->prev;
                else
                    tail = current->prev; // si eliminamos la cola
                
                free(current);
                printf(" Película eliminada correctamente.\n");
                return; // eliminar solo la primera coincidencia
            } else {
                printf(" Eliminación cancelada.\n");
                return;
            }
        }
        current = current->next;
    }

    if (!found)
        printf("No se encontró ninguna película con ese nombre.\n");
}

//funcion para mostrar todas las peliculas
void displayMoviesN(int n){
    mov* current = head; //inicio desde la cabeza 
    int count = 0;
    if (!current) {
        printf("No hay peliculas para mostrar.\n");
        return;
    }
    while (current && count < n) {//recorre la lista
        printf("ID: %d, Title: %s, Duration: %d, Rating: %.2f\n", current->ID, current->title,  current->duracion, current->calificacion);
        current = current->next; //imprime cada nodo
        count++;
    }
}



//actualizar pelicula por titulo (coincidencia parcial)
void updateMovieByTitle(const char* name) { 
    if (!head) {
        printf("No hay películas para actualizar.\n");
        return;
    }

    char searchLower[50];
    strncpy(searchLower, name, sizeof(searchLower) - 1);
    searchLower[sizeof(searchLower) - 1] = '\0';
    toLowerStr(searchLower); // convertir búsqueda a minúsculas

    mov* current = head;
    int found = 0;

    while (current) {
        char titleLower[50];
        strncpy(titleLower, current->title, sizeof(titleLower) - 1);
        titleLower[sizeof(titleLower) - 1] = '\0';
        toLowerStr(titleLower);

        if (strstr(titleLower, searchLower)) { // si coincide parcialmente
            found = 1;
            printf("\n--- Película encontrada ---\n");
            printf("ID: %d\nTítulo: %s\nDuración: %d min\nCalificación: %.2f\n",
                   current->ID, current->title, current->duracion, current->calificacion);
            
            printf("¿Desea actualizar esta película? (s/n): ");
            char respuesta;
            scanf(" %c", &respuesta);
            while (getchar() != '\n'); // limpiar buffer

            if (tolower(respuesta) == 's') {
                // Solicitar nuevos datos
                int newID, newDuracion;
                char newTitle[50];
                double newCalificacion;

                printf("Ingrese el nuevo ID (actual: %d): ", current->ID);
                scanf("%d", &newID);
                while (getchar() != '\n'); // limpiar buffer

                printf("Ingrese el nuevo título (actual: %s): ", current->title);
                fgets(newTitle, sizeof(newTitle), stdin);
                newTitle[strcspn(newTitle, "\n")] = '\0'; // eliminar salto de línea

                printf("Ingrese la nueva duración en minutos (actual: %d): ", current->duracion);
                scanf("%d", &newDuracion);
                while (getchar() != '\n'); // limpiar buffer

                printf("Ingrese la nueva calificación (actual: %.2f): ", current->calificacion);
                scanf("%lf", &newCalificacion);
                while (getchar() != '\n'); // limpiar buffer

                // Actualizar datos
                current->ID = newID;
                strncpy(current->title, newTitle, sizeof(current->title) - 1);
                current->title[sizeof(current->title) - 1] = '\0';
                current->duracion = newDuracion;    
                current->calificacion = newCalificacion;
                printf(" Película actualizada correctamente.\n");
                return; // actualizar solo la primera coincidencia 
            } else {
                printf(" Actualización cancelada.\n");
                return;
            }
        }
        current = current->next;
    }   
    if (!found)
        printf("No se encontró ninguna película con ese nombre.\n");
}




//liberar memoria de la lista
void freeList() {
    mov* current = head;
    while (current) {
        mov* temp = current;
        current = current->next;
        free(temp);
    }
    head = tail = NULL;
}



int main(){
    FILE* file = fopen("Movies.csv", "r"); //abre el archivo en modo lectura

    if(!file){//si no se puede abrir el archivo
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    //Menu
    int option; //variable para la opcion elegida por el usuario
    int resp=1;
    while(resp==1){ //control del ciclo del menu
        printf("\n---- MENU ----\n");
        printf("1. Cargar datos del archivo\n");
        printf("2. Busqueda de pelicula por nombre\n");
        printf("3. Añadir pelicula\n");
         printf("4. Eliminar pelicula\n");
         printf("5. Actualizar pelicula\n");
         printf("6. Mostrar n peliculas\n");
        printf("7. Salir\n");
        printf("Ingrese su opcion: ");

        if (scanf("%d", &option) != 1) {
            printf("Entrada invalida.\n");
            while (getchar() != '\n'); // limpiar buffer
            continue;
        }
            while (getchar() != '\n'); // limpiar salto de línea sobrante

        
        switch(option){
            case 1:
            {
                int count = loadFromFile(file); //llama a la funcion 
                printf(" %d Datos cargados correctamente.\n", count);
                break;
            }
            case 2: {
                //funcion para buscar pelicula por nombre
                char name[50];
                printf("Ingrese el nombre de la pelicula a buscar: ");
                fgets(name, sizeof name, stdin);
                name[strcspn(name, "\n")] = '\0'; // eliminar salto de línea
                searchByName(name);
                break;
            }
            
            case 3:
                //funcion añadir pelicula
                {
                    int ID, duracion;
                    char title[50];
                    double calificacion;
                    printf("Ingrese el ID de la pelicula: ");
                    scanf("%d", &ID);
                    printf("Ingrese el titulo de la pelicula: ");
                    scanf(" %[^\n]", title);
                    printf("Ingrese la duracion de la pelicula (en minutos): ");
                    scanf("%d", &duracion);
                    printf("Ingrese la calificacion de la pelicula (0.0 - 10.0): ");
                    scanf("%lf", &calificacion);
                    
                    addMovie(ID, title, duracion, calificacion);
                }
                break;
            case 4:
                {
                char name[50];
                printf("Ingrese el titulo (o parte del titulo) de la pelicula a eliminar: ");
                fgets(name, sizeof name, stdin);
                name[strcspn(name, "\n")] = '\0';
                deleteMovieByTitle(name);
                break;
}

            case 5:
                { 
                char name[50];
                printf("Ingrese el titulo (o parte del titulo) de la pelicula a actualizar: ");
                fgets(name, sizeof name, stdin);
                name[strcspn(name, "\n")] = '\0';
                updateMovieByTitle(name);
                break;
            }
            case 6:
                {
                    int n;
                    printf("Ingrese el numero de peliculas a mostrar: ");
                    scanf("%d", &n);
                    displayMoviesN(n);
                    break;
                }
             case 7:
                resp = 0;
                break;   
                 
               
            default:
                printf("Opcion no valida.\n");
        }
    }

    fclose(file);
    freeList();
    printf("Programa finalizado. \n");
    return 0;
}





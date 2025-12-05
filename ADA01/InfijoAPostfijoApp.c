#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Definición de la pila para caracteres
typedef struct {
    int tope;
    int capacidad;
    char *elementos;
} PilaCaracter;

// Definición de la pila para números
typedef struct {
    int tope;
    int capacidad;
    double *elementos;
} PilaNumero;

// Funciones para la pila de caracteres
void inicializarPilaCaracter(PilaCaracter* p, int capacidad) {
    p->tope = -1;
    p->capacidad = capacidad;
    p->elementos = (char*) malloc(sizeof(char) * capacidad);
}

int pilaCaracterVacia(PilaCaracter* p) {
    return p->tope == -1;
}
void apilarCaracter(PilaCaracter* p, char c) {
    if (p->tope < p->capacidad - 1) {
        p->elementos[++(p->tope)] = c;
    }
}
char desapilarCaracter(PilaCaracter* p) {
    if (!pilaCaracterVacia(p)) {
        return p->elementos[(p->tope)--];
    }
    return '\0';
}
char cimaCaracter(PilaCaracter* p) {
    if (!pilaCaracterVacia(p)) {
        return p->elementos[p->tope];
    }
    return '\0';
}
void liberarPilaCaracter(PilaCaracter* p) {
    free(p->elementos);
}

// Funciones para la pila de números
void inicializarPilaNumero(PilaNumero* p, int capacidad) {
    p->tope = -1;
    p->capacidad = capacidad;
    p->elementos = (double*) malloc(sizeof(double) * capacidad);
}
int pilaNumeroVacia(PilaNumero* p) {
    return p->tope == -1;
}
void apilarNumero(PilaNumero* p, double num) {
    if (p->tope < p->capacidad - 1) {
        p->elementos[++(p->tope)] = num;
    }
}
double desapilarNumero(PilaNumero* p) {
    if (!pilaNumeroVacia(p)) {
        return p->elementos[(p->tope)--];
    }
    return 0;
}
void liberarPilaNumero(PilaNumero* p) {
    free(p->elementos);
}

//función para determinar la precedencia de los operadores
int precedencia(char op) {
    switch (op) {
        case '+': case '-': return 1;// Menor precedencia
        case '*': case '/': return 2;
        case '^': return 3; // Mayor precedencia
        default: return 0;
    }
}


//funcion para verificar si un caracter es un operador
int esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Función para convertir infija a postfija
void infijaAPostfija(char* infija, char* postfija) {
    PilaCaracter pila;
    inicializarPilaCaracter(&pila, strlen(infija) + 1);

    int i = 0, k = 0; // i para infija, k para postfija
    while (infija[i] != '\0') {
        if (isdigit(infija[i])) { //verificar si el caracter es un número
            while (isdigit(infija[i])) {//mientras sea un número
                postfija[k++] = infija[i++]; // Copiar el número a postfija
            }
            postfija[k++] = ' ';//agregar espacio para separar los números
            continue;//continuar con el siguiente caracter
        } else if (infija[i] == '(') { //si es un paréntesis IZQUIERDO
            apilarCaracter(&pila, infija[i]); //apilarlo en la pila de catacteres
        } else if (infija[i] == ')') { //si es un paréntesis DERECHO
            while (!pilaCaracterVacia(&pila) && cimaCaracter(&pila) != '(') { //mientras la pila no esté vacía y el tope no sea un paréntesis IZQUIERDO
                postfija[k++] = desapilarCaracter(&pila); //desapilar y agregarlo a postfija hasta encontrar el paréntesis IZQUIERDO
                postfija[k++] = ' ';//agregar los espacios
            }
            desapilarCaracter(&pila);// Cuando se encuentre el parentesis IZQUIERDO, desapilar el '(' de la pila
        } else if (esOperador(infija[i])) { //si es un operador
            while (!pilaCaracterVacia(&pila) && precedencia(cimaCaracter(&pila)) >= precedencia(infija[i])) {//verificar la precedencia
                postfija[k++] = desapilarCaracter(&pila);//desapilar y agregar a postfija
                postfija[k++] = ' ';
            }
            apilarCaracter(&pila, infija[i]);//apilar el operador actual
        }
        i++;
    }
    while (!pilaCaracterVacia(&pila)) {//desapilar todos los operadores restantes en la pila
        postfija[k++] = desapilarCaracter(&pila);//desapilar y agregar a postfija
        postfija[k++] = ' ';
    }
    postfija[k] = '\0';//terminar la cadena postfija

    liberarPilaCaracter(&pila);//liberar la memoria de la pila
}

// Función para evaluar la expresión postfija
double evaluarPostfija(char* postfija) {
    PilaNumero pila;
    inicializarPilaNumero(&pila, strlen(postfija) + 1);

    int i = 0;
    char token[100];//para almacenar números como cadena

    while (postfija[i] != '\0') {//recorrer la expresión postfija
        if (isdigit(postfija[i])) {//si es un número
            int j = 0;
            while (isdigit(postfija[i])) {//mientras sea un NUMERO
                token[j++] = postfija[i++];//copiar el número al token
            }
            token[j] = '\0';//terminar la cadena token
            apilarNumero(&pila, atof(token));//convertir el token a número y apilarlo
        } else if (esOperador(postfija[i])) {//si es un OPERADOR
            double val2 = desapilarNumero(&pila);//desapilar el segundo operando
            double val1 = desapilarNumero(&pila);//desapilar el primer operando
            switch (postfija[i]) {//verificar el operador y realizar la operación correspondiente
                case '+': apilarNumero(&pila, val1 + val2); break;
                case '-': apilarNumero(&pila, val1 - val2); break;
                case '*': apilarNumero(&pila, val1 * val2); break;
                case '/': 
                if(val2 == 0) {//verificar división por cero
                    printf("Error: Division por cero.\n");
                    liberarPilaNumero(&pila);
                    exit(EXIT_FAILURE);
                }
                apilarNumero(&pila, val1 / val2); break;
                case '^': {
                    double res = 1;
                    for (int p = 0; p < (int)val2; p++) res *= val1;
                    apilarNumero(&pila, res);
                    break;
                }
            }
        }
        i++;
    }
    double resultado = desapilarNumero(&pila);//el resultado final está en la cima de la pila
    liberarPilaNumero(&pila);//liberar la memoria de la pila
    return resultado;//devolver el resultado
}

int main() {
    FILE *entrada, *salida;//archivos de entrada y salida
    char infija[200], postfija[200];//cadenas para las expresiones
    double resultado;

    // Abrir archivos
    entrada = fopen("expr_infijas.txt", "r");//archivo de entrada como lectura
    salida = fopen("expr_posfijas.txt", "w");//archivo de salida como escritura


    if (!entrada || !salida) {//verificar si los archivos se abrieron correctamente
        printf("Error al abrir archivos.\n");
        return 1;
    }

    while (fgets(infija, sizeof(infija), entrada)) {//leer cada línea del archivo de entrada
        infija[strcspn(infija, "\n")] = 0;//remover el salto de línea

        char *expresion = strtok(infija, ";");//dividir la línea en expresiones separadas por ';'
        while (expresion != NULL) {//
            while (*expresion == ' ') expresion++;//remover espacios al inicio
            int len = strlen(expresion);//remover espacios al final
            while (len > 0 && expresion[len - 1] == ' ') {//mientras haya espacios al final
                expresion[--len] = '\0';//remover el espacio
            }

            if (len > 0) {//si la expresión no está vacía
                infijaAPostfija(expresion, postfija);//llamar a la función para convertir infija a postfija
                resultado = evaluarPostfija(postfija);//llamar a la función para evaluar la expresión postfija

                fprintf(salida, "Postfija: %s; ", postfija);//escribir la expresión postfija en el archivo de salida
                fprintf(salida, "Resultado: %.2f\n\n", resultado);//escribir el resultado en el archivo de salida
            }
            expresion = strtok(NULL, ";");//obtener la siguiente expresión
        }
    }

    // Cerrar archivos
    fclose(entrada);
    fclose(salida);

    printf("Archivo de salida completado!\n");
    return 0;
}

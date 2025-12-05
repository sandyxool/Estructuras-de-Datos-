# Estructuras de Datos: Actividad final
# Unidad 1 Pilas y colas: 
**ADA 1: Pilas**

**Problema:** Parser de Expresiones Aritméticas 

**Descripción:** Evaluar expresiones aritméticas tales como: 2+3, 2*(3+4) o ((2+4)*7)+3*(9–5) es tarea de un 
analizador sintáctico (parser). Para realizar la evaluación de estas expresiones, muchos lenguajes de 
programación utilizan un algoritmo de dos pasos: 
1. Transformar la expresión aritmética en notación de postfija (Notación Polaca Inversa). 
2. Evaluar la expresión de postfija. 
La notación postfija de las expresiones antes mencionadas: 3 2 + ,   2 3 4 + *,   2 4 + 7 * 3 9 5 - * +

Realizar un programa que por medio de la línea de comando (cmd o terminal) que lea un archivo de entrada con un 
conjunto de líneas con expresiones aritméticas (infijas) con delimitador final punto y coma; genere un archivo de 
salida con un conjunto de líneas con las expresiones postfijas correspondientes, así como el resultado de evaluar 
tal expresión postfija y con delimitador final punto y coma. 


**ADA 2: Colas**

**Problema** Simulador de Atención al Cliente en un Banco (Queues)

**Descripción:** Desarrollar un programa que simule la atención en un banco con 3 cajas 
de atención. Los clientes llegan de forma aleatoria a las cajas: 
La aplicación debe:
* Generar a los clientes con el número de transacciones bancarias (tiempo en que estará en la 
caja) 
* Asignar de forma aleatoria a cada cliente a la cola correspondiente (cola 1, 2 o 3). 
* Atender a los clientes (indicar cuando ingresa a la cola el cliente, a qué cola ingresó y cuando 
salió el cliente) 
* Calcular algunas estadísticas como: tiempo promedio de espera en cada cola, número de 
* clientes atendidos en cada cola, etc.

# Unidad 2 listas ligadas:

**ADA 3: Listas**

**Problema:** Implementación de lista de Movies con Listas 

**Descripción:** Utilizando el archivo Movie.csv implemente una Lista Doblemente Ligada de Movies (películas). La 
implementación deberá contar con los siguientes procesos: 
* Pueden seleccionar los atributos del archivo Movie que consideren más relevantes y puede seleccionar 
algunas filas
* Su Lista Ligada almacenará cada uno de los datos de las películas. 
* Los procesos o funciones para contemplar son:
  
         - Importación de los datos a la lista (carga de datos).
  
         - Búsqueda de alguna película por título (puede ser nombre exacto o no)
         y que muestre toda la información de esa película (director, actores, año de estreno, etc.)
  
         - Búsqueda de algún actor (cast) por nombre (puede ser nombre exacto o no) y que muestre toda
         la información del actor (nombre de la película en la que ha trabajado, con el director de la película, 
         el año de estreno, etc.)
  
         - Insertar una nueva película con todos sus datos, la inserción puede ser en cualquier lugar de la 
         lista, al principio, al final, etc.

         - Eliminar una película por título (puede ser nombre exacto o no) y que muestre toda la información 
         de la película antes de eliminar indicando con un mensaje al usuario ¿desea eliminar?

          - Actualizar algunos datos de la película, aquí primero se debe hacer una búsqueda por título de la 
          película y posteriormente se le indica al usuario que dato de la película se quiere modificar. 

 

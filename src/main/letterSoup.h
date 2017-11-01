#ifndef LETTER_SOUP_H_INCLUDED
#define LETTER_SOUP_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
    Definimos una estructura Solucion con tres enteros, un x e y para representar la posicion en
    la sopa de letras, y direccion representando de 0 para arriba hasta 7 en el sentido horario.
*/
typedef struct {
    int x;
    int y;
    int direccion;
} Solucion;

/*
    Definimos una estructura Palabra con un puntero a char palabra para almacenar la palabra, 
    y un puntero a Solucion solucion representando la solucion de la sopa de letras (NULL si no
    tiene solucion).
*/
typedef struct {
    char *palabra;
    Solucion *solucion;
} Palabra;

/*
    Definimos una estructura Universo con un entero tamanioUniverso representando la cantidad de
    palabras, y un puntero a Palabra para guardar el conjunto de palabras.
*/
typedef struct Universo {
    int tamanioUniverso;
    Palabra *palabras;
} Universo;

/*
    Definimos una estructura SopaDeLetras con dos enteros, numeroDeColumnas y numeroDeFilas
    representando las dimensiones de la sopa de letras, y un doble puntero a char celdas
    donde guardamos las letras.
*/
typedef struct {
    int numeroDeColumnas;
    int numeroDeFilas;
    char **celdas;
} SopaDeLetras;

int esDireccion (SopaDeLetras sopaDeLetras, char *palabra, int x, int y, int direccion);

int encontrarDireccion (SopaDeLetras sopaDeLetras, char *palabra, int x, int y);

Solucion *encontrarSolucion (SopaDeLetras sopaDeLetras, char *palabra);

Universo resolverSopaDeLetras (SopaDeLetras sopaDeLetras, Universo universo);

#endif
#include "letterSoup.h"
#include <stdio.h>
#include <stdlib.h>

// Constantes de colores para terminales ANSI
#ifndef WINDOWS
    #define COLOR_ROJO     "\x1b[31m"
    #define COLOR_VERDE    "\x1b[32m"
    #define COLOR_DEFECTO  "\x1b[0m"
#else
    #define COLOR_ROJO     ""
    #define COLOR_VERDE    ""
    #define COLOR_DEFECTO  ""
#endif

/*
    limpiarPantalla: () -> void

    Limpia la pantalla de la terminal con una llamada al sistema.
*/
void limpiarPantalla () {
    #ifdef WINDOWS
        system("cls");
    #else
        system ("clear");
    #endif
}

/*
    leerSopaDeLetras: () -> SopaDeLetras

    Le pide al usuario la cantidad de columnas y filas, y luego el ingreso de la sopa de letras
    y retorna una estructura SopaDeLetras.
*/
SopaDeLetras leerSopaDeLetras () {
    SopaDeLetras sopaDeLetras;

    printf("Ingrese la cantidad de columnas:\n");
    scanf("%d", &(sopaDeLetras.numeroDeColumnas));

    printf("Ingrese la cantidad de filas:\n");
    scanf("%d", &(sopaDeLetras.numeroDeFilas));

    char buffer[sopaDeLetras.numeroDeFilas + 1];
    int valid;
    sopaDeLetras.celdas = malloc(sopaDeLetras.numeroDeFilas * sizeof(char *));

    int y;
    for (y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        valid = 0;

        while (valid == 0) {
            printf("Ingrese la fila %d: ", y + 1);
            scanf("%s", buffer);

            if (strlen(buffer) == sopaDeLetras.numeroDeColumnas) {
                valid = 1;
            } else {
                printf("%s¡NO TIENE LA CANTIDAD DE COLUMANS INGRESADAS!%s\n", COLOR_ROJO, COLOR_DEFECTO);
            }
        }

        sopaDeLetras.celdas[y] = malloc((sopaDeLetras.numeroDeColumnas + 1) * sizeof(char));
        strcpy(sopaDeLetras.celdas[y], buffer);
    }

    return sopaDeLetras;
}

/*
    leerUniverso: () -> Universo

    Le pide al usuario la cantidad de palabras, y luego el ingreso de las mismas y retorna una
    estructura Universo con las palabras sin solucion.
*/
Universo leerUniverso () {
    Universo universo;
    char buffer[100];

    printf("Ingrese la cantiad de palabras:\n");
    scanf("%d", &(universo.tamanioUniverso));

    universo.palabras = malloc(universo.tamanioUniverso * sizeof(Palabra));

    int x;
    for (x = 0; x < universo.tamanioUniverso; x++) {
        printf("Ingrese la palabra %d: ", x + 1);
        scanf("%s", buffer);

        universo.palabras[x].palabra    = malloc((strlen(buffer) + 1) * sizeof(char));
        universo.palabras[x].solucion   = NULL;

        strcpy(universo.palabras[x].palabra, buffer);
    }

    return universo;
}

/*
    mostrarUniverso: Universo -> void

    Muestra por pantalla las palabras, y por cada una de ellas si encontro solucion o no, y si
    tiene solucion muestra las coordenadas y la direccion de la misma.
*/
void mostrarUniverso (Universo universo) {
    char arrows[8][8] = {"\u2B06", "\u2B08", "\u27A1", "\u2B0A", "\u2B07", "\u2B0B", "\u2B05", "\u2B09"};

    int y;
    for (y = 0; y < universo.tamanioUniverso; y++) {
        if (universo.palabras[y].solucion) {
            printf("%s\u2713 %s", COLOR_VERDE, COLOR_DEFECTO);
        } else {
            printf("%s\u274C %s", COLOR_ROJO, COLOR_DEFECTO);
        }

        printf("%s", universo.palabras[y].palabra);

        if (universo.palabras[y].solucion) {
            printf(" (x: %d y: %d) %s", (universo.palabras[y].solucion)->x, (universo.palabras[y].solucion)->y, arrows[(universo.palabras[y].solucion)->direccion]);
        }

        printf("\n");
    }
}

// Funcion main.
int main (int argc, char const *argv[]) {
    limpiarPantalla();
    SopaDeLetras sopaDeLetras   = leerSopaDeLetras();

    limpiarPantalla();
    Universo universo           = leerUniverso();

    universo                    = resolverSopaDeLetras (sopaDeLetras, universo);

    limpiarPantalla();
    mostrarUniverso(universo);

    return 0;
}
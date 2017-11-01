#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constantes de colores para terminales ANSI
#define ANSI_COLOR_ROJO     "\x1b[31m"
#define ANSI_COLOR_VERDE    "\x1b[32m"
#define ANSI_COLOR_DEFECTO  "\x1b[0m"

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

/*
    limpiarPantalla: () -> void

    Limpia la pantalla de la terminal con una llamada al sistema.
*/
void limpiarPantalla () {
    system("clear");
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

    for (int y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        valid = 0;

        while (valid == 0) {
            printf("Ingrese la fila %d: ", y + 1);
            scanf("%s", buffer);

            if (strlen(buffer) == sopaDeLetras.numeroDeColumnas) {
                valid = 1;
            } else {
                printf("%s¡NO TIENE LA CANTIDAD DE COLUMANS INGRESADAS!%s\n", ANSI_COLOR_ROJO, ANSI_COLOR_DEFECTO);
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

    for (int x = 0; x < universo.tamanioUniverso; x++) {
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

    for (int y = 0; y < universo.tamanioUniverso; y++) {
        if (universo.palabras[y].solucion) {
            printf("%s\u2713 %s", ANSI_COLOR_VERDE, ANSI_COLOR_DEFECTO);
        } else {
            printf("%s\u274C %s", ANSI_COLOR_ROJO, ANSI_COLOR_DEFECTO);
        }

        printf("%s", universo.palabras[y].palabra);

        if (universo.palabras[y].solucion) {
            printf(" (x: %d y: %d) %s", (universo.palabras[y].solucion)->x, (universo.palabras[y].solucion)->y, arrows[(universo.palabras[y].solucion)->direccion]);
        }

        printf("\n");
    }
}

/*
    esDireccion: (SopaDeLetras, char*, int, int, int) -> int

    Busca en la SopaDeLetras, la palabra, en la posicion y direccion ingresadas y retorna 0 si
    no la encuentra o 1 si la encuentra.
*/
int esDireccion (SopaDeLetras sopaDeLetras, char *palabra, int x, int y, int direccion) {
    int xOffset[8]  = {0, 1, 1, 1, 0, -1, -1, -1},
        yOffset[8]  = {-1, -1, 0, 1, 1, 1, 0, -1},
        strLen      = strlen(palabra);

    if (strLen == 0) {
        return 1;
    }

    if (tolower(sopaDeLetras.celdas[y][x]) != tolower(palabra[0])) {
        return 0;
    }

    if (0 > x + (strLen - 1) * xOffset[direccion] ||
        sopaDeLetras.numeroDeColumnas <= x + (strLen - 1) * xOffset[direccion] ||
        0 > y + (strLen - 1) * yOffset[direccion] ||
        sopaDeLetras.numeroDeFilas <= y + (strLen - 1) * yOffset[direccion]) {
        return 0;
    }

    return esDireccion(sopaDeLetras, palabra + 1, x + xOffset[direccion], y + yOffset[direccion], direccion);
}

/*
    encontrarDireccion: (SopaDeLetras, char*, int, int) -> int

    Busca en la SopaDeLetras, la palabra, en la posicion ingresada y retorna la direccion, siendo
    0 arriba hasta 7 en sentido horario.    
*/
int encontrarDireccion (SopaDeLetras sopaDeLetras, char *palabra, int x, int y) {
    int strLen = strlen(palabra);

    for (int l = 0; l < strLen; l++) {
        for (int d = 0; d < 8; d++) {
            if (esDireccion(sopaDeLetras, palabra, x, y, d)) {
                return d;
            }
        }
    }

    return -1;
}

/*
    encontrarSolucion: (SopaDeLetras, char*) -> Solucion*

    Busca en la SopaDeLetras, la palabra ingresada y devuelve un puntero con la solucion (NULL
    si no hay solucion).    
*/
Solucion *encontrarSolucion (SopaDeLetras sopaDeLetras, char *palabra) {
    int direccion;
    Solucion *solucion;

    for (int y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        for (int x = 0; x < sopaDeLetras.numeroDeColumnas; x++) {
            direccion = encontrarDireccion (sopaDeLetras, palabra, x, y);

            if (direccion >= 0 && direccion <= 7) {
                solucion = malloc(sizeof(Solucion));

                solucion->x = x;
                solucion->y = y;
                solucion->direccion = direccion;

                return solucion;
            }
        }
    }

    return NULL;
}

/*
    encontrarSolucion: (SopaDeLetras, Universo) -> Universo

    Busca en la SopaDeLetras, el universo de palabras y retorna un universo con soluciones.    
*/
Universo resolverSopaDeLetras (SopaDeLetras sopaDeLetras, Universo universo) {
    for (int z = 0; z < universo.tamanioUniverso; z++) {
        universo.palabras[z].solucion = encontrarSolucion(sopaDeLetras, universo.palabras[z].palabra);
    }

    return universo;
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <limits.h>

typedef struct {
    int x;
    int y;
    int direccion;
} Solucion;

typedef struct {
    char *palabra;
    Solucion *solucion;
} Palabra;

typedef struct Universo {
    int tamanioUniverso;
    Palabra *palabras;
} Universo;

typedef struct {
    int numeroDeColumnas;
    int numeroDeFilas;
    char **celdas;
} SopaDeLetras;

void limpiarPantalla () {
    system("clear");
}



SopaDeLetras abrirSopaDeLetras (char *path) {
    int strLen;
    char str[100];
    SopaDeLetras sopaDeLetras;
    char **ptr;

    sopaDeLetras.numeroDeColumnas = INT_MAX;
    sopaDeLetras.numeroDeFilas = 0;
    sopaDeLetras.celdas = NULL;

    FILE * file;
    file = fopen(path , "r");

    if (file) {
        while ((fscanf(file, "%s", str)) != EOF) {
            // Realloc the first dimension
            ptr = realloc(sopaDeLetras.celdas, (sopaDeLetras.numeroDeFilas + 1) * sizeof(char *));

            if (ptr) {
                sopaDeLetras.celdas = ptr;

                // Save row
                strLen = strlen(str);

                if (sopaDeLetras.numeroDeColumnas > strLen) {
                    sopaDeLetras.numeroDeColumnas = strLen;
                }

                sopaDeLetras.celdas[sopaDeLetras.numeroDeFilas] = malloc((sopaDeLetras.numeroDeColumnas + 1) * sizeof(char));
                
                strcpy(sopaDeLetras.celdas[sopaDeLetras.numeroDeFilas], str);
                sopaDeLetras.numeroDeFilas++;
            }
        }

        fclose(file);
    }

    return sopaDeLetras;
}

struct Universo abrirUniverso (char *path) {
    int strLen;
    char str[100];
    Universo universo;
    Palabra *ptr;

    universo.tamanioUniverso = 0;
    universo.palabras = NULL;

    FILE * file;
    file = fopen(path , "r");

    if (file) {
        while ((fscanf(file, "%s", str)) != EOF) {
            // Realloc the 'array'
            ptr = realloc(universo.palabras, (universo.tamanioUniverso + 1) * sizeof(Palabra));

            if (ptr) {
                universo.palabras = ptr;

                universo.palabras[universo.tamanioUniverso].palabra = malloc((strlen(str) + 1) * sizeof(char));
                strcpy(universo.palabras[universo.tamanioUniverso].palabra, str);

                universo.palabras[universo.tamanioUniverso].solucion = NULL;

                universo.tamanioUniverso++;
            }
        }

        fclose(file);
    }

    return universo;
}




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
                printf("¡NO TIENE LA CANTIDAD DE COLUMANS INGRESADAS!\n");
            }
        }

        sopaDeLetras.celdas[y] = malloc((sopaDeLetras.numeroDeColumnas + 1) * sizeof(char));
        strcpy(sopaDeLetras.celdas[y], buffer);
    }

    return sopaDeLetras;
}

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

void mostrarUniverso (Universo universo) {
    char arrows[8][8] = {"\u2B06", "\u2B08", "\u27A1", "\u2B0A", "\u2B07", "\u2B0B", "\u2B05", "\u2B09"};

    for (int y = 0; y < universo.tamanioUniverso; y++) {
        if (universo.palabras[y].solucion) {
            printf("\u2713 ");
        } else {
            printf("\u274C ");
        }

        printf("%s", universo.palabras[y].palabra);

        if (universo.palabras[y].solucion) {
            printf(" (x: %d y: %d) %s", (universo.palabras[y].solucion)->x, (universo.palabras[y].solucion)->y, arrows[(universo.palabras[y].solucion)->direccion]);
        }

        printf("\n");
    }
}

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

Universo resolverSopaDeLetras (SopaDeLetras sopaDeLetras, Universo universo) {
    for (int z = 0; z < universo.tamanioUniverso; z++) {
        universo.palabras[z].solucion = encontrarSolucion(sopaDeLetras, universo.palabras[z].palabra);
    }

    return universo;
}

int main (int argc, char const *argv[]) {
    /*limpiarPantalla();
    SopaDeLetras sopaDeLetras   = leerSopaDeLetras();

    limpiarPantalla();
    Universo universo           = leerUniverso();*/

    SopaDeLetras sopaDeLetras = abrirSopaDeLetras("sopaDeLetras.txt");
    Universo universo = abrirUniverso("universo.txt");

    universo                    = resolverSopaDeLetras (sopaDeLetras, universo);

    limpiarPantalla();
    mostrarUniverso(universo);

    return 0;
}
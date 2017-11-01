#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

}

int encontrarDireccion (SopaDeLetras sopaDeLetras, char *palabra, int x, int y) {
    int strLen = strlen(palabra);

    for (int l = 0; l < strLen; l++) {
        for (int d = 0; d < 8; d++) {
            
        }
    }
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
    SopaDeLetras sopaDeLetras   = leerSopaDeLetras();*/

    limpiarPantalla();
    Universo universo           = leerUniverso();

    for (int x = 0; x < universo.tamanioUniverso; x++) {
        printf("%s\n", universo.palabras[x].palabra);
    }

    /*universo                    = resolverSopaDeLetras (sopaDeLetras, universo);

    limpiarPantalla();
    mostrarUniverso(universo);*/

    return 0;
}
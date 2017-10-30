#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Solucion {
    int x;
    int y;
    int direccion;
};

struct Palabra {
    char *palabra;
    struct Solucion *solucion;
};

struct Universo {
    int tamanioUniverso;
    struct Palabra *palabras;
};

struct SopaDeLetras {
    int numeroDeColumnas;
    int numeroDeFilas;
    char **celda;
};

void limpiarPantalla () {
    system("clear");
}

struct SopaDeLetras leerSopaDeLetras () {
    
}

struct Universo leerUniverso () {
    
}

void mostrarUniverso (struct Universo universo) {

}

int encontrarDireccion (struct SopaDeLetras sopaDeLetras, char *palabra, int x, int y) {
    int strLen = strlen(palabra);

    for (int l = 0; l < strLen; l++) {
        for (int d = 0; d < 8; d++) {
            
        }
    }
}

struct Solucion encontrarSolucion (struct SopaDeLetras sopaDeLetras, char *palabra) {
    int direccion;
    struct Solucion *solucion;

    for (int y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        for (int x = 0; x < sopaDeLetras.numeroDeColumnas; x++) {
            direccion = encontrarDireccion (sopaDeLetras, palabra);

            if (direccion >= 0 && direccion <= 7) {
                solucion = malloc(sizeof(struct Solucion));

                solucion->x = x;
                solucion->y = y;
                solucion->direccion = direccion;

                return solucion;
            }
        }
    }

    return NULL;
}

struct Universo resolverSopaDeLetras (struct SopaDeLetras sopaDeLetras, struct Universo universo) {
    for (int z = 0; z < sopaDeLetras.tamanioUniverso; z++) {
        sopaDeLetras.palabras[z].solucion = encontrarSolucion(sopaDeLetras, sopaDeLetras.palabras[z].palabra);
    }

    return sopaDeLetras;
}

int main (int argc, char const *argv[]) {
    SopaDeLetras sopaDeLetras   = leerSopaDeLetras();
    Universo universo           = leerUniverso();

    universo                    = resolverSopaDeLetras (sopaDeLetras, universo);

    mostrarUniverso(universo);

    return 0;
}
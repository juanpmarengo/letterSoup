#include "letterSoup.h"

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

    if (
        direccion < 0 || direccion > 7 ||
        0 > x || sopaDeLetras.numeroDeColumnas <= x ||
        0 > y || sopaDeLetras.numeroDeFilas <= y ||
        tolower(sopaDeLetras.celdas[y][x]) != tolower(palabra[0])) {
        return 0;
    }

    return esDireccion(sopaDeLetras, palabra + 1, x + xOffset[direccion], y + yOffset[direccion], direccion);
}

/*
    encontrarDireccion: (SopaDeLetras, char*, int, int) -> int

    Busca en la SopaDeLetras, la palabra, en la posicion ingresada y retorna la direccion, siendo
    0 hacia arriba hasta 7 en sentido horario. Si no la encuentra retorna -1
*/
int encontrarDireccion (SopaDeLetras sopaDeLetras, char *palabra, int x, int y) {
    int d;
    for (d = 0; d < 8; d++) {
        if (esDireccion(sopaDeLetras, palabra, x, y, d)) {
            return d;
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

    int y, x;
    for (y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        for (x = 0; x < sopaDeLetras.numeroDeColumnas; x++) {
            direccion = encontrarDireccion (sopaDeLetras, palabra, x, y);

            if (direccion != -1) {
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
    int z;
    for (z = 0; z < universo.tamanioUniverso; z++) {
        universo.palabras[z].solucion = encontrarSolucion(sopaDeLetras, universo.palabras[z].palabra);
    }

    return universo;
}
#include "letterSoup.h"

/*
    esDireccion: (SopaDeLetras, char*, int, int, int) -> int

    esDireccion toma una sopa, una palabra, dos enteros que representan coordenadas (x,y),
    y un entero que representa la dirección (en un sistema radial de 8 direcciones, siendo
    0 hacia arriba, orientado en sentido horario), y se llama recursivamente hasta finalmente
    devolver 1 si ha encontrado la palabra originada en (x,y) orientada en la dirección dada,
    o 0 en caso de no encontrarla.

    -Ejemplos-
    Entrada: Sopa= "qwe" , palabra= "sc", x= 1, y= 1, direccion= 3
                   "asd"
                   "zxc"
        Salida: 1

    Entrada: Sopa= "qwe" , palabra= "cxz", x= 2 y= 2, direccion= 0
                   "asd"
                   "zxc"
        Salida: 0

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
        tolower(sopaDeLetras.celdas[y][x]) != tolower(palabra[0])
    ) {
        return 0;
    }

    return esDireccion(sopaDeLetras, palabra + 1, x + xOffset[direccion], y + yOffset[direccion], direccion);
}

/*
    encontrarDireccion: (SopaDeLetras, char*, int, int) -> int
    
    encontrarDireccion toma una sopa, una palabra, y dos enteros que representan coordenadas (x,y),
    y devuelve un entero que representa la direccion (en el mismo sistema utilizado por esDireccion).
    En caso de no encontrar direccion, devuelve -1.
    
    -Ejemplos-
    Entrada: Sopa= "qwe" , palabra= "sc", x= 1, y= 1
                   "asd"
                   "zxc"
        Salida: 3

    Entrada: Sopa= "qwe" , palabra= "cxz", x= 0 y= 2
                   "asd"
                   "zxc"
        Salida: 6

    Entrada: Sopa= "qwe" , palabra= "cq", x= 0 y= 2
                   "asd"
                   "zxc"
        Salida: -1

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

    encontrarSolucion toma una sopa y una palabra, y de vuelve un puntero a la estructura solucion
    (NULL en caso de no existir solucion).

    -Ejemplos-
    Entrada: Sopa= "qwe" , palabra= "sc"
                   "asd"
                   "zxc"
        Salida: Solucion { x=1 , y=1 , direccion=3 }

    Entrada: Sopa= "qwe" , palabra= "zxc"
                   "asd"
                   "zxc"
        Salida: Solucion { x=0 , y=2 , direccion=2 }

    Entrada: Sopa= "qwe" , palabra= "daq"
                   "asd"
                   "zxc"
        Salida: NULL
*/
Solucion *encontrarSolucion (SopaDeLetras sopaDeLetras, char *palabra) {
    int direccion, y, x;
    Solucion *solucion;

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

    Busca el universo de palabras en la sopa, y retorna un universo con las mismas palabras y sus respectivas soluciones.

    -Ejemplo-
    Entrada: Sopa= "qwe" , Universo= {tamanioUniverso=3 , palabras={ {"sc",NULL}, {"zxc",NULL}, {"daq",NULL} } }
                   "asd"
                   "zxc"
        Salida: Universo= {tamanioUniverso=3 , palabras={ {"sc",Solucion { x=1 , y=1 , direccion=3 }}, {"zxc",Solucion { x=0 , y=2 , direccion=2 }}, {"daq",NULL} } }
*/
Universo resolverSopaDeLetras (SopaDeLetras sopaDeLetras, Universo universo) {
    int z;

    for (z = 0; z < universo.tamanioUniverso; z++) {
        universo.palabras[z].solucion = encontrarSolucion(sopaDeLetras, universo.palabras[z].palabra);
    }

    return universo;
}
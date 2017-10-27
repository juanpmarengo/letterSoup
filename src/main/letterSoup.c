#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/ioctl.h>
#include <unistd.h>

/******************/
/*   CONSTANTES   */
/******************/

#define TIME_TO_SLEEP       0.002

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RESET    "\x1b[0m"

/*******************/
/*   ESTRUCTURAS   */
/*******************/

// Representa un coeficiente de la sopa de letra, con la letra en si y el color que quiero renderizar por pantalla
struct Celda {
    char letra;
    char color[10]; // Los colores a usar estan definidos arribas como costantes, no necesito mas de 10 
};

// Represnta la sopa de letras por ancho y alto y la 'array bidimensional' de Celdas
struct SopaDeLetras {
    int numeroDeColumnas;
    int numeroDeFilas;
    struct Celda **celdas;
};

// Representa una solucion con la posicion de la primer letra y su sentido
struct Solucion {
    char columna;
    int fila;
    int sentido;
};

// Represento la palabra a buscar por la palabra en si y un puntero a su solucion (cuando es null no tiene solucion)
struct Palabra {
    char *palabra;
    struct Solucion *solucion;
};

// Es una estructura con la cantidad de palabras ingresadas y una lista de la estructura palabras
struct Universo {
    int tamanioUniverso;
    struct Palabra *palabras;
};

/*****************/
/*   FUNCIONES   */
/*****************/

// Va a la posicion de pantalla indicada, tambien esta en ingles, no voy a llamar a irAXY...
void goToXY (int x, int y) {
    printf("%c[%d;%df", 0x1B, y + 1, x + 1);
}

void goToXYPrint (int x, int y, char *str) {
    goToXY(x, y);
    printf("%s", str);
    fflush(stdout);
}

// Limpiar pantalla me parece absolutamente asqueroso
void clearScreen () {
    system("clear");
}

// Dormir segundos tmb
void sleepSeconds (double seconds) {
    usleep(seconds * 1000000);
}

void hideCursor () {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    goToXY(w.ws_col, w.ws_row);
}

void dibujarVentana () {
    clearScreen();

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    for (int y = 0; y < w.ws_row; y++) {
        for (int x = 0; x < w.ws_col; x++) {
            if (x == 0) {
                if (y == 0) {
                    goToXYPrint(x, y, "\u2554");
                } else if (y == 2) {
                    goToXYPrint(x, y, "\u2560");
                } else if (y == w.ws_row - 1) {
                    goToXYPrint(x, y, "\u255A");
                } else {
                    goToXYPrint(x, y, "\u2551");
                }
            } else if (x == w.ws_col - 1) {
                if (y == 0) {
                    goToXYPrint(x, y, "\u2557");
                } else if (y == 2) {
                    goToXYPrint(x, y, "\u2563");
                } else if (y == w.ws_row - 1) {
                    goToXYPrint(x, y, "\u255D");
                } else {
                    goToXYPrint(x, y, "\u2551");
                }
            } else if (y == 0 || y == 2 || y == w.ws_row - 1) {
                goToXYPrint(x, y, "\u2550");
            }
        }
    }

    fflush(stdout);
}

void dibujarGrilla (int columnas, int filas) {
    for (int y = 0; y < filas + 4; y++) {
        for (int x = 0; x < columnas + 5; x++) {
            if (x == 0) {
                if (y == 0) {
                    goToXYPrint(x + 1, y + 3, "\u250F");
                } else if (y == 2) {
                    goToXYPrint(x + 1, y + 3, "\u2520");
                } else if (y == filas + 3) {
                    goToXYPrint(x + 1, y + 3, "\u2517");
                } else {
                    goToXYPrint(x + 1, y + 3, "\u2503");
                }
            } else if (x == 1) {
                if (y == 0) {
                    goToXYPrint(x + 1, y + 3, "\u2501\u2501");
                } else if (y == 1) {
                    goToXYPrint(x + 1, y + 3, "  ");
                } else if (y == 2) {
                    goToXYPrint(x + 1, y + 3, "\u2500\u2500");
                } else if (y == filas + 3) {
                    goToXYPrint(x + 1, y + 3, "\u2501\u2501");
                } else {
                    if (y - 2 < 10) {
                        goToXY(x + 1, y + 3);
                        printf(" %d", y - 2);
                    } else {
                        goToXY(x + 1, y + 3);
                        printf("%d", y - 2);
                    }
                }

                x++;
            } else if (x == 3) {
                if (y == 0) {
                    goToXYPrint(x + 1, y + 3, "\u252F");
                } else if (y == 2) {
                    goToXYPrint(x + 1, y + 3, "\u253C");
                } else if (y == filas + 3) {
                    goToXYPrint(x + 1, y + 3, "\u2537");
                } else {
                    goToXYPrint(x + 1, y + 3, "\u2502");
                }
            } else if (x == columnas + 4) {
                if (y == 0) {
                    goToXYPrint(x + 1, y + 3, "\u2513");
                } else if (y == 2) {
                    goToXYPrint(x + 1, y + 3, "\u2528");
                } else if (y == filas + 3) {
                    goToXYPrint(x + 1, y + 3, "\u251B");
                } else {
                    goToXYPrint(x + 1, y + 3, "\u2503");
                }
            } else {
                if (y == 0 || y == filas + 3) {
                    goToXYPrint(x + 1, y + 3, "\u2501");
                } else if (y == 2) {
                    goToXYPrint(x + 1, y + 3, "\u2500");
                } else if (y == 1) {
                    goToXY(x + 1, y + 3);
                    printf("%c", x + 61);
                }
            }
        }
    }

    fflush(stdout);
}

void dibujarTabla (int filas, int margenIzquierdo) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    for (int y = 0; y < filas + 4; y++) {
        for (int x = 0; x < w.ws_col - margenIzquierdo - 1; x++) {
            if (x == 0) {
                if (y == 0) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u250F");
                } else if (y == 2) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2520");
                } else if (y == filas + 3) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2517");
                } else {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2503");
                }
            } else if (x == w.ws_col - margenIzquierdo - 2) {
                if (y == 0) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2513");
                } else if (y == 2) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2528");
                } else if (y == filas + 3) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u251B");
                } else {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2503");
                }
            } else if (x == w.ws_col - margenIzquierdo - 7) {
                if (y == 0) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2501");
                } else if (y == 1) {
                    goToXYPrint(x + margenIzquierdo, y + 3, " ");
                } else if (y == 2) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u252C");
                } else if (y == filas + 3) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2537");
                } else {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2502");
                }
            } else {
                if (y == 0 || y == filas + 3) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2501");
                } else if (y == 2) {
                    goToXYPrint(x + margenIzquierdo, y + 3, "\u2500");
                }
            }
        }

        goToXYPrint(margenIzquierdo + 1, 4, "PALABRAS");
    }

    fflush(stdout);
}

void mostrarSalidaVentana (char *str) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    goToXY(1, 1);

    for (int x = 0; x < w.ws_col - 2; x++) {
        printf(" ");
    }

    goToXY(1, 1);

    printf("%s", str);

    fflush(stdout);
}

void mostrarSopaDeLetras (struct SopaDeLetras sopaDeLetras) {
    for (int y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        goToXY(5, y + 6);

        for (int x = 0; x < sopaDeLetras.numeroDeColumnas; x++) {
            printf("%s%c%s", sopaDeLetras.celdas[y][x].color, sopaDeLetras.celdas[y][x].letra, ANSI_COLOR_RESET);
        }
    }

    fflush(stdout);
}

struct SopaDeLetras abrirSopaDeLetras (char *path) {
    int strLen;
    char str[100];
    struct SopaDeLetras sopaDeLetras;
    struct Celda  **ptr;

    sopaDeLetras.numeroDeColumnas = INT_MAX;
    sopaDeLetras.numeroDeFilas = 0;
    sopaDeLetras.celdas = NULL;

    FILE * file;
    file = fopen(path , "r");

    if (file) {
        while ((fscanf(file, "%s", str)) != EOF) {
            // Realloc the first dimension
            ptr = realloc(sopaDeLetras.celdas, (sopaDeLetras.numeroDeFilas + 1) * sizeof(struct Celda*));

            if (ptr) {
                sopaDeLetras.celdas = ptr;

                // Save row
                strLen = strlen(str);

                if (sopaDeLetras.numeroDeColumnas > strLen) {
                    sopaDeLetras.numeroDeColumnas = strLen;
                }

                sopaDeLetras.celdas[sopaDeLetras.numeroDeFilas] = malloc(sopaDeLetras.numeroDeColumnas * sizeof(struct Celda));
                
                for (int c = 0; c < sopaDeLetras.numeroDeColumnas; c++) {
                    sopaDeLetras.celdas[sopaDeLetras.numeroDeFilas][c].letra = str[c];
                    strcpy(sopaDeLetras.celdas[sopaDeLetras.numeroDeFilas][c].color, ANSI_COLOR_RESET); // Le seteo el color por default
                }
                
                sopaDeLetras.numeroDeFilas++;
            }
        }

        fclose(file);
    }

    dibujarGrilla(sopaDeLetras.numeroDeColumnas, sopaDeLetras.numeroDeFilas);
    mostrarSopaDeLetras(sopaDeLetras);

    return sopaDeLetras;
}

struct SopaDeLetras leerSopaDeLetras () {
    struct SopaDeLetras sopaDeLetras;

    mostrarSalidaVentana("Cantidad de columnas? ");
    scanf("%d", &(sopaDeLetras.numeroDeColumnas));

    mostrarSalidaVentana("Cantidad de filas? ");
    scanf("%d", &(sopaDeLetras.numeroDeFilas));

    dibujarGrilla(sopaDeLetras.numeroDeColumnas, sopaDeLetras.numeroDeFilas);

    sopaDeLetras.celdas = malloc(sopaDeLetras.numeroDeFilas * sizeof(struct Celda *));

    mostrarSalidaVentana("Ingrese las letras:");
    getchar(); //Para eliminar la basura del buffer

    for (int y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        sopaDeLetras.celdas[y] = malloc(sopaDeLetras.numeroDeColumnas * sizeof(struct Celda));

        goToXY(5, y + 6);

        for (int x = 0; x < sopaDeLetras.numeroDeColumnas; x++) {
            system("/bin/stty raw"); // Causa que no sea neceario un final de linea para que el buffer le devuelva el char a getchar, lo que si no te deja hacer ctrl c
            sopaDeLetras.celdas[y][x].letra = getchar();
            system("/bin/stty cooked"); // Saca lo anterior

            strcpy(sopaDeLetras.celdas[y][x].color, ANSI_COLOR_RESET); // Le seteo el color por default
        }
    }

    return sopaDeLetras;
}

void mostrarUniverso (struct Universo universo, int margenIzquierdo) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    for (int y = 0; y < universo.tamanioUniverso; y++) {
        goToXY(margenIzquierdo + 1, y + 6);

        printf("%s", universo.palabras[y].palabra);

        goToXY(w.ws_col - 6, y + 6);

        if (universo.palabras[y].solucion) {
            if (universo.palabras[y].solucion->sentido == 8) {
                printf("%s  \u274C%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            } else {
                printf("%c", universo.palabras[y].solucion->columna + 65);

                if (universo.palabras[y].solucion->fila + 1 < 10) {
                    printf(" %d", universo.palabras[y].solucion->fila + 1);
                } else {
                    printf("%d", universo.palabras[y].solucion->fila + 1);
                }

                switch (universo.palabras[y].solucion->sentido) {
                    case 0:
                        printf("\u2B06");
                        break;
                    case 1:
                        printf("\u2B08");
                        break;
                    case 2:
                        printf("\u27A1");
                        break;
                    case 3:
                        printf("\u2B0A");
                        break;
                    case 4:
                        printf("\u2B07");
                        break;
                    case 5:
                        printf("\u2B0B");
                        break;
                    case 6:
                        printf("\u2B05");
                        break;
                    case 7:
                        printf("\u2B09");
                        break;
                }
            }
        } else {
            printf("%s  \u231B%s", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        }
    }

    fflush(stdout);
}

struct Universo abrirUniverso (char *path, int margenIzquierdo) {
    int strLen;
    char str[100];
    struct Universo universo;
    struct Palabra *ptr;

    universo.tamanioUniverso = 0;
    universo.palabras = NULL;

    FILE * file;
    file = fopen(path , "r");

    if (file) {
        while ((fscanf(file, "%s", str)) != EOF) {
            // Realloc the 'array'
            ptr = realloc(universo.palabras, (universo.tamanioUniverso + 1) * sizeof(struct Palabra));

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

    dibujarTabla(universo.tamanioUniverso, margenIzquierdo);
    mostrarUniverso(universo, margenIzquierdo);

    return universo;
}

struct Universo leerUniverso (int margenIzquierdo) {
    struct Universo universo;
    char buffer[40];

    mostrarSalidaVentana("Cantidad de palabras? ");
    scanf("%d", &(universo.tamanioUniverso));

    dibujarTabla(universo.tamanioUniverso, margenIzquierdo);

    universo.palabras = malloc(universo.tamanioUniverso * sizeof(struct Palabra));

    mostrarSalidaVentana("Ingrese las palabras:");

    for (int p = 0; p < universo.tamanioUniverso; p++) {
        goToXY(margenIzquierdo + 1, p + 6);

        scanf("%s", buffer);

        universo.palabras[p].palabra = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(universo.palabras[p].palabra, buffer);

        universo.palabras[p].solucion = NULL; // Sin solucion
    }

    mostrarUniverso(universo, margenIzquierdo);

    return universo;
}

int buscarDireccion (struct SopaDeLetras *sopaDeLetras, int x, int y, char *palabra, int direccion, int posicion, double tiempoDePausa) {
    int resultado = 0;

    if (posicion == strlen(palabra)) {
        return 1;
    }

    switch (direccion) {
        case 0:
            y -= 1;

            break;
        case 1:
            y -= 1;
            x += 1;

            break;
        case 2:
            x += 1;

            break;
        case 3:
            x += 1;
            y += 1;

            break;
        case 4:
            y += 1;

            break;
        case 5:
            y += 1;
            x -= 1;

            break;
        case 6:
            x -= 1;

            break;
        case 7:
            y -= 1;
            x -= 1;

            break;
    }

    if (tolower(sopaDeLetras->celdas[y][x].letra) == tolower(palabra[posicion])) {
        goToXY(x + 5, y + 6);
        printf("%s%c%s", ANSI_COLOR_YELLOW, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
        fflush(stdout);

        sleepSeconds(tiempoDePausa);

        resultado = buscarDireccion(sopaDeLetras, x, y, palabra, direccion, (posicion + 1), tiempoDePausa);

        if (resultado == 1) {
            strcpy(sopaDeLetras->celdas[y][x].color, ANSI_COLOR_GREEN);
        }
    } else {
        goToXY(x + 5, y + 6);
        printf("%s%c%s", ANSI_COLOR_RED, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
        fflush(stdout);

        sleepSeconds(tiempoDePausa);

        resultado = 0;
    }

    goToXY(x + 5, y + 6);
    printf("%s%c%s", sopaDeLetras->celdas[y][x].color, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
    fflush(stdout);

    return resultado;
}

int buscarDirecciones (struct SopaDeLetras *sopaDeLetras, int x, int y, char *palabra, double tiempoDePausa) {
    int posicion = strlen(palabra) - 1;

    for (int d = 0; d < 8; d++) {
        switch (d) {
            case 0:
                if (y - posicion >= 0) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
            case 1:
                if (y - posicion >= 0 && x + posicion < sopaDeLetras->numeroDeColumnas) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
            case 2:
                if (x + posicion < sopaDeLetras->numeroDeColumnas) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
            case 3:
                if (y + posicion < sopaDeLetras->numeroDeFilas && x + posicion < sopaDeLetras->numeroDeColumnas) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
            case 4:
                if (y + posicion < sopaDeLetras->numeroDeFilas) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
            case 5:
                if (y + posicion < sopaDeLetras->numeroDeFilas && x - posicion >= 0) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
            case 6:
                if (x - posicion >= 0) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
            case 7:
                if (y - posicion >= 0 && x - posicion >= 0) {
                    if (buscarDireccion(sopaDeLetras, x, y, palabra, d, 1, tiempoDePausa) == 1) {
                        return d;
                    }
                }

                break;
        }
    }

    return 8;
}

struct Palabra encontrarPalabra (struct SopaDeLetras *sopaDeLetras, struct Palabra palabra, double tiempoDePausa) {
    int direccion;

    palabra.solucion = malloc(sizeof (struct Solucion));
    palabra.solucion->sentido = 8;

    for (int y = 0; y < sopaDeLetras->numeroDeFilas; y++) {
        for (int x = 0; x < sopaDeLetras->numeroDeColumnas; x++) {
            if (tolower(sopaDeLetras->celdas[y][x].letra) == tolower(palabra.palabra[0])) {
                goToXY(x + 5, y + 6);
                printf("%s%c%s", ANSI_COLOR_YELLOW, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
                fflush(stdout);

                direccion = buscarDirecciones(sopaDeLetras, x, y, palabra.palabra, tiempoDePausa);

                if (direccion != 8) {
                    strcpy(sopaDeLetras->celdas[y][x].color, ANSI_COLOR_GREEN);

                    palabra.solucion->sentido = direccion;
                    palabra.solucion->columna = x;
                    palabra.solucion->fila = y;

                    goToXY(x + 5, y + 6);
                    printf("%s%c%s", sopaDeLetras->celdas[y][x].color, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
                    fflush(stdout);

                    sleepSeconds(tiempoDePausa);

                    return palabra;
                } else {
                    goToXY(x + 5, y + 6);
                    printf("%s%c%s", ANSI_COLOR_RED, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
                    
                    sleepSeconds(tiempoDePausa);

                    goToXY(x + 5, y + 6);
                    printf("%s%c%s", sopaDeLetras->celdas[y][x].color, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
                    fflush(stdout);
                }
            } else {
                goToXY(x + 5, y + 6);
                printf("%s%c%s", ANSI_COLOR_RED, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
                fflush(stdout);
                
                sleepSeconds(tiempoDePausa);

                goToXY(x + 5, y + 6);
                printf("%s%c%s", sopaDeLetras->celdas[y][x].color, sopaDeLetras->celdas[y][x].letra, ANSI_COLOR_RESET);
                fflush(stdout);
            }
        }
    }

    return palabra;
}

void encontrarSoluciones (struct SopaDeLetras sopaDeLetras, struct Universo universo, double tiempoDePausa) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    for (int y = 0; y < universo.tamanioUniverso; y++) {
        goToXY(w.ws_col - 6, y + 6);
        printf("%s  \u21B7%s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);

        universo.palabras[y] = encontrarPalabra(&sopaDeLetras, universo.palabras[y], tiempoDePausa);

        mostrarUniverso(universo, sopaDeLetras.numeroDeColumnas + 6);
    }
}

/************/
/*   MAIN   */
/************/

int main () {    
    char path[40] = {0};
    struct SopaDeLetras sopaDeLetras;
    struct Universo universo;

    dibujarVentana();

    /*mostrarSalidaVentana("SOPA DE LETRAS ubicacion de un archivo o ENTER para ingreso manual: ");

    fgets(path, 40, stdin);
    path[strlen(path) - 1] = '\0';

    if (strlen(path) > 0) {
        sopaDeLetras = abrirSopaDeLetras(path);
    } else {
        sopaDeLetras = leerSopaDeLetras();
    }

    mostrarSalidaVentana("UNIVERSO ubicacion de un archivo o ENTER para ingreso manual: ");

    fgets(path, 40, stdin);
    path[strlen(path) - 1] = '\0';

    if (strlen(path) > 0) {
        universo = abrirUniverso(path, sopaDeLetras.numeroDeColumnas + 6);
    } else {
        universo = leerUniverso(sopaDeLetras.numeroDeColumnas + 6);
    }*/

    sopaDeLetras = abrirSopaDeLetras("sopaDeLetras.txt");
    universo = abrirUniverso("universo.txt", sopaDeLetras.numeroDeColumnas + 6);

    encontrarSoluciones(sopaDeLetras, universo, TIME_TO_SLEEP);

    mostrarSalidaVentana("");

    return 0;
}
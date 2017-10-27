#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <unistd.h>

/******************/
/*   CONSTANTES   */
/******************/

#define TIME_TO_SLEEP       0.8

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
                    if (y - 11 < 10) {
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
}

void mostrarSalidaVentana (char *str) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    goToXY(1, 1);

    for (int x = 0; x < w.ws_row - 2; x++) {
        printf(" ");
    }

    goToXY(1, 1);

    printf("%s", str);
}

/*
int radialSearch (struct Grid grid, char *word, int x, int y, int direction, int offset) {
    int result;

    if (offset == strlen(word)) {
        return 1;
    }

    switch (direction) {
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

    if (grid.rows[y][x] == word[offset]) {
        goToXY(x, y);
        printf(ANSI_COLOR_YELLOW    "%c"     ANSI_COLOR_RESET "\n", grid.rows[y][x]);

        sleepSeconds(TIME_TO_SLEEP);

        result = radialSearch(grid, word, x, y, direction, (offset + 1));
        goToXY(x, y);

        if (result == 1) {
            printf(ANSI_COLOR_GREEN "%c"     ANSI_COLOR_RESET "\n", grid.rows[y][x]);
        } else {
            printf("%c\n", grid.rows[y][x]);
        }

        return result;
    } else {
        goToXY(x, y);
        printf(ANSI_COLOR_RED       "%c"     ANSI_COLOR_RESET "\n", grid.rows[y][x]);

        sleepSeconds(TIME_TO_SLEEP);
        goToXY(x, y);
        printf("%c\n", grid.rows[y][x]);

        return 0;
    }
}

int filterRadialSearch (struct Grid grid, char *word, int x, int y) {
    int offset = strlen(word) - 1;

    for (int r = 0; r < 8; r++) {
        switch (r) {
            case 0:
                if (y - offset >= 0) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
            case 1:
                if (y - offset >= 0 && x + offset < grid.width) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
            case 2:
                if (x + offset < grid.width) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
            case 3:
                if (y + offset < grid.height && x + offset < grid.width) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
            case 4:
                if (y + offset < grid.height) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
            case 5:
                if (y + offset < grid.height && x - offset >= 0) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
            case 6:
                if (x - offset >= 0) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
            case 7:
                if (y - offset >= 0 && x - offset >= 0) {
                    if (radialSearch(grid, word, x, y, r, 1) == 1) {
                        return r;
                    }
                }

                break;
        }
    }

    return 8;
}

void solveSoup (struct Grid grid, struct Dict dict) {
    clearScreen();
    printGrid(grid);

    for (int w = 0; w < dict.n; w++) {
        for (int y = 0; y < grid.height; y++) {
            for (int x = 0; x < grid.width; x++) {
                if (grid.rows[y][x] == dict.words[w][0]) {
                    goToXY(x, y);
                    printf(ANSI_COLOR_YELLOW  "%c"     ANSI_COLOR_RESET "\n", grid.rows[y][x]);

                    if(filterRadialSearch(grid, dict.words[w], x, y) != 8) {
                        goToXY(x, y);
                        printf(ANSI_COLOR_GREEN  "%c"     ANSI_COLOR_RESET "\n", grid.rows[y][x]);

                        getchar();
                    }
                } else {
                    goToXY(x, y);
                    printf(ANSI_COLOR_RED     "%c"     ANSI_COLOR_RESET "\n", grid.rows[y][x]);
                }

                sleepSeconds(TIME_TO_SLEEP);
                goToXY(x, y);
                printf("%c", grid.rows[y][x]);
            }
        }
    }
}
*/

void mostrarSopaDeLetras (struct SopaDeLetras sopaDeLetras) {
    for (int y = 0; y < sopaDeLetras.numeroDeFilas; y++) {
        goToXY(5, y + 6);

        for (int x = 0; x < sopaDeLetras.numeroDeColumnas; x++) {
            printf("%s%c%s", sopaDeLetras.celdas[y][x].color, sopaDeLetras.celdas[y][x].letra, ANSI_COLOR_RESET);
        }
    }
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
                printf("  \u2613");
            } else {
                printf("%c", universo.palabras[y].solucion->columna);

                if (universo.palabras[y].solucion->fila < 10) {
                    printf(" %d", universo.palabras[y].solucion->fila);
                } else {
                    printf("%d", universo.palabras[y].solucion->fila);
                }

                switch (universo.palabras[y].solucion->sentido) {
                    case 0:
                        printf("\u2B06\u2503");
                        break;
                    case 2:
                        printf("\u2B06\u2503");
                        break;
                    case 3:
                        printf("\u2B06\u2503");
                        break;
                    case 4:
                        printf("\u2B06\u2503");
                        break;
                    case 5:
                        printf("\u2B06\u2503");
                        break;
                    case 6:
                        printf("\u2B06\u2503");
                        break;
                    case 7:
                        printf("\u2B06\u2503");
                        break;
                    case 8:
                        printf("\u2B06\u2503");
                        break;
                }
            }
        } else {
            printf("  \u231B");
        }
    }
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

void encontrarSoluciones (struct SopaDeLetras sopaDeLetras, struct Universo universo) {

}

/************/
/*   MAIN   */
/************/

int main () {
    char path[40];
    struct SopaDeLetras sopaDeLetras;
    struct Universo universo;

    dibujarVentana();

    /*mostrarSalidaVentana("Ingrese la ubicacion de un archivo (en caso que no se ingrese ninguno se procedera al ingreso por pantalla): ");
    scanf("%s", path);

    if (strlen(path) > 0    ) {
        sopaDeLetras = abrirSopaDeLetras(path);
    } else {
        sopaDeLetras = leerSopaDeLetras();
    }*/

    sopaDeLetras = abrirSopaDeLetras("sopaDeLetras.txt");
    //sopaDeLetras = leerSopaDeLetras();

    universo = abrirUniverso("universo.txt", sopaDeLetras.numeroDeColumnas + 6);
    //universo = leerUniverso(sopaDeLetras.numeroDeColumnas + 6);

    encontrarSoluciones(sopaDeLetras, universo);

    mostrarSalidaVentana("");

    return 0;
}
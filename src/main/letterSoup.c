#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#define TIME_TO_SLEEP       0.008

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RESET    "\x1b[0m"

struct Grid {
    int width;
    int height;
    char **rows;
};

struct Dict {
    int n;
    char **words;
};

void goToXY (int x, int y) {
    printf("%c[%d;%df", 0x1B, y + 1, x + 1);
}

void clearScreen () {
    system("clear");
}

void sleepSeconds (double seconds) {
    usleep(seconds * 1000000);
}

struct Grid readGrid (char *path) {
    char str[100], **ptr;
    int strLen;
    struct Grid grid;

    grid.height = 0;
    grid.width = INT_MAX;
    grid.rows = NULL;

    FILE * file;
    file = fopen(path , "r");

    if (file) {
        while ((fscanf(file, "%s", str)) != EOF) {
            // Realloc the first dimension
            ptr = realloc(grid.rows, (grid.height + 1) * sizeof(char *));

            if (ptr) {
                grid.rows = ptr;
                grid.height++;

                // Save row
                strLen = strlen(str);

                if (grid.width > strLen) {
                    grid.width = strLen;
                }

                grid.rows[grid.height - 1] = malloc(grid.width * sizeof(char));
                strcpy(grid.rows[grid.height - 1], str);
            }
        }

        fclose(file);
    }

    return grid;
}

struct Dict readDict (char *path) {
    char str[100], **ptr;
    int strLen;
    struct Dict dict;

    dict.n = 0;
    dict.words = NULL;

    FILE * file;
    file = fopen(path , "r");

    if (file) {
        while ((fscanf(file, "%s", str)) != EOF) {
            // Realloc the 'array'
            ptr = realloc(dict.words, (dict.n + 1) * sizeof(char*));

            if (ptr) {
                dict.words = ptr;
                dict.n++;

                // Save the word
                strLen = strlen(str);

                dict.words[dict.n - 1] = malloc(strLen * sizeof(char));
                strcpy(dict.words[dict.n - 1], str);
            }
        }

        fclose(file);
    }

    return dict;
}

void printGrid (struct Grid grid) {
    goToXY(0, 0);

    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            printf("%c", grid.rows[y][x]);
        }

        printf("\n");
    }
}

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

int main () {
    struct Grid grid = readGrid("grid.txt");
    struct Dict dict = readDict("dict.txt");

    solveSoup(grid, dict);

    return 0;
}
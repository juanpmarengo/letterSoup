#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct Grid {
    int width;
    int height;
    char **rows;
};

void goToXY (int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void clearScreen () {
    system("clear");
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

int main () {
    struct Grid grid = readGrid("grid.txt");

    printf("%d\n", grid.width);
    printf("%d\n", grid.height);

    for (int x = 0; x < grid.height; x++) {
        printf("%s\n", grid.rows[x]);
    }

    return 0;
}
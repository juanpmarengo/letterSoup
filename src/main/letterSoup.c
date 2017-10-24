#include <stdio.h>
#include <stdlib.h>

void goToXY (int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void clearScreen () {
    system("clear");
}

int main () {
    clearScreen ();

    goToXY (4, 5);
    printf ("Hello world!\n");

    return 0;
}
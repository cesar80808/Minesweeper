#include "minesweeper.h"
#include"screen.h"
#include<stdio.h>

int main(void){

    titleScreen();

    char c;
    do{

        // Jugamos.
        Minesweeper();

        // Preguntamos si quiere jugar de nuevo.
        gotoxy(40, 13);
        setColor(I_CYAN);
        printf("DESEA JUGAR DE NUEVO?     Y/N");
        setColor(I_WHITE);
        gotoxy(40, 14);

        scanf(" %c", &c);

    } while (c == 'Y');

    clearScreen();

    return 0;
}

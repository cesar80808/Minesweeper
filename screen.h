#ifndef SCREEN_H
#define SCREEN_H

// Dimensiones de la pantalla
#define SCREEN_NC 80
#define SCREEN_NR 24

// Índices de colores
#define I_RED 1
#define I_GREEN 2
#define I_YELLOW 3
#define I_BLUE 4
#define I_MAGENTA 5
#define I_CYAN 6
#define I_WHITE 7

// Definimos colores adicionales
#define FOREGROUND_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA (FOREGROUND_BLUE | FOREGROUND_RED)
#define FOREGROUND_CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_WHITE (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN)

// Funciones
void gotoxy(int x, int y);
void clearScreen(void);
void setColor(int idxColor);

#endif
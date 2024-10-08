#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#define BLOCK 219   // caracter bloque "█" 
#define WIDTH 16    // largo del tablero
#define HEIGHT 8    // alto del tablero
#define N_MINES 10  // cantidad inicial de minas
#define H_PADDING 4 // margen horizontal
#define V_PADDING 2 // margen vertical

typedef char ESTADO;
#define SI_LIBERADO 1
#define NO_LIBERADO -1

typedef char MINA;
#define SI_MINA 1
#define NO_MINA -1

typedef char BANDERA;
#define SI_BANDERA 1
#define NO_BANDERA -1

typedef char CON_JUEGO;
#define JUGANDO 0
#define GANA 1
#define PIERDE -1

typedef char MODO;
#define MODO_MINA 1
#define MODO_BANDERA -1

typedef struct CELL{
    int minasVecinas;           // # de minas entre sus vecinos
    ESTADO estado;              // LIBERADO o NO LIBERADO
    MINA mina;                  // CON MINA o SIN MINA
    BANDERA bandera;            // CON BANDERA o SIN BANDERA
    int n_vecinos;              // cuantos vecinos tiene
    struct CELL* vecinos[8];    // arreglo de apuntadores a sus vecinos
} cell;

typedef struct {
    cell** mat;                     // matriz de celdas
    int celdasFaltantes;            // celdas sin mina que aun no han sido liberadas
    int banderasPuestas;            // # de banderas puestas
    int minasFaltantes;             // cantidad total de minas - banderas puestas
    CON_JUEGO condicionDeJuego;     // JUGANDO, PIERDE O GANA
    MODO modo;                      // MODO MINA o MODO BANDERA
} juego;

typedef struct {
    cell* cells[WIDTH * HEIGHT - N_MINES];
    int tope;
} pila;


void Minesweeper(void);
int getInput(char buffer[], char* inChar, int* inInt); 
void accionCelda(char inChar, int inInt, juego* J);
int vecinosConBandera(cell* cell1);

// Inicializar juego
int inicializarJuego(juego* miJuego);   // 1 si error, 0 si OK
cell** crearTablero(void);
void liberarTablero(cell** mat);
void inicializarVecinos(cell** mat);
void estadoInicialRandom(cell** mat);

// Imprimir en pantalla
void printTablero(cell** mat);
void printModo(MODO modo);
void printMinasFaltantes(int minasFaltantes);
void titleScreen(void);

// Funciones para liberar usando una pila
void liberaConPila(int x, int y, juego* J); 
void empilarVecinos(pila* miPila, cell* cell1);
void desempilar(pila* miPila, cell** cell1);
void empilarVecinos(pila* miPila, cell* cell1);
void empilar(pila* miPila, cell* cell1);

#endif
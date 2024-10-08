#include<stdio.h>
#include<stdlib.h>
#include "minesweeper.h"
#include "screen.h"
#include<time.h>

void Minesweeper(void){   

    juego J;
    int err = inicializarJuego(&J);

    if (err == 1){
        printf("\n\tERROR inicializando juego.");
        return;
    }

    int entradaValida = 1;
    // 1 si la entrada ingresada por el usuario es válida.
    // 0 si no fue válida.

    do {
        // Imprimimos juego.
        printTablero(J.mat);
        printModo(J.modo);
        printMinasFaltantes(J.minasFaltantes);

        // Indicación de ingresar input.
        gotoxy(H_PADDING + 1, V_PADDING + 3*HEIGHT + 1);
        setColor(I_CYAN);
        printf("Ingrese casilla o 'X' para cambiar de modo: " );
        setColor(I_WHITE);

        // Si la entrada fue inválida.
        if (entradaValida == 0){
            // Imprimir error.
            gotoxy(H_PADDING + 1, V_PADDING + 3*HEIGHT + 2);
            setColor(I_RED);
            printf("ENTRADA INVALIDA. Intente de nuevo.");
            setColor(I_WHITE);
        }

        // Leemos entrada en buffer.
        char buffer[100];
        gotoxy(H_PADDING + 50, V_PADDING + 3*HEIGHT + 1);
        scanf("%s", &buffer);
        
        // Para almacenar input.
        char inChar;
        int inInt;

        // Leemos y validamos entrada.
        entradaValida = getInput(buffer, &inChar, &inInt);

        if (entradaValida == 0)
            continue;
        
        // Cambiar modo
        if (inChar == 'X'){
            J.modo *= (-1);
            continue;
        }
            
        // En este punto, la entrada fue válida y no fue 'X'.
        accionCelda(inChar, inInt, &J);

        // Verificamos si aún no termina el juego.
        if(J.celdasFaltantes == 0)
            J.condicionDeJuego = GANA;
        
    } while(J.condicionDeJuego == JUGANDO);

    printTablero(J.mat);

    // Imprimimos resultados.
    gotoxy(H_PADDING + 5*WIDTH + 5, V_PADDING + 1);
    switch (J.condicionDeJuego){
    case GANA: 
        setColor(I_GREEN);
        printf("GANASTE :)");
        setColor(I_WHITE);
        break;
    
    case PIERDE: 
        setColor(I_RED);
        printf("PERDISTE :(");
        setColor(I_WHITE);
        break;

    }

    liberarTablero(J.mat); 

    gotoxy(H_PADDING + 5*WIDTH + 5, V_PADDING + 4);
    printf("Presiona [ENTER]");
    gotoxy(H_PADDING + 5*WIDTH + 5, V_PADDING + 5);
    printf("para continuar.");

    getchar(); getchar();   // Esperamos input del usuario.
    clearScreen();
}



cell** crearTablero(void){
    // Alocamos dinámicamente la matriz de celdas.
    cell** map = (cell **) malloc( WIDTH * sizeof(cell*) );
    if(!map)
        return NULL;
    
    for (int i = 0; i < WIDTH; i++){
        map[i] = (cell *) malloc( HEIGHT * sizeof(cell) );
        if(!map[i]){
            while (i > 0)
                free(map[--i]);
            free(map);
            return NULL;
        }
    }
    
    return map;
}

void estadoInicialRandom(cell** mat){
    // Inicializamos cada celda.    
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++){
            mat[i][j].bandera = NO_BANDERA; 
            mat[i][j].estado = NO_LIBERADO;
            mat[i][j].mina = NO_MINA;
            mat[i][j].minasVecinas = 0;
        }

    inicializarVecinos(mat);

    srand(time(0)); // rand seed.

    // Ponemos N_MINES minas aleatoriamente entre todas las celdas.
    int i = 1;
    do {
        // Celda random.
        int randRow = rand() % WIDTH;
        int randCol = rand() % HEIGHT;

        // Si ya tenía mina, intentamos de nuevo.
        if (mat[randRow][randCol].mina == SI_MINA)    
            continue;
        
        mat[randRow][randCol].mina = SI_MINA;

        // Modificamos minasVecinas de sus vecinos.
        for (int j = 0; j < mat[randRow][randCol].n_vecinos; j++)
            mat[randRow][randCol].vecinos[j]->minasVecinas++;

        i++;
    }  while(i <= N_MINES);  

    return;    
}



void inicializarVecinos(cell** mat){     
     // Inicializamos vecinos para cada celda.

    // Las esquinas...
    mat[0][0].n_vecinos = 3;
    mat[0][0].vecinos[0] = &mat[0][1];
    mat[0][0].vecinos[1] = &mat[1][1];
    mat[0][0].vecinos[2] = &mat[1][0];

    mat[WIDTH - 1][0].n_vecinos = 3;
    mat[WIDTH - 1][0].vecinos[0] = &mat[WIDTH - 2][0];
    mat[WIDTH - 1][0].vecinos[1] = &mat[WIDTH - 2][1];
    mat[WIDTH - 1][0].vecinos[2] = &mat[WIDTH - 1][1];

    mat[0][HEIGHT - 1].n_vecinos = 3;
    mat[0][HEIGHT - 1].vecinos[0] = &mat[1][HEIGHT - 1];
    mat[0][HEIGHT - 1].vecinos[1] = &mat[1][HEIGHT - 2];
    mat[0][HEIGHT - 1].vecinos[2] = &mat[0][HEIGHT - 2];

    mat[WIDTH - 1][HEIGHT - 1].n_vecinos = 3;
    mat[WIDTH - 1][HEIGHT - 1].vecinos[0] = &mat[WIDTH - 1][HEIGHT - 2];
    mat[WIDTH - 1][HEIGHT - 1].vecinos[1] = &mat[WIDTH - 2][HEIGHT - 2];
    mat[WIDTH - 1][HEIGHT - 1].vecinos[2] = &mat[WIDTH - 2][HEIGHT - 1];


    // Los bordes del tablero.
    for (int i = 1; i <= WIDTH - 2; i++){
        mat[i][0].n_vecinos = 5;
        mat[i][0].vecinos[0] = &mat[i + 1][0];
        mat[i][0].vecinos[1] = &mat[i + 1][1];
        mat[i][0].vecinos[2] = &mat[i][1];
        mat[i][0].vecinos[3] = &mat[i - 1][1];
        mat[i][0].vecinos[4] = &mat[i - 1][0];

        mat[i][HEIGHT - 1].n_vecinos = 5;
        mat[i][HEIGHT - 1].vecinos[0] = &mat[i + 1][HEIGHT - 1];
        mat[i][HEIGHT - 1].vecinos[1] = &mat[i + 1][HEIGHT - 2];
        mat[i][HEIGHT - 1].vecinos[2] = &mat[i][HEIGHT - 2];
        mat[i][HEIGHT - 1].vecinos[3] = &mat[i - 1][HEIGHT - 2];
        mat[i][HEIGHT - 1].vecinos[4] = &mat[i - 1][HEIGHT - 1];
    }

    for (int i = 1; i <= HEIGHT - 2; i++){
        mat[0][i].n_vecinos = 5;
        mat[0][i].vecinos[0] = &mat[0][i + 1];
        mat[0][i].vecinos[1] = &mat[1][i + 1];
        mat[0][i].vecinos[2] = &mat[1][i];
        mat[0][i].vecinos[3] = &mat[1][i - 1];
        mat[0][i].vecinos[4] = &mat[0][i - 1];

        mat[WIDTH - 1][i].n_vecinos = 5;
        mat[WIDTH - 1][i].vecinos[0] = &mat[WIDTH - 1][i - 1];
        mat[WIDTH - 1][i].vecinos[1] = &mat[WIDTH - 2][i - 1];
        mat[WIDTH - 1][i].vecinos[2] = &mat[WIDTH - 2][i];
        mat[WIDTH - 1][i].vecinos[3] = &mat[WIDTH - 2][i + 1];
        mat[WIDTH - 1][i].vecinos[4] = &mat[WIDTH - 1][i + 1];
    }
    

    // Todo el centro del tablero.
    for (int i = 1; i <= WIDTH - 2; i++)
        for(int j = 1; j <= HEIGHT - 2; j++){
            mat[i][j].n_vecinos = 8;
            mat[i][j].vecinos[0] = &mat[i + 1][j];
            mat[i][j].vecinos[1] = &mat[i + 1][j - 1];
            mat[i][j].vecinos[2] = &mat[i][j - 1];
            mat[i][j].vecinos[3] = &mat[i - 1][j - 1];
            mat[i][j].vecinos[4] = &mat[i - 1][j];
            mat[i][j].vecinos[5] = &mat[i - 1][j + 1];
            mat[i][j].vecinos[6] = &mat[i][j + 1];
            mat[i][j].vecinos[7] = &mat[i + 1][j + 1];
        }
    
    return;
}



void printTablero(cell** mat){    
    clearScreen();

    // Imprimimos números de filas y columnas.
    setColor(I_CYAN);

    for (int i = 0; i < WIDTH; i++){
        gotoxy(H_PADDING + 5*i + 2, V_PADDING - 1);
        printf("%d", i);
    }

    for (int i = 0; i < HEIGHT; i++){
        gotoxy(H_PADDING - 1, V_PADDING + 3*i + 1);
        printf("%c", 'A'+ i);
    }

    // Imprimimos el tablero.
    setColor(I_WHITE);
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++){

            if (mat[i][j].estado == NO_LIBERADO) {   
                // Imprimimos un bloque 5x3.
                for (int k = 5*i; k < 5*i + 5; k++)
                    for (int l = 3*j; l < 3*j + 3; l++){
                        gotoxy(H_PADDING + k, V_PADDING + l);
                        printf("%c", BLOCK);
                    }
                
                if (mat[i][j].bandera == SI_BANDERA){    // y si tiene bandera...
                    // imprime ">" en el centro del bloque 5x3.
                    gotoxy(H_PADDING + 5*i + 2, V_PADDING + 3*j + 1);
                    printf(">");
                }

            } else {    // si ya está liberada...

                // Hacemos un bloque vacío 5x3.
                 for (int k = 5*i; k < 5*i + 5; k++)
                    for (int l = 3*j; l < 3*j + 3; l++){
                        gotoxy(H_PADDING + k, V_PADDING + l);
                        printf(" ");
                    }
                
                // Si tiene mina...
                if (mat[i][j].mina  == SI_MINA){
                    gotoxy(H_PADDING + 5*i + 2, V_PADDING + 3*j + 1);
                    setColor(I_RED);
                    printf("X");
                    setColor(I_WHITE);
                    continue;
                }
                
                // Cantidad de minas vecinas.
                if (mat[i][j].minasVecinas > 0){
                    gotoxy(H_PADDING + 5*i + 2, V_PADDING + 3*j + 1);
                    printf("%d", mat[i][j].minasVecinas);
                }
                
            }   // END ELSE

        }   // END FOR
        
    return;
}



void accionCelda(char inChar, int inInt, juego* J){   
    int x = inInt;
    int y = (int) inChar - 'A';

    if (J->modo == MODO_BANDERA){ 

        J->banderasPuestas = (J->mat[x][y].bandera == SI_BANDERA ? J->banderasPuestas - 1 : J->banderasPuestas + 1);
        J->minasFaltantes = N_MINES - J->banderasPuestas;
        J->mat[x][y].bandera *= (-1);   // Quitamos o ponemos bandera.

    } else {    // Estaba en modo MINA.

        liberaConPila(x, y, J);

    }
        
    return;
}

int inicializarJuego(juego* J){   
    J->mat = crearTablero();
    if(J->mat == NULL){
        printf("\n\tERROR iniciando el juego. ");
        return 1;
    }

    estadoInicialRandom(J->mat);

    J->celdasFaltantes = WIDTH * HEIGHT - N_MINES; 
    J->banderasPuestas = 0;
    J->minasFaltantes = N_MINES - J->banderasPuestas;
    J->condicionDeJuego = JUGANDO; 
    J->modo = MODO_MINA; 

    return 0;
}



void liberaConPila(int x, int y, juego* J){       
    // Declaramos e inicializamos pila.
    pila miPila;        miPila.tope = 0;

    // Empilamos celda ingresada por usuario.
    empilar(&miPila, &J->mat[x][y]);


    while ( miPila.tope > 0 ){  // Mientras no esté vacía la pila...

        cell* currentCell = NULL;
        desempilar(&miPila, &currentCell);
                                                            
        // Si tiene bandera, no hacemos nada. 
        if (currentCell->bandera == SI_BANDERA)
            continue;

        if (currentCell->estado == SI_LIBERADO){  // Si ya estaba liberada esa celda...

            if ( vecinosConBandera(currentCell) == currentCell->minasVecinas )
                // Liberamos vecinos sin bandera.
                empilarVecinos(&miPila, currentCell);
            
            continue;

        }  
        
        // Celda sin bandera y sin liberar.

        if (currentCell->mina == SI_MINA){

            // Si tenía mina, entonces pierde.
            currentCell->estado = SI_LIBERADO;
            J->condicionDeJuego = PIERDE;

        } else {

            currentCell->estado = SI_LIBERADO;
            J->celdasFaltantes--;
            // Si no tenía minas en sus vecinos, liberamos sus vecinos.
            if (currentCell->minasVecinas == 0)
                empilarVecinos(&miPila, currentCell);

        }

    }

    return;
    
}



void empilar(pila* miPila, cell* cell1){      
    miPila->cells[miPila->tope] = cell1;
    miPila->tope++;
    return;
}


void desempilar(pila* miPila, cell** cell1){      
    miPila->tope--;
    *cell1 = miPila->cells[miPila->tope];
    return;
}


int vecinosConBandera(cell* cell1){   
    int res = 0;

    for (int i = 0; i < cell1->n_vecinos; i++)
        if (cell1->vecinos[i]->bandera == SI_BANDERA)
            res++;
        
    return res;
}


void empilarVecinos(pila* miPila, cell* cell1){   
    for (int i = 0; i < cell1->n_vecinos; i++)
        if (cell1->vecinos[i]->estado == -1)
            empilar(miPila, cell1->vecinos[i]);
    
    return;
}



int getInput(char buffer[], char* inChar, int* inInt){
    // Regresa 0 si es una entrada válida
    // y 1 si es inválida.

    // Checamos si el char es X.
    if(buffer[0] == 'X' && buffer[1] == '\0'){
        *inChar = buffer[0];
        return 1;
    }

    // Validamos char.
    if (buffer[0] < 'A' || buffer[0] >= 'A' + HEIGHT)
        return 0;
    
    // Validamos int.
    int temp = atoi(buffer + 1);
    if (temp < 0 || temp >= WIDTH)
        return 0;

    // Si pasa ambos ifs, entonces es una entrada válida
    *inChar = buffer[0];
    *inInt = temp;

    return 1;
}



void printModo(MODO modo){ 

    gotoxy(H_PADDING + 5*WIDTH + 5, V_PADDING + 1);
    setColor(I_CYAN);
    printf("[MODO]   ");

    gotoxy(H_PADDING + 5*WIDTH + 5, V_PADDING + 2);

    switch (modo){
    case MODO_MINA: 
        setColor(I_YELLOW);
        printf("minar [X]");
        setColor(I_WHITE);
        printf(" | bandera [>]");
        break;
    
    case MODO_BANDERA:
        setColor(I_WHITE);
        printf("minar [X] | ");
        setColor(I_YELLOW);
        printf("bandera [>]");
        setColor(I_WHITE); 
        break;

    default:
        printf("mina [X] | bandera [>]");
        break;
    }
    
    return;

}


void printMinasFaltantes(int minasFaltantes){

    gotoxy(H_PADDING + 5*WIDTH + 5, V_PADDING + 5);
    setColor(I_CYAN);
    printf("[MINAS FALTANTES]   ");
    gotoxy(H_PADDING + 5*WIDTH + 5, V_PADDING + 6);
    setColor(I_WHITE);
    printf("%d", minasFaltantes);
    return;

}


void titleScreen(void){   

    clearScreen();
    gotoxy(48, 12);
    setColor(I_YELLOW);
    printf("M I N E S W E E P E R");
    gotoxy(43, 14);
    setColor(I_CYAN);
    printf("Presione [ENTER] para comenzar...");
    setColor(I_WHITE);
    getchar();
    return;

}


void liberarTablero(cell** mat){
    for (int i = 0; i < WIDTH; i++)
        free(mat[i]);
    free(mat);
    return;
}

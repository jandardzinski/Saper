#ifndef _BOX_H_
#define _BOX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct box
{
        bool revealed;                  // zmienna sprawdzajaca czy dana pole w trakcie gry zostalo odkryte
        bool flagged;                     // zmienna ktora sprawdzi nam czy dane pole zostalo oznaczone flaga(zapobiegnie nacisniecie przypadkowe na nia i odsloniecie bomby)
        char value;    	//zmienna ktora posiada ilosc bomb  wokol danego pola albo sama bombe
} *box_t;

box_t **create_board(int row, int col);
void free_board(int row, int col, box_t** board);
void choose_mode_of_the_game(int* row, int* col, int* num_of_bombs);
void print_board(int row, int col, box_t** board);
void generate_bombs(int row, int col, box_t** board, int num_of_bombs);
void count_bombs_around(int row, int col, box_t** board);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct box
{
	bool revealed;			// zmienna sprawdzajaca czy dana pole w trakcie gry zostalo odkryte
	bool flagged;	                  // zmienna ktora sprawdzi nam czy dane pole zostalo oznaczone flaga(zapobiegnie nacisniecie przypadkowe na nia i odsloniecie bomby)
	int number;			//zmienna ktora posiada ilosc bomb wokol danego pola
} *box_t; 

//ogolnie revealed robie na boola bo bedzie albo zwracac true albo false bedzie szybciej po prostu i tak samo czy jest oflagowane

box_t **create_board(int row, int col)              //alokacja pamieci dla dwuwymiarowej tablicy struktur 
{
	box_t** board = malloc(row * sizeof *board);         // to jest alokacja dla wierszy
	if(board == NULL)
	{
		return NULL;
	}
       	for(int i = 0; i < row; i++)				//tutaj zaczyna sie alokacja dla kolumn 
	{
		board[i] = malloc(col * sizeof *board);            //tutaj alokacja dla kolumn dajaca wskazniki do danego elementu tablicy
		if(board[i] == NULL)
		{
			return NULL;
		}
		for(int j = 0; j < col; j++)
		{
			board[i][j] = malloc(sizeof **board);     // a tutaj alokacja pamieci dla struktur znajdujacych sie w kazdym elemencie tablicy
			board[i][j]->revealed = false;            
			board[i][j]->number = 0;	
		}
	}
	return board;	
}

void free_board(int row, int col, box_t** board)         //funkcja zwalniajaca struktury w kazdym polu tablicy i cala tablice
{
	
	for(int i = 0; i < row; i++)
	{
	
		for(int j = 0; j < col; j++)
			free(board[i][j]);	
			
		free(board[i]);
	}
	free(board);
}

void choose_mode_of_the_game(int* row, int* col)                       //funkcja wybierajaca tryb gry
{
	printf(" Wybierz tryb gry, podajac numer trybu:\n");
	printf("1. MALA PLANSZA\n");
	printf("2. SREDNIA PLANSZA\n");
	printf("3. DUZA PLANSZA\n");
	printf("4. NIESTANDARDOWA PLANSZA\n");
	
	char *choice = malloc(sizeof choice);                    //choice to jest numer trybu od 1-4 ale jako char, zeby odpowiednio zabezpieczyc program jak poda sie cokolwiek innego
	printf("\nWprowadz tryb w ktory chcesz zagrac: "); 
	scanf("%s", choice);										
	while(strlen(choice) != 1 || (*choice < '0' || *choice > '4'))		// no i tu na ASCI sprawdzanie podanego trybu 
	{
		printf("Podano niepoprawnie tryb gry: ");
		scanf("%s", choice);

	}

	int mode = *choice - '0';             // tu konwertuje z ASCI na normalna liczbe

	printf("%d\n", mode);

	switch (mode) 				// i tutaj switchem i caseami w zaleznosci jaki tryb wybierze sie to wykonuje sie danych case
	{
		case 1:
		*row = 9;
		*col = 9;
		break;
		case 2:
		*row = 16;
		*col = 16;
		break;
		case 3:
		*row = 16;
		*col = 30;
		break;
		case 4:
		printf("\nPodaj ilosc rzedow planszy: ");
		scanf("%d", row);

		printf("\nPodaj ilosc kolumn planszy: ");
                scanf("%d", col);
		break;
	}
}


/*
int main()
{
	int row;
	int col;
	choose_mode_of_the_game(&row, &col);
}

*/// to kontrolnie do sprawdzania










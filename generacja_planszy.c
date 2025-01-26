#include "generacja_planszy.h"

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
			board[i][j]->value = '0';
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

void choose_mode_of_the_game(int* row, int* col, int* num_of_bombs)                       //funkcja wybierajaca tryb gry
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
		*num_of_bombs = 10;
		*row = 9;
		*col = 9;
		break;
		case 2:
		*num_of_bombs = 40;
		*row = 16;
		*col = 16;
		break;
		case 3:
		*num_of_bombs = 99;
		*row = 16;
		*col = 30;
		break;
		case 4:
		printf("\nPodaj ilosc rzedow planszy: ");
		scanf("%d", row);

		printf("\nPodaj ilosc kolumn planszy: ");
                scanf("%d", col);

		printf("\nPodaj liczbe bomb ktore chcesz wygenerowac: ");
		scanf("%d", num_of_bombs);

		break;
	}
}

void print_board(int row, int col, box_t** board)
{
	for(int i = 0; i < col; i += 2)
	{
		printf("  %d  ", i+1);
		printf(" %d ", i+2);
	}
	printf("\n");	

	
	for(int i = 0; i < col; i++)
	{
		printf("+---");
	}
	printf("+\n");
	for(int i = 0; i < row; i++)
	{
	for(int j = 0; j < col; j++)
	{
		if(board[i][j]->revealed || board[i][j]->value == 'F')
		{
			if(board[i][j]->value > '0' || board[i][j]->value == '*')
				printf("| %c ", board[i][j]->value);

			else
			{
				printf("| "); 
				printf("\u25AA ");
			}
		}
		else
		printf("|   ");
	}
	printf("|\n");
	for(int i = 0; i < col; i++)
	{
		printf("+---");
	}
	printf("+\n");
	}
  
}

void generate_bombs(int row, int col, box_t** board, int num_of_bombs)
{
	int placed_bombs = 0;
	while(placed_bombs < num_of_bombs)
	{
		int randrow = rand() % row ;
	      	int randcol = rand() % col ;
		if(board[randrow][randcol]->value != '*')
			{
				board[randrow][randcol]->value = '*';
				placed_bombs++;
			}	
	}
}	

void count_bombs_around(int row, int col, box_t** board)
{
	int bomb_counter = 0;
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{

			for(int x = i-1; x <= (i+1); x++)
			{
				//printf("y\n");
				for(int y = j-1; y <= (j+1); y++)
				{
					//printf("x\n");
					if(x >= 0 && y >= 0 && x < row && y < col)
					{
						if(board[x][y]->value == '*')
						bomb_counter++;	
					}
				}
			}
			//printf("%d\n", bomb_counter);
			if(board[i][j]->value != '*')
			{
				if(bomb_counter > 0)
				{
					board[i][j]->value = '0' + bomb_counter;
				}	
			
			}
			bomb_counter = 0;
		}	
	}
}





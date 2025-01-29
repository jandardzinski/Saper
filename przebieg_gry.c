#include "generacja_planszy.h"
#include "przebieg_gry.h"
#include <unistd.h>
#include <stdbool.h>

static int revealed = 0;
static int mode; 
static int bomb;
static bool first_move = true;

void set_mode(int value)
{
	mode = value;
	return;
}

int get_mode()
{
	return mode;
}

int get_revealed()
{
	return revealed;
}

void set_bomb(int value)
{
	bomb = value;
	return;
}

void reveal_box(box_t** board, int row, int col, int x, int y)                  //funkcja odpowiadajaca za odkrywanie pol na planszy 
{											
	if(board[x][y]->value == 'F' || (board[x][y]->revealed && first_move))
	{
		return;	
	}
	if((x >= 0 && y >= 0) && x <= row && y <= col)
	{
		if(!board[x][y]->revealed && !board[x][y]->bomb)
			revealed++;

		board[x][y]->revealed = true;

		if(board[x][y]->value == '0' )
		{
			for(int i = x-1; i <= (x+1); i++)
			{
				for(int j = y-1; j <= y+1; j++)
				{
					if((i >= 0 && j >= 0) && i < row && j < col)
					{
						if(!board[i][j]->revealed)
						reveal_box(board, row, col, i, j);	
					}
				}
			}
		}
	}
}

void reveal_bombs(box_t **board, int row, int col)     // funkcja sluzaca do odkrywania bomb gdy koniec gry
{
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			if(board[i][j]->value == '*')
				board[i][j]->revealed = true;
		}
	}

}

void place_flag(box_t** board, int x, int y, int row, int col)                // funkcja sluzaca do oflagowywania danego pola na planszy 
{
	if(board[x][y]->revealed)						//blokowanie flagowania juz odkrytego pola
	{
		return;
	}
	else
	{
		if(board[x][y]->value != 'F')
		{
			board[x][y]->flagged = true;
			board[x][y]->value = 'F';
		}

		else if(board[x][y]->bomb) 			// w momencie odflagowywania jakiegos pola dla value przywracamy bombe lub odpowiednia liczbe
		{
			board[x][y]->flagged = false;
			board[x][y]->value = '*';
		}
		else								
		{
			board[x][y]->flagged = false;
			board[x][y]->value = '0' ;
			count_again(board, x, y, row, col); 		
		}
	}
}

void process_arguments(int row, int col, box_t** board)				// funkcja przetwarzajaca podawane polecenia f lub r 
{
	int x;
	int y;

	char move;

	bool game_over = false;

	while(!game_over)
	{
		if(scanf("\n%c %d %d", &move, &x, &y) == 3)                // odczyt ruchu podanego przez gracza 		
		{
			if(x >= 0 && x <= row && y >= 0 && y <= col)
			{
			switch (move)
			{
				case 'r':
				if(first_move)						//warunek dla pierwszego ruchu, gwarancja nieodkrycia bomby w pierwszym ruchu
				{
					revealed++;
					first_move = false;
					board[x-1][y-1]->revealed = true;
					generate_bombs(row, col, board, bomb);			
					count_bombs_around(row, col, board);		
				}
					
				reveal_box(board, row, col, x-1, y-1);				// odkrywanie pola
				
				system("clear");						// aktualizacja planszy
				print_board(row, col, board);

				if(revealed == row*col - bomb)                                        
				{								// w momencie odkrycia wszystkich pol dobrze gra konczy sie zwyciestwem
					game_over = true;					// odkrycie wszystkich  bomb
					reveal_bombs(board, row, col);				
					win(board, row, col);
				}

				else if(board[x-1][y-1]->value == '*')                         // w momencie odkrycia bomby gra sie konczy przegrana
				{
					game_over = true;				
					reveal_bombs(board, row, col);				// odkrycie wszystkich bomb na planszy 
					loss(board, row, col);							
				}

				break;
				case 'f':						
				place_flag(board, x-1, y-1, row, col);				// dla f - oflagowanie danego pola i odswiezenie planszy

				system("clear");
				print_board(row, col, board);

				break;
				default:
				printf("Podano niepoprawna funkcje\n");
				break;
			}
			}

			else
			{	
				system("clear");
				print_board(row, col, board);
			}
		}
	}
}

void win(box_t **board, int row, int col)  // moment zwyciestwa w grze 
{
	system("clear");			
	print_board(row, col, board);			
	
	int score = 10*revealed*mode;			//punkty za odkryte prawidlowo pola 

	printf("\nYou won\n");			
	printf("Your score: %d\n", score);		//wypisanie wyniku

	add_to_scoreboard(score); 			// dodanie wyniku do listy graczy
	show_scoreboard();				// wyswietlenie top5 graczy

	free_board(row, col, board);			// zwolnienie pamieci tablicy

	return;
}

void loss(box_t **board, int row, int col)			// moment przegrania gry
{
	system("clear");
	print_board(row, col, board);				// wypisanie odkrytej planszy
		
	int score = 10*revealed*mode;				//punkty za rozegrana gre

	printf("\nYou lost\n");					
	printf("Your score: %d\n", score);			//wypisanie wyniku
	
	add_to_scoreboard(score);				//tak samo wypisanie listy top5 graczy i dodanie wyniku do listy
	show_scoreboard();

	free_board(row, col, board);

	return;
}

void add_to_scoreboard(int new_score)
{
	FILE *scoreboard = fopen("scoreboard.txt", "r");		
	FILE *new_scoreboard = fopen("new_scoreboard.txt", "w");
	char new_name[32];

	char name[32];
	int score;

	printf("\nPodaj nazwe gracza: ");			//podanie nazwy gracza na koniec gry
	scanf("%s", new_name);					

	bool replaced = false;					

	while(fscanf(scoreboard, "%s %d", name, &score) == 2)	// odczyt zapisanych wynikow graczy z pliku	
	{
		if(!replaced && score < new_score)					// dodanie w odpowiednie miejsce na liscie nowego gracza z jego wynikiem
		{
			replaced = true;
			fprintf(new_scoreboard, "%s %d\n", new_name, new_score);	// zapisanie nowego gracza do listy
		}

		fprintf(new_scoreboard, "%s %d\n", name, score); 			// dodanie listy graczy do nowej listy
	}

	if(!replaced)
		fprintf(new_scoreboard, "%s %d\n", new_name, new_score);    // dodanie nowego gracza na ostatnie miejsce

	fclose(scoreboard);
	fclose(new_scoreboard);

	remove("scoreboard.txt");					// usuniecie starej listy i przypisanie nazwy starej listy do nowej
	rename("new_scoreboard.txt", "scoreboard.txt");					
}

void show_scoreboard()							// odczyt listy z pliku i wypisanie top5 graczy
{
	FILE *scoreboard = fopen("scoreboard.txt", "r");

	char name[32];
	int score;

	printf("\nSCOREBOARD\n");

	for(int i = 1; i <= 5; i++)
	{
		if(fscanf(scoreboard, "%s %d", name, &score) == 2)
		       printf("%d. %s %d\n", i, name, score);

		else 
			break;	
	}

	fclose(scoreboard);

}

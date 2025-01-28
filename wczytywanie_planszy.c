#include "wczytywanie_planszy.h"
#include "generacja_planszy.h"
#include "przebieg_gry.h"
#include <stdio.h>

void load_board(box_t **board)		// wczytywanie planszy i ruchow z pliku
{
	char file_name[32];

	printf("Wprowadz nazwe pliku: ");	//podajemy nazwe pliku z ktorego wczytujemy
        scanf("%s", file_name);          

	FILE *file = fopen(file_name, "r");  

	while(file == NULL)
	{
		printf("Nie poprawna nazwa pliku\nWprowadz nazwe ponownie: ");	 // jak plik nie istnieje to ponowna mozliwosc podania nazwy pliku
		scanf("%s", file_name);
		file = fopen(file_name, "r");
	}

	int row, col, bombs;			
	int mode;
	int revealed;

	fscanf(file, "%d %d %d", &row, &col, &bombs);                     // pierwsze 3 wartosci w pliku - liczba wierszy, kolumn i bomb
	
	if(row >= 9 && row < 16)					// ustawienie trybu gry w zaleznosci od podanych wartosci
		mode = 1;

	else if(row >= 16 && row < 30)
		mode = 2;

	else if(row >= 30)
		mode = 3;
	board = create_board(row, col);					// stworzenie planszy

	set_bomb(bombs);						// zwraca ilosc bomb na planszy

	int x, y;

	for(int i = 0; i < bombs; i++)					// odczyt wspolrzednych bomb podanych w pliku testowym	
	{
		fscanf(file, "%d %d", &x, &y);			
		board[x-1][y-1]->value = '*';				// przypisanie bomby do danej wspolrzednej
	}

	count_bombs_around(row, col, board);				// zliczenie bomb wokol danego pola

	char move;
	bool game_over = false;
	int counter = 0;					

	while(fscanf(file, "\n%c %d %d", &move, &x, &y) == 3)			// wczytywanie ruchow z pliku
	{
		switch(move)
		{
			case 'r':
			reveal_box(board, row, col, x-1, y-1);			// gdy r no to odkrywanie planszy
			revealed = get_revealed();

			if(!game_over && board[x-1][y-1]->value == '*' && !board[x-1][y-1]->flagged)
			{
				printf("Poprawne ruchy: %d\nWynik: %d\n0\n", counter, 10*(revealed-1)*mode);   // gdy odkryjemy bombe 
													       // wypisanie ilosci poprawnych ruchow, oraz wyniku gracza
				game_over = true;
			}

			else if(!game_over && revealed == row*col - bombs)					// wygrana i wypisanie tego samego		
			{
				printf("Poprawne ruchy: %d\nWynik: %d\n1\n", counter, 10*revealed*mode);
				game_over = true;
			}

			else
				counter++;									


			break;					
			case 'f':
			place_flag(board, x-1, y-1, row, col);			//oflagowywanie danego pola
			break;
		}


	}

	if(!game_over)										// wynik gry w momencie jej niedokonczenia 
	{
		revealed = get_revealed();
		printf("Poprawne ruchy: %d\nWynik: %d\n0\n", counter, 10*revealed*mode);
	}

	fclose(file);	
}

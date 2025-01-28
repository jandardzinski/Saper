#include "generacja_planszy.h"
#include "przebieg_gry.h"

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
			board[i][j]->bomb = false;
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

	set_mode(mode);

	switch (mode) 				// i tutaj switchem i caseami w zaleznosci jaki tryb wybierze sie to wykonuje sie danych case
	{
		case 1:
		*num_of_bombs = 10;
		*row = 9;				// mala, srednia i duza plansza - dla nich ilosc bomb i rozmiar z polecenia
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
		case 4: 						//plansza niestandardowa
		printf("\nPodaj ilosc rzedow planszy: ");		// rozmiary oraz ilosc bomb podawane przez gracza
		while(check_arguments(row) != 0)
		{
			printf("Blednie podano ilosc rzedow\nWprowdz wartosc ponownie: ");	//sprawdzenie czy dobrze podano liczbe wierszy
		}				
		
		printf("\nPodaj ilosc kolumn planszy: ");
		while(check_arguments(col) != 0)							// -||- liczbe kolumn	
		{
			printf("Niepoprawna wartosc kolumn\nWprowadz wartosc ponownie: ");
		}													
		printf("\nPodaj liczbe bomb ktore chcesz wygenerowac: ");			// -||- liczby bomb
		while(check_arguments(num_of_bombs) != 0)
		{	
			printf("Niepoprawnie podano ilosc bomb\n");
		}
		break;
	}

	if(*row >= 9 && *row < 16)
		set_mode(1);

	else if(*row >= 16 && *row < 30)
		set_mode(2);
	
	else if(*row >= 30)
		set_mode(3);

}

void print_board(int row, int col, box_t** board)    //funkcja wypisujaca plansze do terminala
{
	printf("\n");	

	printf("Wynik: %d\n\n", 10*get_revealed()*get_mode());  
	
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
		{									// uzupelnianie tablicy bombami i liczba bomb wokol danego pola
			if(board[i][j]->value > '0' || board[i][j]->value == '*')
				printf("| %c ", board[i][j]->value);

			else
			{
				printf("| "); 				// puste pole
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

void generate_bombs(int row, int col, box_t** board, int num_of_bombs)          //funkcja generujaca bomby
{
	int placed_bombs = 0;                                    
	while(placed_bombs < num_of_bombs)          
	{
		int randrow = rand() % row ;
	      	int randcol = rand() % col ;
		if(!board[randrow][randcol]->revealed && board[randrow][randcol]->value != '*')
			{
				board[randrow][randcol]->bomb = true;
				board[randrow][randcol]->value = '*';
				placed_bombs++;
			}	
	}
}	

void count_bombs_around(int row, int col, box_t** board)  // funkcja zliczajaca bomby wokol danego pola
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

void count_again(box_t **board, int x, int y, int row, int col)      // ponowne zliczenie bomb (w wypadku gdy dla dane pole zostanie odflagowane)
{
	if(board[x][y]->value == '*')
	{
		return;
	}
	else
	if(x-1 >= 0 && y-1 >= 0 && x+1 < row && y+1 < col)
	for(int i = x - 1; i <= x + 1; i++)
	{
		for(int j = y - 1; i <= y + 1; j++)
		{
			if(board[i][j]->value == '*')
			board[i][j]->value++;
		}
	}
	
}

int check_arguments(int *value)               // funkcja sprawdzajaca czy podane rozmiary planszy i liczba bomb bedzie prawidlowa ( do 4 trybu gry )
{
	char buf[32];
	
	scanf("%s", buf);

	for(long unsigned int i = 0; i < strlen(buf); i++)
	{
		if(!isdigit(buf[i]))
		{
			return 1;
		}
	}

	if(atoi(buf) >= 9)
	{
		*value = atoi(buf); 
		return 0;
	}

	else
		return 1;
} 

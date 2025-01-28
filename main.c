#include "generacja_planszy.h"
#include "przebieg_gry.h"
#include "wczytywanie_planszy.h"
#include <locale.h>

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	int row;
	int col;
	int num_of_bombs;
	srand(time(NULL));

	box_t **board;
	
	if(argc < 2)
	{
		choose_mode_of_the_game(&row, &col, &num_of_bombs);
		board = create_board(row, col);
		
		set_bomb(num_of_bombs);
			
		system("clear");

		print_board(row, col, board);
		process_arguments(row, col, board);
	}

	if(argc > 1 && strcmp(argv[1], "test") == 0)
		load_board(board);

	return 0;	
}

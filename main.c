#include "generacja_planszy.h"
#include "przebieg_gry.h"
#include <locale.h>

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	int row;
	int col;
	int num_of_bombs;
	srand(time(NULL));
	choose_mode_of_the_game(&row, &col, &num_of_bombs);
	box_t** board = create_board(row, col);
	generate_bombs(row, col, board, num_of_bombs);
	count_bombs_around(row, col, board);
	
	system("clear");

	print_board(row, col, board);
	process_arguments(argc, argv, row, col, board);
	
/*	printf("\n\n");
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			if(board[i][j]->value > '0' || board[i][j]->value == '*')
				printf("%c ", board[i][j]->value);
			else	
				printf("\u25AA ");
		}
		printf("\n\n");
	}
	return 0;
}
*/
	return 0;
}

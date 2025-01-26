
#include "generacja_planszy.h"
#include <unistd.h>



void reveal_box(box_t** board, int row, int col, int x, int y)
{
	if(board[x][y]->value == 'F')
	{
		printf("Nie mozna odkryc tego pola");
		return;	
	}
	if((x >= 0 && y >= 0) && x <= row && y <= col)
	{
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

void place_flag(box_t** board, int row, int col, int x, int y)
{
	if(board[x][y]->revealed)
	{
		printf("Nie mozna flagowac odkrytego wczesniej pola\n");
		return;
	}
	else 
	{
		if(board[x][y]->value != 'F')
		{
			board[x][y]->value = 'F';
		}
		else
		board[x][y]->value = '0';
	}
}

void process_arguments(int argc, char **argv, int row, int col, box_t** board)
{
	int x;
	int y;

	char move[2];
	char buf_x[4];
	char buf_y[4];

	while(true)
	{
		if(scanf("%s %s %s", move, buf_x, buf_y) == 3)		
		{
			x = atoi(buf_x);
			y = atoi(buf_y);
		}			

		switch (*move)
		{
			case 'r':
			reveal_box(board, row, col, x-1, y-1);
			break;
			case 'f':
			place_flag(board, row, col, x-1, y-1);
			break;
			default:
			printf("Podano niepoprawna funkcje\n");
			break;
		}

		system("clear");
		print_board(row, col, board);
	}
}

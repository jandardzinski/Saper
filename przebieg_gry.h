#include "generacja_planszy.h"

void set_first_move_false();
void set_mode(int value);
void set_bomb(int value);
int get_mode();
int get_revealed();
void reveal_box(box_t** board, int row, int col, int x, int y);
void place_flag(box_t** board, int x, int y, int row, int col);
void reveal_bombs(box_t **board, int row, int col);
void process_arguments(int row, int col, box_t** board);
void win(box_t **board, int row, int col);
void loss(box_t **board, int row, int col);
void add_to_scoreboard(int new_score);
void show_scoreboard();

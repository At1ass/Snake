#include <ncurses.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "snake_body.h"
#include "field.h"
#include "movement.h"
#define SET_POS(a, RC) !((snake_body->a - a) == field->RC - 1 || (snake_body->a - a) == 0) ? snake_body->a - a : field->RC - 1 - snake_body->a;

game_field *field;

int score = 0;
int speed = 5;

Snake *snake_body = NULL;

void gen_fruit()
{
	int x;
	int y;
	do
	{
		x = rand() % (field->ROW - 2) + 1;
		y = rand() % (field->COL - 2) + 1;

	}
	while(field->area[x][y] == 'B' || field->area[x][y] == 'o');

	field->area[x][y] = 'A';
}

void game_over()
{

	printw("YOU FAILED\n");
	nocbreak();

	game_field_cleanup(&field);
	getch();
	endwin();
	exit(0);
}

void move_snake(int x, int y)
{
	int tmp_X = snake_body->x;
	int tmp_Y = snake_body->y;

	snake_body->x = SET_POS(x, ROW);
	snake_body->y = SET_POS(y, COL);

	if((field->area[snake_body->x][snake_body->y] == 'o') ||
		(field->area[snake_body->x][snake_body->y] == 'T'))
	{
		game_over();
	}

	if(field->area[snake_body->x][snake_body->y] == 'A')
	{
		++score;
		add_tail(&snake_body);
		gen_fruit();
	}
	field->area[snake_body->x][snake_body->y] = 'B';

	Snake *tmp = snake_body->next;
	while(tmp != NULL)
	{
		int tx = tmp->x;
		int ty = tmp->y;

		tmp->x = tmp_X;
		tmp->y = tmp_Y;

		field->area[tmp->x][tmp->y] = 'o';

		tmp_X = tx;
		tmp_Y = ty;

		tmp = tmp->next;
	}

	field->area[tmp_X][tmp_Y] = ' ';
}

void Draw()
{
	printw("SCORE: %d | SPEED: %d", score, 5 - speed);
	for(int i = 0; i < field->ROW; i++) {
		for(int j = 0; j < field->COL; ++j) {
			int key_pair = 4;
			if(field->area[i][j] == 'B') {
				key_pair = 3;
			}
			if(field->area[i][j] == 'o') {
				key_pair = 1;
			}
			if(field->area[i][j] == 'A') {
				key_pair = 5;
			}
			if(field->area[i][j] == '0' ||
				field->area[i][j] == 'T') {
				key_pair = 2;
			}

			attron(COLOR_PAIR(key_pair));
			printw("%c%c", field->area[i][j], field->area[i][j]);
			attroff(COLOR_PAIR(key_pair));
		}

		printw("\n");

	}
	refresh();
}

void Speed_change()
{
	static bool is_change = false;

	if(score % 15 == 0 && is_change)
	{
		halfdelay(--speed);
		is_change = false;
	}
	else
	{
		if(score % 15 != 0)
			is_change = true;
	}
}

bool
opposite(const size_t x,
        const size_t prev_x,
        const size_t y,
        const size_t prev_y)
{
    return (x == -prev_x && y == prev_y)
        || (x == prev_x && y == -prev_y);
}

int main()
{

	int xW, yW;
	int x = -1, y = 0;
	int is_change = false;
    size_t prev_x = 0, prev_y = 0;
	initscr();

	getmaxyx(stdscr ,yW, xW);

	field = init_field();
	field->field_ops_t->init_game_field_size(&field, xW, yW);
	field->field_ops_t->generate_game_area(&field);
	generate_troubles(&field, field->ROW, field->COL);

	printf("11 %d", field->COL);
	if(has_colors() == FALSE)
	{

		game_field_cleanup(&field);
		endwin();
		exit(EXIT_FAILURE);
	}
	start_color();

	init_pair(1, COLOR_RED, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLUE);
	init_pair(4, COLOR_GREEN, COLOR_GREEN);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);

	noecho();
	halfdelay(speed);
	keypad(stdscr, TRUE);

	add_tail(&snake_body);
	add_tail(&snake_body);
	add_tail(&snake_body);

	while(1){

		erase();
		Draw();

		Speed_change();

		int input = getch();

		if(input == KEY_UP || input == 'k')
		{
			MOVE_UP
		}
		if(input == KEY_DOWN || input == 'j')
		{
			MOVE_DOWN
		}
		if(input == KEY_LEFT || input == 'h')
		{
			MOVE_LEFT
		}
		if(input == KEY_RIGHT || input == 'l')
		{
			MOVE_RIGHT
		}

        if (opposite(x, prev_x, y, prev_y)) {
            x = prev_x;
            y = prev_y;
        }
		move_snake(x, y);

        prev_x = x;
        prev_y = y;
	}

	game_field_cleanup(&field);
	getch();

	endwin();

	return 0;
}

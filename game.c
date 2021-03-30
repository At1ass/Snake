#include <ncurses.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "snake_body.h"
#include "field.h"

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
		x = rand() % 8 + 1;
		y = rand() % 8 + 1;
	}
	while(field->area[x][y] == 'B' || field->area[x][y] == 'o');
	
	field->area[x][y] = 'A';
}

void move_snake(int x, int y)
{
	bool isMoveX = true;
	bool isMoveY = true;
	bool need_fruit = false;
	field->area[snake_body->x][snake_body->y] = ' ';
	int tmp_X = snake_body->x, tmp_Y = snake_body->y;
	snake_body->x = !((snake_body->x - x) ==field->ROW - 1 || (snake_body->x - x) == 0) ? snake_body->x - x : field->ROW - 1 - snake_body->x;
	snake_body->y = !((snake_body->y - y) == field->COL - 1 || (snake_body->y - y) == 0) ? snake_body->y - y : field->COL - 1 - snake_body->y;
	
	if(field->area[snake_body->x][snake_body->y] == 'o')
	{
		printw("YOU FAILED\n");
		nocbreak();
		getch();
		endwin();
		exit(0);
	}

	if((snake_body->x == snake_body->next->x) &&
	  	(snake_body->y == snake_body->next->y) )
	  {
	  	snake_body->x = tmp_X;
		snake_body->y = tmp_Y;
	  }
	if(field->area[snake_body->x][snake_body->y] == 'A')
	{
		++score;
		add_tail(&snake_body);
		need_fruit = true;
	}
	field->area[snake_body->x][snake_body->y] = 'B';

	isMoveX = ((tmp_X - snake_body->x) == 0) ? false : true;
	isMoveY = ((tmp_Y - snake_body->y) == 0) ? false : true;

	Snake *tmp = snake_body->next;
	while(tmp != NULL) 
	{
		field->area[tmp->x][tmp->y] = ' ';
		int tx = tmp->x, ty = tmp->y;
		if(isMoveX || isMoveY){ 
			tmp->x = tmp_X;
			tmp->y = tmp_Y;
		}
		
	
		field->area[tmp->x][tmp->y] = 'o';

		tmp_X = tx;
		tmp_Y = ty;

		tmp = tmp->next;
	}

	if(need_fruit)
		gen_fruit();
}



int main()
{

	int xW, yW;
	int x = -1, y = 0;
	int is_change = false;
	initscr();
	
	getmaxyx(stdscr ,yW, xW);
	
	field = init_field();
	field->field_ops_t->init_game_field_size(&field, xW, yW);
	field->field_ops_t->generate_game_area(&field);

	printf("11 %d", field->COL);
	if(has_colors() == FALSE)
	{
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

		printw("SCORE: %d | SPEED: %d | xW yW: %d %d", score, 5 - speed, xW, yW);
		for(int i = 0; i < field->ROW; i++)
		{
			for(int j = 0; j < field->COL; ++j)
			{
				int key_pair = 4;
				if(field->area[i][j] == 'B')
				{
					key_pair = 3;
				}
				if(field->area[i][j] == 'o')
				{
					key_pair = 1;
				}
				if(field->area[i][j] == 'A')
				{
					key_pair = 5;
				}
				if(field->area[i][j] == '0')
				{
					key_pair = 2;
				}

				attron(COLOR_PAIR(key_pair));
				printw("%c%c", field->area[i][j], field->area[i][j]);
				attroff(COLOR_PAIR(key_pair));
			}

			printw("\n");


		}
		refresh();

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

		int input = getch();
		if(input == KEY_UP)
		{
			x = 1;
			y = 0;
		}
		if(input == KEY_DOWN)
		{
			x = -1;
			y = 0;
		}
		if(input == KEY_LEFT)
		{
			x = 0;
			y = 1;
		}
		if(input == KEY_RIGHT)
		{
			x = 0;
			y = -1;
		}
			move_snake(x, y);
	}

	free(field);
	getch();

	endwin();

	return 0;
}

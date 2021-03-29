#include <ncurses.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "snake_body.h"

char field[10][10] = 
{
	{'0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', 'B', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', '0', '0', '0', '0', '0', '0', '0', '0', '0' }
};

int score = 0;
int speed = 5;

Snake *snake_body = NULL;
/*struct snake
{
	int x;
	int y;

	struct snake *next;
};
typedef struct snake Snake;

void add();
Snake *snake = NULL;

void add()
{
	if(snake == NULL) {
		snake = (Snake *)malloc(sizeof(Snake));
		snake->x = 4;
		snake->y = 4;
		snake->next = NULL;
		return;
	}

	static int i = 0;
	Snake *tmp = snake;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	Snake *ttmp = (Snake *)malloc(sizeof(Snake));
	ttmp->x = tmp->x;
	ttmp->y = tmp->y;
	ttmp->next = NULL;
	tmp->next = ttmp;
}
*/
void gen_fruit()
{
	int x;
	int y; 
	do
	{
		x = rand() % 8 + 1;
		y = rand() % 8 + 1;
	}
	while(field[x][y] == 'B' || field[x][y] == 'o');
	field[x][y] = 'A';
	
}

void swap(int x, int y)
{
	bool isMoveX = true;
	bool isMoveY = true;
	bool need_fruit = false;
	field[snake_body->x][snake_body->y] = ' ';
	int tmp_X = snake_body->x, tmp_Y = snake_body->y;
	snake_body->x = !((snake_body->x - x) == 9 || (snake_body->x - x) == 0) ? snake_body->x - x : 9 - snake_body->x;
	snake_body->y = !((snake_body->y - y) == 9 || (snake_body->y - y) == 0) ? snake_body->y - y : 9 - snake_body->y;
	
	if(field[snake_body->x][snake_body->y] == 'o')
	{
		printw("YOU FAILED\n");
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
	if(field[snake_body->x][snake_body->y] == 'A')
	{
		++score;
		add_tail(&snake_body);
		need_fruit = true;
	}
	field[snake_body->x][snake_body->y] = 'B';

	isMoveX = ((tmp_X - snake_body->x) == 0) ? false : true;
	isMoveY = ((tmp_Y - snake_body->y) == 0) ? false : true;

	Snake *tmp = snake_body->next;
	while(tmp != NULL) 
	{
		field[tmp->x][tmp->y] = ' ';
		int tx = tmp->x, ty = tmp->y;
		if(isMoveX || isMoveY){ 
			tmp->x = tmp_X;
			tmp->y = tmp_Y;
		}
		
	
		field[tmp->x][tmp->y] = 'o';

		tmp_X = tx;
		tmp_Y = ty;

		tmp = tmp->next;
	}

	if(need_fruit)
		gen_fruit();
}

int main()
{
	int x = -1, y = 0;
	int is_change = false;
	initscr();

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

		printw("SCORE: %d | SPEED: %d", score, 5 - speed);
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 10; ++j)
			{
				int key_pair = 4;
				if(field[i][j] == 'B')
				{
					key_pair = 3;
				}
				if(field[i][j] == 'o')
				{
					key_pair = 1;
				}
				if(field[i][j] == 'A')
				{
					key_pair = 5;
				}
				if(field[i][j] == '0')
				{
					key_pair = 2;
				}

				attron(COLOR_PAIR(key_pair));
				printw("%c%c", field[i][j], field[i][j]);
				attroff(COLOR_PAIR(key_pair));
			}

			printw("\n");

			/*memcpy(h, field[i], 10);
			attron(COLOR_PAIR(1));
			printw("%s\n", h);
			attroff(COLOR_PAIR(1));
		*/

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
			swap(x, y);
	}

	getch();

	endwin();

	return 0;
}

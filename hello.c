#include <ncurses.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>


char field[10][10] = 
{
	{'0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', '0' },
	{'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0' },
	{'0', '0', '0', '0', '0', '0', '0', '0', '0', '0' }
};

int score = 0;

struct snake
{
	int x;
	int y;

	struct snake *next;
};
typedef struct snake Snake;

void add();
Snake *snake_g = NULL;

void add()
{
	if(snake_g == NULL) {
		snake_g = (Snake *)malloc(sizeof(Snake));
		snake_g->x = 4;
		snake_g->y = 4;
		snake_g->next = NULL;
		return;
	}

	static int i = 0;
	Snake *tmp = snake_g;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	Snake *ttmp = (Snake *)malloc(sizeof(Snake));
	ttmp->x = tmp->x;
	ttmp->y = tmp->y;
	ttmp->next = NULL;
	tmp->next = ttmp;
}

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
	field[snake_g->x][snake_g->y] = ' ';
	int tmp_X = snake_g->x, tmp_Y = snake_g->y;
	snake_g->x = !((snake_g->x - x) == 9 || (snake_g->x - x) == 0)? snake_g->x - x : snake_g->x;
	snake_g->y = !((snake_g->y - y) == 9 || (snake_g->y - y) == 0) ? snake_g->y - y : snake_g->y;
	
	if(field[snake_g->x][snake_g->y] == 'o')
	{
		printw("YOU FAILED\n");
		getch();
		endwin();
		exit(0);
	}

	if((snake_g->x == snake_g->next->x) &&
	  	(snake_g->y == snake_g->next->y) )
	  {
	  	snake_g->x = tmp_X;
		snake_g->y = tmp_Y;
	  }
	if(field[snake_g->x][snake_g->y] == 'A')
	{
		++score;
		add();
		need_fruit = true;
	}
	field[snake_g->x][snake_g->y] = 'B';

	isMoveX = ((tmp_X - snake_g->x) == 0) ? false : true;
	isMoveY = ((tmp_Y - snake_g->y) == 0) ? false : true;

	Snake *tmp = snake_g->next;
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
	keypad(stdscr, TRUE);
	add();
	add();
	add();
	while(1){

		erase();

		printw("SCORE: %d", score);
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
				printw("%c", field[i][j]);
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

		int input = getch();
		if(input == KEY_UP)
			swap(1, 0);
		if(input == KEY_DOWN)
			swap(-1, 0);
		if(input == KEY_LEFT)
			swap(0, 1);
		if(input == KEY_RIGHT)
			swap(0, -1);
	}

	getch();

	endwin();

	return 0;
}

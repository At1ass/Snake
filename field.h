#ifndef __FIELD_H__
#define __FIELD_H__

#include <stdlib.h>

typedef struct field_ops game_field_ops;
typedef struct field_t game_field;

struct field_t
{
	char ** area;
	
	int ROW;
	int COL;

	game_field_ops *field_ops_t;

};

struct field_ops
{	
	void (*init_game_field_size)(game_field **,int, int);
	void (*generate_game_area)(game_field **);
};

void generate_game_area_t(game_field **field);

void init_game_field_size_t(game_field **field, int row, int col);

game_field * init_field();

void game_field_cleanup(game_field **field);

void generate_troubles(game_field **field, int heigth, int width);

static void game_field_level_create(game_field **field);

#endif

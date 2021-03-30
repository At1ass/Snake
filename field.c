#include "field.h"

void generate_game_area_t(game_field **field)
{
	(*field)->area = (char **)malloc((*field)->ROW * sizeof(char *));
        for(int i = 0; i < (*field)->ROW; ++i)
                (*field)->area[i] = (char *)malloc((*field)->COL * sizeof(char));

        for(int i = 0; i < (*field)->ROW; ++i) {
                for(int j = 0; j < (*field)->COL; ++j) {
                        if(i == 0 || j == 0 || i == (*field)->ROW - 1 || j == (*field)->COL - 1)
                                (*field)->area[i][j] = '0';
                        else
                                (*field)->area[i][j] = ' ';
                }
        }

        (*field)->area[4][4] = 'B';
        (*field)->area[(*field)->ROW / 2 + 1][(*field)->COL / 2 + 1] = 'A';	
}
void init_game_field_size_t(game_field **field, int x, int y)
{
	(*field)->ROW = y / 4 + y / 2;
	(*field)->COL = (x / 4 + x / 2) / 2;
}

game_field * init_field()
{
	game_field * field = (game_field *)calloc(1, sizeof(game_field));
	field->field_ops_t = (game_field_ops *)calloc(1, sizeof(game_field_ops));

	field->field_ops_t->init_game_field_size = &init_game_field_size_t;
	field->field_ops_t->generate_game_area = &generate_game_area_t;

	return field;
}

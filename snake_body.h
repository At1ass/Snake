#ifndef __SNAKE_BODY_H__
#define __SNAKE_BODY_H__

#include <stdlib.h>
#include <stddef.h>

struct snake
{
        int x;
        int y;

        struct snake *next;
};
typedef struct snake Snake;

void add_tail(Snake **snake_body);


#endif

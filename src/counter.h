#ifndef COUNTER_H
#define COUNTER_H

typedef struct counter_t counter_t;

#include "hud.h"

struct counter_t{
	hud_t *hud;
	uint32_t count;
	char label[16];
	uint32_t x, y;
};

counter_t *counter_create(hud_t *hud);
void counter_delete(counter_t *counter);

void counter_set_label(counter_t *counter, const char *string);
void counter_draw(counter_t *counter, SDL_Surface *target);

#endif

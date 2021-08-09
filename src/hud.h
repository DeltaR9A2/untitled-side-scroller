#ifndef HUD_H
#define HUD_H

typedef struct hud_t hud_t;

#include "game.h"
#include "font.h"
#include "counter.h"

struct hud_t{
	game_t *game;
	font_t *font;
	counter_t *counter;
};

hud_t *hud_create(game_t *game);
void hud_delete(hud_t *hud);

#endif

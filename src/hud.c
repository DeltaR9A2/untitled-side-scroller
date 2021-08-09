#include "hud.h"

hud_t *hud_create(game_t *game){
	hud_t *hud = malloc(sizeof(hud_t));
	
	hud->game = game;
	hud->font = game->font;
	hud->counter = counter_create(hud);
	counter_set_label(hud->counter, "Candy");
	
	return hud;
}

void hud_delete(hud_t *hud){
	counter_delete(hud->counter);
	free(hud);
}

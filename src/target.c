#include <stdlib.h>

#include "target.h"

const uint8_t TARGET_NONE = 0;
const uint8_t TARGET_INFO = 1;
const uint8_t TARGET_TEST = 2;

target_t *target_create(void){
	target_t *target = malloc(sizeof(target_t));

	target->type = TARGET_NONE;
	target->rect = rect_create();
	target->sprite = NULL;
	target->event = NULL;
	
	return target;
}

void target_delete(target_t *target){
	rect_delete(target->rect);
	free(target);
}

void target_activate(target_t *target, game_t *game){

}

void target_set_rect_numbers(target_t *target, uint32_t x, uint32_t y, uint32_t w, uint32_t h){
	target->rect->x = x;
	target->rect->y = y;
	target->rect->w = w;
	target->rect->h = h;

};

#include <stdlib.h>

#include "sprite.h"

struct sprite_t{
	rect_t *rect;
	anim_t *anim;
	int step;
};

sprite_t *sprite_create(){
	sprite_t *sprite = malloc(sizeof(sprite_t));
	sprite->rect = rect_create();
	return sprite;
}

void sprite_delete(sprite_t *sprite){
	rect_delete(sprite->rect);
	free(sprite);
}

void sprite_update(sprite_t *sprite){
	sprite->step += 1;
}

void sprite_set_anim(sprite_t *sprite, anim_t *anim){
	if(sprite->anim != anim){
		sprite->anim = anim;
		sprite->step = 0;
		
		if(anim != NULL && anim->frames != NULL && anim->frames[0] != NULL){
			sprite->rect->w = anim->frames[0]->w;
			sprite->rect->h = anim->frames[0]->h;
		}
		
	}
}

void sprite_move_to(sprite_t *sprite, rect_t *rect){
	rect_move_to(sprite->rect, rect);
}

void sprite_draw(sprite_t *sprite, SDL_Surface *target, uint32_t x_offset, uint32_t y_offset){
	SDL_Rect draw_rect;

	rect_copy_to_sdl(sprite->rect, &draw_rect);
	draw_rect.x -= x_offset;
	draw_rect.y -= y_offset;

	anim_draw(sprite->anim, sprite->step, target, &draw_rect);
}

rect_t *sprite_get_rect(sprite_t *sprite){
	return sprite->rect;
}
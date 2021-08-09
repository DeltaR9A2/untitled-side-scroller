#include <stdlib.h>

#include "sprite.h"

sprite_t *sprite_create(){
	sprite_t *sprite = malloc(sizeof(sprite_t));
	sprite->rect = rect_create();
	return sprite;
}

void sprite_delete(sprite_t *sprite){
	rect_delete(sprite->rect);
	free(sprite);
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


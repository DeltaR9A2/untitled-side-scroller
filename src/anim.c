#include <stdlib.h>

#include "anim.h"

anim_t *anim_create(void){
	anim_t *anim = malloc(sizeof(anim_t));
	anim->len = 0;
	anim->fps = 0;
	anim->frames = NULL;
	return anim;
}

void anim_init(anim_t *anim, fset_t *fset, int start, int len, int fps){
  printf("anim_init([*anim], [*fset], %i, %i, %i) \n", start, len, fps);
	anim->len = len;
	anim->fps = fps;
	
	anim->frames = malloc(sizeof(SDL_Surface*)*(anim->len));
	
	for(int i=0; i < anim->len; i++){
		anim->frames[i] = fset->frames[start + i];
	}
}	 

void anim_delete(anim_t *anim){
	free(anim->frames);
	free(anim);
}

void anim_draw(anim_t *anim, int step, SDL_Surface *target, SDL_Rect *dest){
	// Magic number 0.01 is the inverse of 100
	// and 100 is the refresh rate of the main loop
	// so step*0.01 turns steps back into seconds.
	SDL_BlitSurface(anim->frames[((int)((step*0.01) * anim->fps)) % anim->len], NULL, target, dest);
}


#ifndef sprite_h
#define sprite_h

#include "rect.h"
#include "anim.h"

typedef struct sprite_t sprite_t;
struct sprite_t{
	rect_t *rect;
	anim_t *anim;
	int step;
};

sprite_t *sprite_create();

void sprite_delete(sprite_t *sprite);

void sprite_set_anim(sprite_t *sprite, anim_t *anim);

#endif

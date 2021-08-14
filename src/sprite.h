#ifndef sprite_h
#define sprite_h

#include "rect.h"
#include "anim.h"

typedef struct sprite_t sprite_t;

sprite_t *sprite_create();
void sprite_delete(sprite_t *sprite);
void sprite_update(sprite_t *sprite);


void sprite_set_anim(sprite_t *sprite, anim_t *anim);
void sprite_move_to(sprite_t *sprite, rect_t *rect);

void sprite_draw(sprite_t *sprite, SDL_Surface *target, uint32_t x_offset, uint32_t y_offset);
rect_t *sprite_get_rect(sprite_t *sprite);

#endif

#ifndef sprite_h
#define sprite_h

#include "rect.h"

SDL_Surface *load_image(const char *fn);
SDL_Surface *create_surface(int32_t w, int32_t h);

typedef struct fset_t fset_t;
typedef struct anim_t anim_t;
typedef struct sprite_t sprite_t;

fset_t *fset_load(const char *fn, uint32_t cols, uint32_t rows);

anim_t *anim_create(const char* name, const fset_t *fset, int start, int len, int fps);
anim_t *anim_get(const char* name);

//bool sdl_rect_overlap(SDL_Rect *a, SDL_Rect *b);

sprite_t *sprite_create();
void sprite_delete(sprite_t *sprite);
void sprite_update(sprite_t *sprite);

void sprite_move_to(sprite_t *sprite, rect_t *rect);
void sprite_anim_set(sprite_t *sprite, anim_t *anim);
void sprite_anim_set_by_name(sprite_t *sprite, const char *name);

void sprite_draw(sprite_t *sprite, SDL_Surface *target, uint32_t x_offset, uint32_t y_offset);

#endif

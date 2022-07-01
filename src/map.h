#ifndef map_h
#define map_h

typedef struct map_t map_t;

#include "rect_list.h"
#include "cmap.h"

struct map_t{
	rect_t *rect;

	SDL_Surface *image;
	
	rect_list_t *terrain_rects;
	rect_list_t *platform_rects;
};

map_t *map_create(void);
void map_delete(map_t *map);

void map_init(map_t *map, const char *physics_fn, const char *image_fn);

void map_update(map_t *map);

#endif

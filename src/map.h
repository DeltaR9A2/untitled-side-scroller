#ifndef map_h
#define map_h

typedef struct map_t map_t;

#include "cmap.h"

struct map_t{
	rect_t rect;

	SDL_Surface *image;
	
	rect_t *terrain_rects;
	rect_t *platform_rects;
};

map_t *map_load(const char *map_name);

#endif

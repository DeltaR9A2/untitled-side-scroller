#ifndef map_h
#define map_h

#include <stdbool.h>

typedef struct map_t map_t;

#include "rect.h"

map_t *map_load(const char *map_name);

rect_t *map_get_rect(map_t *map);
SDL_Surface *map_get_image(map_t *map);

rect_t *map_get_terrain_rects(map_t *map);
rect_t *map_get_platform_rects(map_t *map);

double map_get_w(map_t *map);
double map_get_h(map_t *map);

#endif

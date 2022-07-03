#ifndef map_h
#define map_h

#include <stdbool.h>

typedef struct map_t map_t;

#include "rect.h"

map_t *map_load(const char *map_name);
SDL_Surface *map_get_image(map_t *map);
rect_t *map_get_rect(map_t *map);

rect_t *map_get_terrain_rects(map_t *map);
rect_t *map_get_platform_rects(map_t *map);

#endif

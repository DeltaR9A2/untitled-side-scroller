#ifndef map_h
#define map_h

#include "rect.h"

typedef struct map_t map_t;

struct map_t{
    rect_t *rect;
    rect_t *terrain_rects;
    rect_t *platform_rects;
    SDL_Surface *image;
};

map_t *map_load_by_name(const char *map_name);

#endif

#ifndef map_h
#define map_h

#include <stdbool.h>

typedef struct map_t map_t;

extern const double WORLD_CELL_W;
extern const double WORLD_CELL_H;

#include "rect.h"

struct map_t{
	rect_t *rect;
	rect_t *terrain_rects;
	rect_t *platform_rects;
	SDL_Surface *image;
    
    int32_t world_x;
    int32_t world_y;
};

void map_preload_all(void);

map_t *map_load_by_coords(int32_t x, int32_t y);
bool map_exists_at_coords(int32_t x, int32_t y);

double map_get_w(map_t *map);
double map_get_h(map_t *map);
double map_get_cell_w(map_t *map);
double map_get_cell_h(map_t *map);

map_t *map_load_by_name(const char *map_name);

#endif

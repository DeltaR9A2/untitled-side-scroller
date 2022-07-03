#ifndef map_h
#define map_h


typedef struct map_t map_t;
typedef struct cmap_t cmap_t;
typedef struct body_t body_t;

#include <stdbool.h>
#include "rect.h"

extern const uint32_t BLOCKED_L;
extern const uint32_t BLOCKED_R;
extern const uint32_t BLOCKED_U;
extern const uint32_t BLOCKED_D;
extern const uint32_t BLOCKED_MASK;

extern const uint32_t PLAT_DROP;

struct body_t{
	rect_t *rect;
	double vx, vy;
	uint32_t flags;
};

body_t *body_create(void);
void body_delete(body_t *body);

rect_t *body_get_rect(body_t *body);

void body_set_size(body_t *body, double w, double h);
void body_move_to(body_t *body, rect_t *rect);
void body_set_velocity(body_t *body, double vx, double vy);

void do_physics_to_it(body_t *body, rect_t *terr_rects, rect_t *plat_rects);



struct cmap_t{
	rect_t *rect;
	int8_t *data;
};

cmap_t *cmap_create(void);
void cmap_init(cmap_t *cmap, int32_t x, int32_t y, int32_t w, int32_t h);
void cmap_delete(cmap_t *cmap);
void cmap_reset(cmap_t *cmap);

void cmap_copy_data_from(cmap_t *cmap, int8_t *data);

int32_t cmap_xy_to_i(cmap_t *cmap, int32_t x, int32_t y);
bool cmap_xy_is_valid(cmap_t *cmap, int32_t x, int32_t y);

void cmap_rect_mark(cmap_t *cmap, rect_t *rect);
bool cmap_rect_check(cmap_t *cmap, rect_t *rect);

void cmap_add_to_rect_list(cmap_t *cmap, rect_t **rects);



struct map_t{
	rect_t rect;

	SDL_Surface *image;
	
	rect_t *terrain_rects;
	rect_t *platform_rects;
};

map_t *map_load(const char *map_name);

#endif

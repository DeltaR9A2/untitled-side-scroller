#ifndef map_h
#define map_h

#include <stdbool.h>

typedef struct map_t map_t;
typedef struct body_t body_t;

#include "rect.h"

extern const uint32_t BLOCKED_L;
extern const uint32_t BLOCKED_R;
extern const uint32_t BLOCKED_U;
extern const uint32_t BLOCKED_D;
extern const uint32_t BLOCKED_MASK;

extern const uint32_t PLAT_DROP;



body_t *body_create(void);
void body_delete(body_t *body);

rect_t *body_get_rect(body_t *body);

void body_move_to(body_t *body, rect_t *rect);
void body_set_pos(body_t *body, double x, double y);
void body_set_size(body_t *body, double w, double h);
void body_set_velocity(body_t *body, double vx, double vy);

void body_get_velocity(body_t *body, double *vx, double *vy);

void body_set_flags(body_t *body, uint32_t flags);
void body_unset_flags(body_t *body, uint32_t flags);
bool body_check_flags(body_t *body, uint32_t flags);

void do_physics_to_it(body_t *body, map_t *map);



map_t *map_load(const char *map_name);
SDL_Surface *map_get_image(map_t *map);
rect_t *map_get_rect(map_t *map);

rect_t *map_get_terrain_rects(map_t *map);
rect_t *map_get_platform_rects(map_t *map);

#endif

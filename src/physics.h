#ifndef physics_h
#define physics_h

#include "rect.h"

extern const uint32_t BLOCKED_L;
extern const uint32_t BLOCKED_R;
extern const uint32_t BLOCKED_U;
extern const uint32_t BLOCKED_D;
extern const uint32_t BLOCKED_MASK;

extern const uint32_t PLAT_DROP;

typedef struct body_t body_t;
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

#endif
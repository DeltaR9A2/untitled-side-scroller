#ifndef rect_h
#define rect_h

typedef struct rect_t rect_t;

#include <stdbool.h>
#include <SDL.h>

struct rect_t{
	double x, y, w, h;
};

rect_t *rect_create(void);
void rect_delete(rect_t *rect);
void rect_init(rect_t *rect, double x, double y, double w, double h);

void rect_copy_to_sdl(rect_t *rect, SDL_Rect *sdl_rect);

double rect_get_l_edge(rect_t *rect);
void rect_set_l_edge(rect_t *rect, double n);

double rect_get_r_edge(rect_t *rect);
void rect_set_r_edge(rect_t *rect, double n);

double rect_get_t_edge(rect_t *rect);
void rect_set_t_edge(rect_t *rect, double n);

double rect_get_b_edge(rect_t *rect);
void rect_set_b_edge(rect_t *rect, double n);

double rect_get_mid_x(rect_t *rect);
void rect_set_mid_x(rect_t *rect, double n);

double rect_get_mid_y(rect_t *rect);
void rect_set_mid_y(rect_t *rect, double n);

void rect_set_size(rect_t *rect, double w, double h);
void rect_set_center(rect_t *rect, double x, double y);

double rect_get_area(rect_t *rect);

void rect_grow(rect_t *rect, double dw, double dh);

void rect_move_to(rect_t *rect, rect_t *other);
double rect_range_to(rect_t *rect, rect_t *other);
void rect_limit_to(rect_t *rect, rect_t *other);
void rect_match_to(rect_t *rect, rect_t *other);
double rect_angle_to(rect_t *rect, rect_t *other);

void rect_move_at_angle(rect_t *rect, double angle, double dist);
void rect_move_toward(rect_t *rect, rect_t *other, double dist);

bool rect_overlap(rect_t *rect, rect_t *other);

#endif

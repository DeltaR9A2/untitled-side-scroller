#include <stdlib.h>
#include <math.h>
//#include <tgmath.h>

#include "rect.h"

rect_t *rect_create(void){
	rect_t *rect = malloc(sizeof(rect_t));
	rect->x = 0;
	rect->y = 0;
	rect->w = 1;
	rect->h = 1;
	return rect;
}

void rect_delete(rect_t *rect){
	free(rect);
}

void rect_init(rect_t *rect, double x, double y, double w, double h){
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}

void rect_copy_to_sdl(rect_t *rect, SDL_Rect *sdl_rect){
	sdl_rect->x = (uint32_t)(rect->x);
	sdl_rect->y = (uint32_t)(rect->y);
	sdl_rect->w = (uint32_t)(rect->w);
	sdl_rect->h = (uint32_t)(rect->h);
}

double rect_get_l_edge(rect_t *rect){ return rect->x; }
void rect_set_l_edge(rect_t *rect, double n){ rect->x = n; }

double rect_get_r_edge(rect_t *rect){ return rect->x + rect->w; }
void rect_set_r_edge(rect_t *rect, double n){ rect->x = n - rect->w; }

double rect_get_t_edge(rect_t *rect){ return rect->y; }
void rect_set_t_edge(rect_t *rect, double n){ rect->y = n; }

double rect_get_b_edge(rect_t *rect){ return rect->y + rect->h; }
void rect_set_b_edge(rect_t *rect, double n){ rect->y = n - rect->h; }

double rect_get_mid_x(rect_t *rect){ return rect->x + (rect->w / 2.0); }
void rect_set_mid_x(rect_t *rect, double n){ rect->x = n - (rect->w / 2.0); }

double rect_get_mid_y(rect_t *rect){ return rect->y + (rect->h / 2.0); }
void rect_set_mid_y(rect_t *rect, double n){ rect->y = n - (rect->h / 2.0); }

void rect_set_center(rect_t *rect, double x, double y){
	rect_set_mid_x(rect, x);
	rect_set_mid_y(rect, y);
}

void rect_set_size(rect_t *rect, double w, double h){
	rect->w = w;
	rect->h = h;
}

double rect_get_area(rect_t *rect){
	return rect->w * rect->h;
}

void rect_grow(rect_t *rect, double dw, double dh){
	double mid_x = rect_get_mid_x(rect);
	double mid_y = rect_get_mid_y(rect);
	
	rect->w += dw;
	rect->h += dh;
	
	rect_set_center(rect, mid_x, mid_y);
}

void rect_move_to(rect_t *rect, rect_t *other){
	rect_set_mid_x(rect, rect_get_mid_x(other));	
	rect_set_mid_y(rect, rect_get_mid_y(other));
}

double rect_range_to(rect_t *rect, rect_t *other){
	return sqrt(pow(rect_get_mid_x(other) - rect_get_mid_x(rect), 2) + 
				pow(rect_get_mid_y(other) - rect_get_mid_y(rect), 2));
}

void rect_limit_to(rect_t *rect, rect_t *other){
	if(rect->w >= other->w){
		rect_set_mid_x(rect, rect_get_mid_x(other));
	}else if(rect_get_l_edge(rect) < rect_get_l_edge(other)){
		rect_set_l_edge(rect, rect_get_l_edge(other));
	}else if(rect_get_r_edge(rect) > rect_get_r_edge(other)){
		rect_set_r_edge(rect, rect_get_r_edge(other));
	}

	if(rect->h >= other->h){
		rect_set_mid_y(rect, rect_get_mid_y(other));
	}else if(rect_get_t_edge(rect) < rect_get_t_edge(other)){
		rect_set_t_edge(rect, rect_get_t_edge(other));
	}else if(rect_get_b_edge(rect) > rect_get_b_edge(other)){
		rect_set_b_edge(rect, rect_get_b_edge(other));
	}
}

void rect_match_to(rect_t *rect, rect_t *other){
	rect->x = other->x;
	rect->y = other->y;
	rect->w = other->w;
	rect->h = other->h;
}

double rect_angle_to(rect_t *rect, rect_t *other){
	return atan2(rect_get_mid_x(other) - rect_get_mid_x(rect),
				 rect_get_mid_y(other) - rect_get_mid_y(rect));
}

void rect_move_at_angle(rect_t *rect, double angle, double dist){
	rect_set_mid_x(rect, rect_get_mid_x(rect) + (dist * cos(angle)));
	rect_set_mid_y(rect, rect_get_mid_y(rect) + (dist * sin(angle)));
}

void rect_move_toward(rect_t *rect, rect_t *other, double dist){
	if(rect_range_to(rect, other) > dist){
		rect_move_at_angle(rect, rect_angle_to(rect, other), dist);
	}else{
		rect_move_to(rect, other);
	}
}

bool rect_overlap(rect_t *rect, rect_t *other){
		if((rect->w == 0) || (rect->h == 0) || (other->w == 0) || (other->h == 0)){
			return false;
		}

		if((rect->x + rect->w) <= (other->x) || (rect->x) >= (other->x + other->w) ||
		   (rect->y + rect->h) <= (other->y) || (rect->y) >= (other->y + other->h)){
			return false;
		}else{
			return true;
		}
}

/*
	def union(self, other):
		"""Expands ``self`` exactly enough to cover the entire area of ``other``."""
		new_l = min(self.l_edge, other.l_edge)
		new_t = min(self.t_edge, other.t_edge)
		new_r = max(self.r_edge, other.r_edge)
		new_b = max(self.b_edge, other.b_edge)
		self.x = new_l
		self.y = new_t
		self.w = new_r - new_l
		self.h = new_b - new_t
*/


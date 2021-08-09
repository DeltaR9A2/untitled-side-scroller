#include <stdlib.h>

#include "cmap.h"

const static int32_t GRID_SIZE = 8;

cmap_t *cmap_create(void){
	cmap_t *cmap = malloc(sizeof(cmap_t));
	cmap->rect = rect_create();
	cmap->data = NULL;	  
	return cmap;
}

void cmap_init(cmap_t *cmap, int32_t x, int32_t y, int32_t w, int32_t h){
	rect_init(cmap->rect, x, y, w, h);
	cmap->data = malloc(sizeof(int8_t)*(w*h));
}

void cmap_delete(cmap_t *cmap){
	free(cmap->data);
	rect_delete(cmap->rect);
}

int32_t cmap_xy_to_i(cmap_t *cmap, int32_t x, int32_t y){
	return (int32_t)((x - cmap->rect->x) + ((y - cmap->rect->y) * cmap->rect->w));
}

#define CMAP_ITERATION_START \
	for(int32_t y = (int32_t)rect_get_t_edge(cmap->rect); y < rect_get_b_edge(cmap->rect); y++){ \
		for(int32_t x = (int32_t)rect_get_l_edge(cmap->rect); x < rect_get_r_edge(cmap->rect); x++){ \
			int32_t i = cmap_xy_to_i(cmap, x, y);

#define CMAP_ITERATION_STOP \
		} \
	}

#define CMAP_RECT_ITERATION_START \
	for(int32_t y = (int32_t)rect_get_t_edge(rect); y < rect_get_b_edge(rect); y++){ \
		for(int32_t x = (int32_t)rect_get_l_edge(rect); x < rect_get_r_edge(rect); x++){ \
			int32_t i = cmap_xy_to_i(cmap, x, y);

#define CMAP_RECT_ITERATION_STOP \
		} \
	}

void cmap_reset(cmap_t *cmap){
	CMAP_ITERATION_START;

	if(cmap->data[i] > 0){
		cmap->data[i] = 1;
	}else{
		cmap->data[i] = 0;
	}

	CMAP_ITERATION_STOP;
}

void cmap_copy_data_from(cmap_t *cmap, int8_t *data){
	CMAP_ITERATION_START;

	cmap->data[i] = data[i];

	CMAP_ITERATION_STOP;
}

void cmap_rect_mark(cmap_t *cmap, rect_t *rect){
	CMAP_RECT_ITERATION_START;

	cmap->data[i] = 2;

	CMAP_RECT_ITERATION_STOP;
}

bool cmap_xy_is_valid(cmap_t *cmap, int32_t x, int32_t y){
	return	(x >= rect_get_l_edge(cmap->rect)) &&
			(x < rect_get_r_edge(cmap->rect)) &&
			(y >= rect_get_t_edge(cmap->rect)) &&
			(y < rect_get_b_edge(cmap->rect));
}

bool cmap_rect_check(cmap_t *cmap, rect_t *rect){
	CMAP_RECT_ITERATION_START;

	if(!cmap_xy_is_valid(cmap, x, y) || cmap->data[i] == 0){
		return false;
	}
	
	CMAP_RECT_ITERATION_STOP;
	
	return true;
}

void cmap_add_to_rect_list(cmap_t *cmap, rect_list_t *rects){
	rect_t *current_rect;
	
	CMAP_ITERATION_START;

	if(cmap->data[i] == 1){
		current_rect = rect_list_get(rects);
		rect_init(current_rect, x, y, 1, 1);
		
		while(cmap_rect_check(cmap, current_rect)){
			current_rect->w += 1;
		}
		current_rect->w -= 1;

		while(cmap_rect_check(cmap, current_rect)){
			current_rect->h += 1;
		}
		current_rect->h -= 1;
		
		cmap_rect_mark(cmap, current_rect);
		
		current_rect->x *= GRID_SIZE;
		current_rect->y *= GRID_SIZE;
		current_rect->w *= GRID_SIZE;
		current_rect->h *= GRID_SIZE;
	}
	
	CMAP_ITERATION_STOP;
}


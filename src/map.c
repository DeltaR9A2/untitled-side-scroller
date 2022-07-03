#include <stdio.h>

#include "map.h"
#include "sprite.h"
#include "stb_ds.h"

const uint32_t BLOCKED_L = 0x00000001;
const uint32_t BLOCKED_R = 0x00000002;
const uint32_t BLOCKED_U = 0x00000004;
const uint32_t BLOCKED_D = 0x00000008;
const uint32_t BLOCKED_MASK = 0x0000000F;

const uint32_t PLAT_DROP = 0x00000010;


struct body_t{
	rect_t *rect;
	double vx, vy;
	uint32_t flags;
};

typedef struct cmap_t cmap_t;

struct cmap_t{
	rect_t *rect;
	int8_t *data;
};

struct map_t{
	rect_t rect;

	SDL_Surface *image;
	
	rect_t *terrain_rects;
	rect_t *platform_rects;
};

body_t *body_create(void){
	body_t *body = malloc(sizeof(body_t));
	body->rect = rect_create();
	body->vx = 0;
	body->vy = 0;
	body->flags = 0;
	return body;
}

void body_delete(body_t *body){
	rect_delete(body->rect);
	free(body);
}

rect_t *body_get_rect(body_t *body){
	return body->rect;
}

void body_set_size(body_t *body, double w, double h){
	rect_set_size(body->rect, w, h);
}

void body_move_to(body_t *body, rect_t *rect){
	rect_move_to(body->rect, rect);
}

void body_set_pos(body_t *body, double x, double y){
    body->rect->x = x;
    body->rect->y = y;
}

void body_set_velocity(body_t *body, double vx, double vy){
	body->vx = vx;
	body->vy = vy;
}

void body_get_velocity(body_t *body, double *vx, double *vy){
        *vx = body->vx;
        *vy = body->vy;
}

void body_set_flags(body_t *body, uint32_t flags){
    body->flags |= flags;
}

void body_unset_flags(body_t *body, uint32_t flags){
    body->flags &= ~flags;
}

bool body_check_flags(body_t *body, uint32_t flags){
    return (body->flags & flags) == flags;
}

void do_physics_to_it(body_t *body, map_t *map){
    rect_t *terr_rects = map->terrain_rects;
    rect_t *plat_rects = map->platform_rects;
    
	rect_t start = (rect_t){0,0,0,0};
	rect_match_to(&start, body->rect);
	
	body->flags &= ~BLOCKED_MASK;
	
	body->rect->x += body->vx;
	for(int i=0; i<arrlen(terr_rects); i++){
        rect_t *curr_rect = &terr_rects[i];
		if(rect_overlap(body->rect, curr_rect)){
			if(body->vx > 0 && rect_get_r_edge(body->rect) >= rect_get_l_edge(curr_rect)){
				body->vx = 0;
				rect_set_r_edge(body->rect, rect_get_l_edge(curr_rect));
				body->flags |= BLOCKED_R;
			}else if(body->vx < 0 && rect_get_l_edge(body->rect) <= rect_get_r_edge(curr_rect)){
				body->vx = 0;
				rect_set_l_edge(body->rect, rect_get_r_edge(curr_rect));
				body->flags |= BLOCKED_L;
			}
			break;
		}
	}

	body->rect->y += body->vy;
	for(int i=0; i<arrlen(terr_rects); i++){
        rect_t *curr_rect = &terr_rects[i];
		if(rect_overlap(body->rect, curr_rect)){
			if(body->vy > 0 && rect_get_b_edge(body->rect) >= rect_get_t_edge(curr_rect)){
				body->vy = 0;
				rect_set_b_edge(body->rect, rect_get_t_edge(curr_rect));
				body->flags |= BLOCKED_D;
			}else if(body->vy < 0 && rect_get_t_edge(body->rect) <= rect_get_b_edge(curr_rect)){
				body->vy = 0;
				rect_set_t_edge(body->rect, rect_get_b_edge(curr_rect));
				body->flags |= BLOCKED_U;
			}
			break;
		}
	}
	
	if(body->vy > 0 && !(body->flags & PLAT_DROP)){
        for(int i=0; i<arrlen(plat_rects); i++){
            rect_t *curr_rect = &plat_rects[i];
			if(rect_overlap(body->rect, curr_rect)){
				if(rect_get_b_edge(&start) <= rect_get_t_edge(curr_rect)){
					body->vy = 0;
					rect_set_b_edge(body->rect, rect_get_t_edge(curr_rect));
					body->flags |= BLOCKED_D;
					break;
				}

			}
		}
	}
}



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

void cmap_add_to_rect_list(cmap_t *cmap, rect_t **rects){
	rect_t *current_rect;
	
	CMAP_ITERATION_START;

    current_rect = rect_create();

	if(cmap->data[i] == 1){
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
	
    arrput(*rects, *current_rect);
    
	CMAP_ITERATION_STOP;
}

static struct { char *key; map_t *value; } *map_index = NULL;

map_t *map_create(void){
	map_t *map = malloc(sizeof(map_t));

	map->rect = (rect_t){0,0,0,0};
	
	map->image = NULL;

	map->terrain_rects = NULL;
	map->platform_rects = NULL;
	
	return map;
}

void map_delete(map_t *map){
	arrfree(map->platform_rects);
	arrfree(map->terrain_rects);
	
	free(map);
}

void map_init(map_t *map, const char *data_fn, const char *image_fn){
	SDL_Surface *map_data = load_image(data_fn);

	rect_init(&map->rect, 0, 0, map_data->w * 8, map_data->h * 8);

	cmap_t *terrain_cmap = cmap_create();
	cmap_init(terrain_cmap, 0, 0, map_data->w, map_data->h);

	cmap_t *platform_cmap = cmap_create();
	cmap_init(platform_cmap, 0, 0, map_data->w, map_data->h);

	for(int i=0; i < map_data->w * map_data->h; i++){
		uint32_t pixel = ((Uint32 *)map_data->pixels)[i];

		if(pixel == 0x333366FF){
			terrain_cmap->data[i] = 1;
			platform_cmap->data[i] = 0;
		}else if(pixel == 0x9999DDFF){
			terrain_cmap->data[i] = 0;
			platform_cmap->data[i] = 1;
		}else{
			terrain_cmap->data[i] = 0;
			platform_cmap->data[i] = 0;
		}
	}

	cmap_add_to_rect_list(terrain_cmap, &map->terrain_rects);
	cmap_delete(terrain_cmap);

	cmap_add_to_rect_list(platform_cmap, &map->platform_rects);
	cmap_delete(platform_cmap);

	map->image = load_image(image_fn);
}

void map_update(map_t *map){
    return;
}

map_t *map_load(const char *map_name){
	map_t *this_map = shget(map_index, map_name);
    
    if(this_map == NULL){
        this_map = map_create();

        char map_image_fn[256]; // 256 = max posix fn length + terminator
        char map_rects_fn[256];
        snprintf(map_image_fn, 256, "%s_image.png", map_name);
        snprintf(map_rects_fn, 256, "%s_rects.png", map_name);
        map_init(this_map, map_rects_fn, map_image_fn);
    }

    return this_map;
}

SDL_Surface *map_get_image(map_t *map){
    return map->image;
}

rect_t *map_get_rect(map_t *map){
    return &map->rect;
}
rect_t *map_get_terrain_rects(map_t *map){
    return map->terrain_rects;
}

rect_t *map_get_platform_rects(map_t *map){
    return map->platform_rects;
}

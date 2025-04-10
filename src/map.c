#include <stdio.h>

#include "map.h"
#include "sprite.h"
#include "stb_ds.h"
//#include "tinydir.h"

const int32_t GRID_SIZE = 8; // 1px terrain = GRID_SIZE px visual

typedef struct cmap_t{ // cmap means collision map 
	int32_t w, h;
	int8_t *data;
} cmap_t;

cmap_t *cmap_create(int32_t x, int32_t y, int32_t w, int32_t h){
	cmap_t *cmap = malloc(sizeof(cmap_t));
	cmap->w = w; cmap->h = h;
	cmap->data = malloc(sizeof(int8_t)*(w*h));
	return cmap;
}

void cmap_delete(cmap_t *cmap){
	free(cmap->data);
	free(cmap);
}

inline void cmap_set(cmap_t *cmap, int32_t x, int32_t y, int8_t n){
	cmap->data[ (x + (y * cmap->w)) ] = n;
}

inline int8_t cmap_get(cmap_t *cmap, int32_t x, int32_t y){
	return cmap->data[ (x + (y * cmap->w)) ];
}

void cmap_rect_mark(cmap_t *cmap, rect_t *rect){
	for(int32_t y = (int32_t)rect_get_t_edge(rect); y < rect_get_b_edge(rect); y++){
		for(int32_t x = (int32_t)rect_get_l_edge(rect); x < rect_get_r_edge(rect); x++){
			cmap_set(cmap, x, y, 2);
		}
	}
}

inline bool cmap_xy_is_valid(cmap_t *cmap, int32_t x, int32_t y){
	return	(x >= 0) &&
			(x <  cmap->w) &&
			(y >= 0) &&
			(y <  cmap->h);
}

bool cmap_rect_check(cmap_t *cmap, rect_t *rect){
	for(int32_t y = (int32_t)rect_get_t_edge(rect); y < rect_get_b_edge(rect); y++){
		for(int32_t x = (int32_t)rect_get_l_edge(rect); x < rect_get_r_edge(rect); x++){
			if(cmap_xy_is_valid(cmap, x, y) && cmap_get(cmap, x, y)){
				continue;
			}else{
				return false;
			}
		}
	}
	
	return true;
}

void cmap_add_to_rect_list(cmap_t *cmap, rect_t **rects){
	rect_t *current_rect;
	
	for(int32_t y = 0; y < cmap->h; y++){ 
		for(int32_t x = 0; x < cmap->w; x++){ 
			if(cmap_get(cmap, x, y) == 1){
				current_rect = rect_create();
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

				arrput(*rects, *current_rect);
			}
		}
	}
}

map_t *map_create(void){
	map_t *map = malloc(sizeof(map_t));

	map->rect = rect_create();
	map->terrain_rects = NULL;
	map->platform_rects = NULL;
	map->image = NULL;

	return map;
}

void map_delete(map_t *map){
	arrfree(map->platform_rects);
	arrfree(map->terrain_rects);
	rect_delete(map->rect);
	free(map);
}

map_t *map_load_by_name(const char *map_name){
	char map_terrain_fn[1024];
	snprintf(map_terrain_fn, 1024, "./map-%s-terrain.png", map_name);
	SDL_Surface *map_terrain_data = load_image(map_terrain_fn);

	if(map_terrain_data == NULL){ 
		return NULL; 
	}

	map_t *map = map_create();

	char map_image_fn[1024];
	snprintf(map_image_fn, 1024, "./map-%s-visual.png", map_name);
	map->image = load_image(map_image_fn);

	rect_init(map->rect, 0, 0, map_terrain_data->w * GRID_SIZE, map_terrain_data->h * GRID_SIZE);

	cmap_t *terrain_cmap  = cmap_create(0, 0, map_terrain_data->w, map_terrain_data->h);
	cmap_t *platform_cmap = cmap_create(0, 0, map_terrain_data->w, map_terrain_data->h);

	for(int i=0; i < map_terrain_data->w * map_terrain_data->h; i++){
		uint32_t pixel = ((Uint32 *)map_terrain_data->pixels)[i];

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

	return map;
}

#include <stdio.h>

#include "map.h"
#include "sprite.h"
#include "stb_ds.h"
#include "tinydir.h"

const int32_t GRID_SIZE = 16;
const double WORLD_CELL_W = 128.0;
const double WORLD_CELL_H = 128.0;

typedef struct cmap_t cmap_t; //= cmap means collision map 

struct cmap_t{
	rect_t *rect;
	int8_t *data;
};

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
          (x <  rect_get_r_edge(cmap->rect)) &&
          (y >= rect_get_t_edge(cmap->rect)) &&
          (y <  rect_get_b_edge(cmap->rect));
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

#undef CMAP_ITERATION_START
#undef CMAP_ITERATION_STOP
#undef CMAP_RECT_ITERATION_START
#undef CMAP_RECT_ITERATION_STOP 
//=========================================================

#define WORLD_SIZE 256
static map_t* WORLD_MAP_GRID[WORLD_SIZE][WORLD_SIZE];

//static struct { char *key; map_t *value; } *map_index = NULL;

map_t *map_create(void){
	map_t *map = malloc(sizeof(map_t));

	map->rect = rect_create();
	map->terrain_rects = NULL;
	map->platform_rects = NULL;
	map->image = NULL;
    
  map->world_x = 0;
  map->world_y = 0;

	return map;
}

void map_delete(map_t *map){
	arrfree(map->platform_rects);
	arrfree(map->terrain_rects);
	rect_delete(map->rect);
	free(map);
}
void map_update(map_t *map){
  return;
}

double map_get_w(map_t *map){
  return map->rect->w;
}

double map_get_h(map_t *map){
  return map->rect->h;
}

double map_get_cell_w(map_t *map){
  return (uint32_t)ceil(map->rect->w / WORLD_CELL_W);
}

double map_get_cell_h(map_t *map){
  return (uint32_t)ceil(map->rect->h / WORLD_CELL_H);
}

map_t *map_load_by_name(const char *map_name){
  map_t *map = map_create();
    
  char map_rects_fn[1024];
  snprintf(map_rects_fn, 1024, "./maps/%s/terrain-int.png", map_name);
	SDL_Surface *map_rects = load_image(map_rects_fn);
    
  if(map_rects == NULL){ 
    map_delete(map);
    return NULL; 
  }

  char map_image_fn[1024];
  snprintf(map_image_fn, 1024, "./maps/%s/_composite.png", map_name);
	map->image = load_image(map_image_fn);

	rect_init(map->rect, 0, 0, map_rects->w * GRID_SIZE, map_rects->h * GRID_SIZE);

	cmap_t *terrain_cmap = cmap_create();
	cmap_init(terrain_cmap, 0, 0, map_rects->w, map_rects->h);

	cmap_t *platform_cmap = cmap_create();
	cmap_init(platform_cmap, 0, 0, map_rects->w, map_rects->h);

	for(int i=0; i < map_rects->w * map_rects->h; i++){
		uint32_t pixel = ((Uint32 *)map_rects->pixels)[i];

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

map_t *map_load_by_coords(int32_t x, int32_t y){
  map_t *map = WORLD_MAP_GRID[x][y];

  if(map != NULL){ return map; }

  char map_name[1024];
  snprintf(map_name, 1024, "map_%i_%i", x, y);   

  map = map_load_by_name(map_name);

  if(map == NULL){ return NULL; }

  map->world_x = x;
  map->world_y = y;

  uint32_t cell_w = (uint32_t)map_get_cell_w(map);
  uint32_t cell_h = (uint32_t)map_get_cell_h(map);
        
  for(int ix=0; ix<cell_w; ix++){
    for(int iy=0; iy<cell_h; iy++){
      WORLD_MAP_GRID[x+ix][y+iy] = map;
    }
  }

  return map;
}

bool map_exists_at_coords(int32_t x, int32_t y){
  return !(WORLD_MAP_GRID[x][y] == NULL);
}

void map_attempt_preload(const char* map_name){
  char *token = NULL;
  char name_buffer[32];
    
  strncpy(name_buffer, map_name, 32);
  token = strtok(name_buffer, "_");
    
  if(strncmp(token, "map", 3) == 0){
    int x = 0;
    int y = 0;
        
    token = strtok(NULL, "_");
    x = atoi(token);
    token = strtok(NULL, "_");
    y = atoi(token);
        
    map_t *map = map_load_by_coords(x,y); 
    if(map == NULL){
      printf("Failed to preload map named %s \n", map_name);
    }
  }
}

void map_preload_all(void){
  tinydir_dir dir;
  tinydir_file file;

  tinydir_open(&dir, "./maps");

  while(dir.has_next){
    tinydir_readfile(&dir, &file);
    map_attempt_preload(file.name);
    tinydir_next(&dir);
  }

  tinydir_close(&dir);
}

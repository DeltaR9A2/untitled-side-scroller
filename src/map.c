#include "map.h"
#include "sprite.h"
#include "stb_ds.h"
#include <stdio.h>

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

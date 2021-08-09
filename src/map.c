#include "map.h"

map_t *map_create(void){
	map_t *map = malloc(sizeof(map_t));

	map->rect = rect_create();
	
	map->image = NULL;

	map->terrain_rects = rect_list_create();
	map->platform_rects = rect_list_create();
	
	map->targets = target_dict_create();
	map->items = item_list_create();
	
	return map;
}

void map_delete(map_t *map){
	item_list_delete(map->items);
	target_dict_delete(map->targets);

	rect_list_delete(map->platform_rects);
	rect_list_delete(map->terrain_rects);
	
	rect_delete(map->rect);
	
	free(map);
}

void map_init(map_t *map, const char *data_fn, const char *image_fn){
	SDL_Surface *map_data = load_image(data_fn);

	rect_init(map->rect, 0, 0, map_data->w * 8, map_data->h * 8);

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

	cmap_add_to_rect_list(terrain_cmap, map->terrain_rects);
	cmap_delete(terrain_cmap);

	cmap_add_to_rect_list(platform_cmap, map->platform_rects);
	cmap_delete(platform_cmap);

	map->image = load_image(image_fn);
}

void map_update(map_t *map){
	for(target_node_t *iter = map->targets->head; iter; iter = iter->next){
		if (iter->data->sprite != NULL){
			iter->data->sprite->step += 1;
		}
	}
	
	for(item_node_t *iter = map->items->head; iter; iter = iter->next){
		if(iter->data->flags & ITEM_ALIVE){
			item_update(iter->data);
			do_physics_to_it(iter->data->body, map->terrain_rects, map->platform_rects);
		}
	}

}

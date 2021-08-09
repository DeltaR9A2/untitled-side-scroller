#include <stdlib.h>

#include "camera.h"

camera_t *camera_create(void){
	camera_t *camera = malloc(sizeof(camera_t));
	camera->view = rect_create();
	camera->bounds = rect_create();
	camera->buffer = NULL;
	camera->fade_buffer = NULL;
	#ifdef DEBUG
	camera->debug_buffer = NULL;
	#endif
	
	return camera;
}

void camera_init(camera_t *camera, int32_t w, int32_t h){
	camera->view->w = w;
	camera->view->h = h;
	camera->buffer = create_surface(w, h);
	camera->fade_buffer = create_surface(w, h);
	
	camera_set_fade(camera, 0x00000000);
	
	#ifdef DEBUG
	camera->debug_buffer = create_surface(w, h);
	#endif
}

void camera_delete(camera_t *camera){
	rect_delete(camera->view);
	SDL_FreeSurface(camera->buffer);

	#ifdef DEBUG
	SDL_FreeSurface(camera->debug_buffer);
	#endif
}

void camera_set_fade(camera_t *camera, int32_t color){
  SDL_FillRect(camera->fade_buffer, NULL, color);
}

void camera_fill_rect(camera_t *camera, rect_t *rect, int32_t color){
	SDL_Rect fill_rect;

	rect_copy_to_sdl(rect, &fill_rect);
	
	fill_rect.x -= (int)camera->view->x;
	fill_rect.y -= (int)camera->view->y;

	SDL_FillRect(camera->buffer, &fill_rect, color);
}

#ifdef DEBUG
void camera_debug_rect(camera_t *camera, rect_t *rect, int32_t color){
	SDL_Rect fill_rect;

	rect_copy_to_sdl(rect, &fill_rect);
	
	fill_rect.x -= (int)camera->view->x;
	fill_rect.y -= (int)camera->view->y;

	SDL_FillRect(camera->debug_buffer, &fill_rect, color);
}

void camera_draw_terrain_rects(camera_t *camera, game_t *game){
	rect_node_t *iter = game->active_map->terrain_rects->head;
	while(iter != NULL){
		camera_debug_rect(camera, iter->data, 0x33336644);
		iter = iter->next;
	}
}

void camera_draw_platform_rects(camera_t *camera, game_t *game){
	rect_node_t *iter = game->active_map->platform_rects->head;
	while(iter != NULL){
		camera_debug_rect(camera, iter->data, 0x7777AA44);
		iter = iter->next;
	}
}

void camera_draw_debug_info(camera_t *camera, game_t *game){
	SDL_Rect fill_rect;

	char buffer[128] = "\0";

	int line_no = 0;
	int line_h = font_get_height(game->debug_font) + 2;

	fill_rect.x = 0;
	fill_rect.y = 0;
	fill_rect.w = 150;
	fill_rect.h = (5*line_h)+6;

	SDL_FillRect(camera->debug_buffer, &fill_rect, 0x000000AA);
	
	#define PRINT_DEBUG_LINE font_draw_string(game->debug_font, buffer, 4, 3 + (line_no * line_h), camera->debug_buffer); line_no++;

	sprintf(buffer, "Win Size: %ix%i", game->core->win_cw, game->core->win_ch);
	PRINT_DEBUG_LINE

	sprintf(buffer, "Scale: %04.2f", core_get_scale(game->core));
	PRINT_DEBUG_LINE

	sprintf(buffer, "Time: %06.2fs", ((double)game->step)/200.0);
	PRINT_DEBUG_LINE

	sprintf(buffer, "Player Pos: %4.0f,%4.0f", game->player->body->rect->x, game->player->body->rect->y);
	PRINT_DEBUG_LINE

	int npc_x = ((int)(game->player->body->rect->x/8.0)*8);
	int npc_y = ((int)((game->player->body->rect->y-26)/8.0)*8)+8;

	sprintf(buffer, "NPC Pos: %i,%i", npc_x, npc_y);
	PRINT_DEBUG_LINE

	#undef PRINT_DEBUG_LINE
}
#endif

void camera_draw_sprite(camera_t *camera, sprite_t *sprite){
	SDL_Rect draw_rect;

	rect_copy_to_sdl(sprite->rect, &draw_rect);
	
	draw_rect.x -= (int)camera->view->x;
	draw_rect.y -= (int)camera->view->y;

 	anim_draw(sprite->anim, sprite->step, camera->buffer, &draw_rect);
}

void camera_draw_surface(camera_t *camera, SDL_Surface *surface){
	SDL_Rect draw_rect;

	draw_rect.x = 0 - (int)camera->view->x;
	draw_rect.y = 0 - (int)camera->view->y;
	draw_rect.w = surface->w;
	draw_rect.h = surface->h;

	SDL_BlitSurface(surface, NULL, camera->buffer, &draw_rect);
}

void camera_draw_player(camera_t *camera, player_t *player){
	
	if(player->flashing % 2 == 0){
		#ifdef DEBUG
		camera_debug_rect(camera, player->sprite->rect, 0x22DD2244);
		camera_debug_rect(camera, player->body->rect, 0x2222DD44);
		#endif
		
		camera_draw_sprite(camera, player->sprite);
	}
}

void camera_draw_targets(camera_t *camera, game_t *game)
{
	target_node_t *iter = game->active_map->targets->head;
	while(iter != NULL)
	{

		if(iter->data->sprite != NULL){
			#ifdef DEBUG
			camera_debug_rect(camera, iter->data->sprite->rect, 0xDDAA2222);
			#endif

			camera_draw_sprite(camera, iter->data->sprite);
		}
		
		iter = iter->next;
	}
}

void camera_draw_items(camera_t *camera, game_t *game){
	item_node_t *iter = game->active_map->items->head;
	while(iter != NULL){
		if(iter->data->flags & ITEM_ALIVE){
			if(iter->data->sprite != NULL){
				#ifdef DEBUG
				camera_debug_rect(camera, iter->data->sprite->rect, 0x22AADD22);
				#endif
				camera_draw_sprite(camera, iter->data->sprite);
			}
		}
		
		iter = iter->next;
	}
}

void camera_draw_game(camera_t *camera, game_t *game){
	camera->view->x = floor(camera->view->x);
	camera->view->y = floor(camera->view->y);

	SDL_FillRect(camera->buffer, NULL, 0xDDDDDDFF);

	#ifdef DEBUG
	SDL_FillRect(camera->debug_buffer, NULL, 0x00000000);
	#endif

	rect_limit_to(camera->view, camera->bounds);

	#ifdef DEBUG
	camera_draw_terrain_rects(camera, game);
	camera_draw_platform_rects(camera, game);
	#endif
	
	camera_draw_surface(camera, game->active_map->image);
	camera_draw_targets(camera, game);
	camera_draw_items(camera, game);

	camera_draw_player(camera, game->player);

	#ifdef DEBUG
	camera_draw_debug_info(camera, game);
	
	SDL_BlitSurface(camera->debug_buffer, NULL, camera->buffer, NULL);
	#endif

  SDL_BlitSurface(camera->fade_buffer, NULL, camera->buffer, NULL);
}



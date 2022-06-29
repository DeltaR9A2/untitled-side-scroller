#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "game.h"
#include "rect.h"

const uint32_t GAME_MODE_MENU = 0;
const uint32_t GAME_MODE_PLAY = 1;
const uint32_t GAME_MODE_DIALOGUE = 2;

const uint32_t GAME_MESSAGE_LEN = 128;

void game_create_data_structures(game_t *game);
void game_delete_data_structures(game_t *game);

void game_set_message(game_t *game, const char *text){
	game->message_timeout = 240;
	sprintf(game->message, "%s", text);

	SDL_Rect fill_rect;
	fill_rect.x = 0;
	fill_rect.y = 0;
	fill_rect.w = game->message_surface->w;
	fill_rect.h = game->message_surface->h;
	SDL_FillRect(game->message_surface, &fill_rect, 0x666666FF);

	fill_rect.x += 2;
	fill_rect.y += 2;
	fill_rect.w -= 4;
	fill_rect.h -= 4;
	SDL_FillRect(game->message_surface, &fill_rect, 0x333333FF);

	font_draw_string(game->font, game->message, 8, 4, game->message_surface);
}

void game_update_map(game_t *game){
	if(game->active_map == NULL){ return; }
	
	map_update(game->active_map);

//	target_t *nearest_target = NULL;
//	int32_t nearest_distance = 9999;
//	int32_t current_distance = 9999;

	rect_t *player_rect = player_get_rect(game->player);

	/*for(target_node_t *iter = game->active_map->targets->head; iter; iter = iter->next){
		if(iter->data->sprite != NULL){
			current_distance = (int32_t)rect_range_to(iter->data->rect, player_rect);
			if(current_distance < nearest_distance){
				nearest_distance = current_distance;
				nearest_target = iter->data;
			}
		}
	}*/

	/*if(nearest_distance <= 32 && nearest_target != NULL){
		game->active_target = nearest_target;
	}else{
		game->active_target = NULL;
	}*/
}

static void game_add_default_map(game_t *game){
	map_t *map = map_dict_get(game->maps, "default");
	map_init(map, "map_default.png", "map_default_image.png");
}

game_t *game_create(core_t *core){
	game_t *game = malloc(sizeof(game_t));

	game->core = core;
	game->step = 0;
	game->mode = GAME_MODE_PLAY;

	game_create_data_structures(game);

	game->font = font_create("font_nokia.png");
	game->debug_font = font_create("font_nokia.png");

	game->menu = menu_create_main_menu(game);
	
	camera_init(game->camera, 640, 360);

    game->message = calloc(GAME_MESSAGE_LEN, sizeof(char));
    game->message_surface = create_surface(640-256, 6+font_get_height(game->font));
	game->message_timeout = 0;

	game_add_default_map(game);
	game_select_map(game, "default");
	player_move_to_coord(game->player, 128, 128);

	map_t *test_map = map_dict_get(game->maps, "test_map");
	map_init(test_map, "map_test.png", "map_test_image.png");
	
//	target_t *spawn_target = target_dict_get(test_map->targets, "new_game_spawn");
//	target_set_rect_numbers(spawn_target, 200, 136, 64, 64);
	
	game_select_map(game, "test_map");
	
	sprite_anim_set(player_get_sprite(game->player), anim_get("player_idle_r"));
	player_update(game->player, game);
	
	game_update_map(game);

	return game;
}

void game_delete(game_t *game){
	free(game->message);

	menu_delete(game->menu);	
	font_delete(game->font);

	game_delete_data_structures(game);

	free(game);
}

void game_select_map(game_t *game, const char *map_name){
	game->active_map = map_dict_get(game->maps, map_name);
    // game->active_target = NULL;
	
	rect_match_to(game->camera->bounds, game->active_map->rect);
}

void game_fast_frame(game_t *game){
	game->step += 1;

	if(game->mode == GAME_MODE_MENU){
		if(controller_just_pressed(game->controller, BTN_U)){
			menu_up(game->menu);
		}
		if(controller_just_pressed(game->controller, BTN_D)){
			menu_down(game->menu);
		}
		if(controller_just_pressed(game->controller, BTN_A)){
			menu_activate(game->menu);
		}
		if(controller_just_pressed(game->controller, BTN_START)){
			menu_activate(game->menu);
		}

	}else if(game->mode == GAME_MODE_PLAY){
		player_update(game->player, game);
		game_update_map(game);

		/*if(controller_just_pressed(game->controller, BTN_A)){
			if(game->active_target != NULL){
				target_activate(game->active_target, game);
			}
		}*/
		
		if(controller_just_pressed(game->controller, BTN_START)){
			game->mode = GAME_MODE_MENU;
			camera_set_fade(game->camera, 0x000000CC);
		}
	}
}

void game_full_frame(game_t *game){
	SDL_Rect draw_rect;

	game_fast_frame(game);

	rect_move_to(game->camera->view, player_get_rect(game->player));
	camera_draw_game(game->camera, game);
	SDL_BlitSurface(game->camera->buffer, NULL, game->core->screen, NULL);

	if(game->mode == GAME_MODE_MENU){
		menu_draw(game->menu, game->core->screen);
	}

	if(game->message_timeout > 0){
		game->message_timeout -= 1;
		draw_rect.x = 8;
		draw_rect.y = 360 - (8+game->message_surface->h);

		SDL_BlitSurface(game->message_surface, NULL, game->core->screen, &draw_rect);
	}
	
}

void game_create_data_structures(game_t *game){
	game->controller = controller_create();
	game->camera = camera_create();
	game->player = player_create();

	game->maps = map_dict_create();
}

void game_delete_data_structures(game_t *game){
	map_dict_delete(game->maps);

	player_delete(game->player);
	camera_delete(game->camera);
	controller_delete(game->controller);
}


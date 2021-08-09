#include <stdlib.h>
#include <stdint.h>

#include "menu.h"
#include "sdl_helpers.h"

const uint32_t OPTION_LABEL_LEN = 32;
const uint32_t MENU_MAX_OPTIONS = 8;

option_t *option_create(char *label, void (*action)(menu_t*)){
	option_t *option = malloc(sizeof(option_t));
	option->label = malloc(sizeof(char) * OPTION_LABEL_LEN);
	sprintf(option->label, label);
	option->action = action;
	return option;
}

void option_delete(option_t *option){
	free(option->label);
	free(option);
}

void option_activate(option_t *option, menu_t *menu){
	if(option->action != NULL){
		option->action(menu);
	}
}

menu_t *menu_create(game_t *game){
	menu_t *menu = malloc(sizeof(menu_t));

	menu->game = game;
	menu->font = game->font;
	
	menu->options = malloc(sizeof(option_t*) * MENU_MAX_OPTIONS);
	
	for(uint32_t i=0; i<MENU_MAX_OPTIONS; i++){
		menu->options[i] = NULL;
	}
	
	menu->buffer = NULL;
	
	menu->selection = 0;
	
	// Default position in top-left of screen
	menu->x_pos = 8;
	menu->y_pos = 8;
	
	return menu;
}

void menu_delete(menu_t *menu){
	for(uint32_t i=0; i<MENU_MAX_OPTIONS; i++){
		if(menu->options[i] != NULL){
			option_delete(menu->options[i]);
		}
	}
	
	SDL_FreeSurface(menu->buffer);
	
	free(menu);
}


void menu_delete_buffer(menu_t *menu){
	SDL_FreeSurface(menu->buffer);
	menu->buffer = NULL;
}

void menu_create_buffer(menu_t *menu){
	#ifdef DEBUG
	if(menu->buffer != NULL){
		printf("MEMORY LEAK: MENU BUFFER OVER-WRITE\n");
	}
	#endif

	int32_t x = 0;
	int32_t w = 0;
	int32_t h = 0;
	
	for(uint32_t i=0; i<MENU_MAX_OPTIONS; i++){
		if(menu->options[i] != NULL){
			x = font_get_width(menu->font, menu->options[i]->label);
			if(x > w){ w = x; }
			h += font_get_height(menu->font);
		}
	}
	
	w += 20;
	h += 8;
	
	menu->buffer = create_surface(w,h);
}

void menu_draw_buffer(menu_t *menu){
	if(menu->buffer == NULL){
		menu_create_buffer(menu);
	}

	SDL_Rect fill_rect;

	fill_rect.x = 0;
	fill_rect.y = 0;
	fill_rect.w = menu->buffer->w;
	fill_rect.h = menu->buffer->h;
	SDL_FillRect(menu->buffer, &fill_rect, 0x666666FF);

	fill_rect.x += 2;
	fill_rect.y += 2;
	fill_rect.w -= 4;
	fill_rect.h -= 4;
	SDL_FillRect(menu->buffer, &fill_rect, 0x333333FF);

	int x = 10;
	int y = 4;
	int h = font_get_height(menu->game->font);
	
	for(uint32_t i=0; i<MENU_MAX_OPTIONS; i++){
		if(menu->options[i] != NULL){
			font_draw_string(menu->game->font, menu->options[i]->label, x, y, menu->buffer);
		}
		if(i == menu->selection){
			font_draw_string(menu->game->font, ">", x-6, y, menu->buffer);
		}
		y += h;
	}
}

void menu_up(menu_t *menu){
	menu->selection -= 1;
	menu->selection %= menu->num_options;
	menu_draw_buffer(menu);
}

void menu_down(menu_t *menu){
	menu->selection += 1;
	menu->selection %= menu->num_options;
	menu_draw_buffer(menu);
}

void menu_activate(menu_t *menu){
	if(menu->options[menu->selection] != NULL){
		if(menu->options[menu->selection]->action != NULL){
			menu->options[menu->selection]->action(menu);
		}else{
			printf("ERROR: MENU: Option '%s' has NULL action.\n", menu->options[menu->selection]->label);
		}
	}
	menu_draw_buffer(menu);
}

void menu_draw(menu_t *menu, SDL_Surface *surface){
	if(menu->buffer == NULL){
		menu_draw_buffer(menu);
	}else{
		SDL_Rect draw_rect;
		draw_rect.x = menu->x_pos;
		draw_rect.y = menu->y_pos;
	
		SDL_BlitSurface(menu->buffer, NULL, surface, &draw_rect);
	}
}

void menu_add_option(menu_t *menu, char *label, void (*action)(menu_t*)){
	menu_delete_buffer(menu);

	for(uint32_t i=0; i<MENU_MAX_OPTIONS; i++){
		if(menu->options[i] == NULL){
			menu->options[i] = option_create(label, action);
			menu->num_options = (uint8_t)(i+1);
			return;
		}
	}
	
	printf("ERROR: MENU: Could not add option '%s': Menu full.\n", label);
}

menu_t *menu_create_main_menu(game_t *game){
	menu_t *menu = menu_create(game);
	
	menu_add_option(menu, "Play Game", &menu_new_game);
	menu_add_option(menu, "Load Game", &menu_load_game);
	menu_add_option(menu, "Options", &menu_options);
	menu_add_option(menu, "Exit", &menu_exit);
	
	return menu;
}

void menu_exit(menu_t *menu){
	menu->game->core->running = false;
}

void menu_new_game(menu_t *menu){
	menu->game->mode = GAME_MODE_PLAY;
	camera_set_fade(menu->game->camera, 0x00000000);
}

void menu_load_game(menu_t *menu){
	game_set_message(menu->game, "'Load Game' not implemented yet.");
}

void menu_options(menu_t *menu){
	game_set_message(menu->game, "'Options' not implemented yet.");
}


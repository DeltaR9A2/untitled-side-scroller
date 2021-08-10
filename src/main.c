#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#undef main

#include "core.h"
#include "game.h"
#include "controller.h"

int main_event_watch(void *data, SDL_Event *e){
	game_t *game = (game_t *)data;
	
	if((e->type == SDL_QUIT)){
		game->core->running = false;
	}else if(e->type == SDL_KEYDOWN && e->key.keysym.scancode == SDL_SCANCODE_F11){
		core_toggle_fullscreen(game->core);
	}else if(e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_RESIZED){
		core_window_resize(game->core, e->window.data1, e->window.data2);
	}
	
	return 0;
}

int main(void){
	SDL_Init(SDL_INIT_EVERYTHING);
	
	core_t *core = core_create();
	game_t *game = game_create(core);

	SDL_AddEventWatch(&main_event_watch, game);
	
	double ms_per_frame = 10.0;
	double curr_ms = SDL_GetTicks();
	double prev_ms = curr_ms;
	double ms_delta = 0;
	double ms_accum = 0;
	
	while(core->running){
		prev_ms = curr_ms;
		curr_ms = SDL_GetTicks();
		ms_delta = curr_ms - prev_ms;
		ms_accum += ms_delta;
		
		if(ms_accum > ms_per_frame){
			ms_accum -= ms_per_frame;

			controller_poll_events(game->controller);
			
			if(ms_accum > ms_per_frame){
				game_fast_frame(game);
			}else{
				game_full_frame(game);
				core_window_redraw(core);
			}
		}
	}

	game_delete(game);
	core_delete(core);
	clear_image_cache();
	
	SDL_Quit();
	
	return 0;
}

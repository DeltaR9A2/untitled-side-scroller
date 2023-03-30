#include <stdlib.h>

#include "core.h"

// PC 3x   = 640x360  1.777  **
// PC 4x   = 480x270  1.777
// PC 5x   = 384x216  1.777  **
// PC 6x   = 320x180  1.777

// VGA MAX = 640x480  1.333  **  --
// VGA 13h = 320x200  1.600  **  --
// PAL     = 720x576  1.250  **
// NTSC    = 720x480  1.500  **  --

// SNES    = 256x224  1.143  **
// GBA     = 240x160  1.500  **

// SNES STRETCH = 1.166

//384/240

const int32_t VIRTUAL_SCREEN_W = 480;
const int32_t VIRTUAL_SCREEN_H = 256;

//static SDL_Texture *create_texture(SDL_Renderer *render, int32_t w, int32_t h){
//	return SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, w, h);
//}

static SDL_Texture *create_streaming_texture(SDL_Renderer *render, int32_t w, int32_t h){
	return SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
}

struct core_t{
	bool running;
	bool fullscreen;

	SDL_Window *window;
	int32_t win_vw, win_vh;
	int32_t win_cw, win_ch;
	SDL_Rect active_rect;

	SDL_Renderer *rend;
	SDL_Surface *screen;
	SDL_Texture *screen_texture;
};

static core_t *THE_CORE = NULL;

core_t *core_create(void){
	core_t *core = malloc(sizeof(core_t));
	
	core->running = true;
	core->fullscreen = false;

	core->win_vw = VIRTUAL_SCREEN_W;
	core->win_vh = VIRTUAL_SCREEN_H;
	core->win_cw = core->win_vw*2;
	core->win_ch = core->win_vh*2;
	
	core->active_rect.x = 0;
	core->active_rect.y = 0;
	core->active_rect.w = core->win_vw*2;
	core->active_rect.h = core->win_vh*2;

	core->window = SDL_CreateWindow(
		"game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		core->win_cw,
		core->win_ch,
		SDL_WINDOW_RESIZABLE
	);
	
	if(core->window == NULL){
		printf("Could not create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	core->rend = SDL_CreateRenderer(core->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	core->screen = create_surface(core->win_vw, core->win_vh);
	core->screen_texture = create_streaming_texture(core->rend, core->win_vw, core->win_vh);

    core_window_resize(core, core->win_vw*2, core->win_vh*2);

	return core;
}

void core_delete(core_t *core){
	SDL_DestroyTexture(core->screen_texture);
	SDL_FreeSurface(core->screen);
	SDL_DestroyRenderer(core->rend);
	SDL_DestroyWindow(core->window);
	free(core);
}

double core_get_window_scale(core_t *core){
	double h_scale = (int)((double)core->win_cw / (double)core->win_vw);
	double v_scale = (int)((double)core->win_ch / (double)core->win_vh);
	return (h_scale < v_scale) ? h_scale : v_scale;
}

void core_window_resize(core_t *core, int32_t w, int32_t h){
	core->win_cw = w;
	core->win_ch = h;
	
	double scale = core_get_window_scale(core);
	
	core->active_rect.w = (int)(scale * core->win_vw); // SNES STRETCH core->active_rect.w *= 1.1666;
	core->active_rect.h = (int)(scale * core->win_vh);
	core->active_rect.x = (core->win_cw - core->active_rect.w)/2;
	core->active_rect.y = (core->win_ch - core->active_rect.h)/2;
	
    
	//SDL_SetWindowSize(core->window, core->win_cw, core->win_ch);
}

void core_toggle_fullscreen(core_t *core){
	core->fullscreen = !(core->fullscreen);
	
	if(core->fullscreen){
		SDL_SetWindowFullscreen(core->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		core_window_resize(core, 0, 0);
	}else{
		SDL_SetWindowFullscreen(core->window, false);
		core_window_resize(core, 0, 0);
	}
}

void core_window_redraw(core_t *core){
	SDL_UpdateTexture(core->screen_texture, NULL, core->screen->pixels, core->screen->pitch);
	SDL_RenderClear(core->rend);
	SDL_RenderCopy(core->rend, core->screen_texture, NULL, &core->active_rect);
	SDL_RenderPresent(core->rend);
}

rect_t core_get_mouse_pos(core_t *core){
	double scale = core_get_window_scale(core);

	int mx, my;
	SDL_GetMouseState(&mx, &my);
	mx -= core->active_rect.x;
	my -= core->active_rect.y;

	mx = (int)((double)mx / scale);
	my = (int)((double)my / scale);
	
    return (rect_t){mx,my,0,0};
}


core_t *core_get_only(void){
    if(THE_CORE == NULL){
        THE_CORE = core_create();
    }
    return THE_CORE;
}

void core_cleanup(void){
    if(THE_CORE != NULL){
        core_delete(THE_CORE);
        THE_CORE = NULL;
    }
}

bool core_is_running(core_t *core){
    return core->running;
}

void core_stop_running(core_t *core){
    core->running = false;
}

SDL_Surface *core_get_screen_surface(core_t *core){
    return core->screen;
}

rect_t core_get_window_size(core_t *core){
    return (rect_t){0,0,core->win_cw,core->win_ch};
}

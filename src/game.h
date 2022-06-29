#ifndef game_h
#define game_h

#include <stdint.h>

typedef struct game_t game_t;

extern const uint32_t GAME_MODE_MENU;
extern const uint32_t GAME_MODE_PLAY;

extern const uint32_t GAME_MESSAGE_LEN;

#include "core.h"

#include "font.h"
#include "menu.h"
#include "camera.h"
#include "player.h"
#include "controller.h"

#include "map_dict.h"

#include "map.h"
#include "target.h"

struct game_t{
	core_t *core;
	uint32_t step;
	uint32_t mode;

	font_t *font;
	font_t *debug_font;

	controller_t *controller;
	
	menu_t *menu;
	
	camera_t *camera;	
	player_t *player;

	map_dict_t *maps;

	map_t *active_map;
	//target_t *active_target;
	
	//////////////////////////////
	char *message;
	uint32_t message_timeout;
	SDL_Surface *message_surface;
	//////////////////////////////
};

game_t *game_create(core_t *core);
void game_delete(game_t *game);

void game_fast_frame(game_t *game);
void game_full_frame(game_t *game);

void game_select_map(game_t *game, const char *map_name);

void game_set_message(game_t *game, const char *text);

#endif

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

	map_t *active_map;
    uint8_t map_world_x;
    uint8_t map_world_y;
	
	//////////////////////////////
	char *message;
	uint32_t message_timeout;
	SDL_Surface *message_surface;
	//////////////////////////////
};

game_t *game_get_only(void);
void game_cleanup(void);

void game_fast_frame(game_t *game);
void game_full_frame(game_t *game);

void game_set_message(game_t *game, const char *text);
void game_select_map(game_t *game, uint8_t x, uint8_t y);

#endif

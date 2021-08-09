#ifndef game_h
#define game_h

#include <stdint.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef struct game_t game_t;

extern const uint32_t GAME_MODE_MENU;
extern const uint32_t GAME_MODE_PLAY;
extern const uint32_t GAME_MODE_DIALOGUE;

extern const uint32_t GAME_MESSAGE_LEN;

#include "core.h"

#include "hud.h"
#include "font.h"
#include "menu.h"
#include "camera.h"
#include "player.h"
#include "controller.h"

#include "fset_dict.h"
#include "anim_dict.h"

#include "event_dict.h"
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
	
	hud_t *hud;
	menu_t *menu;
	
	camera_t *camera;	
	fset_dict_t *fsets;
	anim_dict_t *anims;

	player_t *player;

	event_dict_t *events;
	map_dict_t *maps;

	map_t *active_map;
	target_t *active_target;

	lua_State *LUA;

	//////////////////////////////
	char *message;
	uint32_t message_timeout;
	SDL_Surface *message_surface;
	//////////////////////////////

	//////////////////////////////
	char *dialogue_content;
	SDL_Surface *dialogue_portrait;
	SDL_Surface *dialogue_surface;
	//////////////////////////////

};

game_t *game_create(core_t *core);
void game_delete(game_t *game);

void game_fast_frame(game_t *game);
void game_full_frame(game_t *game);

void game_select_map(game_t *game, const char *map_name);

void game_set_message(game_t *game, const char *text);
void game_set_dialogue(game_t *game, const char *portrait, const char *message);

#endif

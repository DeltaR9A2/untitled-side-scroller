#ifndef lua_bindings_h
#define lua_bindings_h

#include <string.h>
#include <stdbool.h>

#include "minilua.h"

//#include <lua.h>
//#include <lualib.h>
//#include <lauxlib.h>

#include <SDL.h>

#include "game.h"
#include "cmap.h"
#include "sdl_helpers.h"

lua_State *lua_create();
void lua_delete();

void lua_set_game(game_t *game);

#endif

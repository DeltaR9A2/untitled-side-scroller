#include <stdlib.h>

#include "target.h"

const uint8_t TARGET_NONE = 0;
const uint8_t TARGET_INFO = 1;
const uint8_t TARGET_TEST = 2;

target_t *target_create(void){
	target_t *target = malloc(sizeof(target_t));

	target->type = TARGET_NONE;
	target->rect = rect_create();
	target->sprite = NULL;
	target->event = NULL;
	
	return target;
}

void target_delete(target_t *target){
	rect_delete(target->rect);
	free(target);
}

void target_activate(target_t *target, game_t *game){
	lua_rawgeti(game->LUA, LUA_REGISTRYINDEX, target->event->lua_ref);
	lua_pushstring(game->LUA, target->event->config);
	
	int retval = lua_pcall(game->LUA, 1, 0, 0);

	if(retval != 0){
		printf("WARNING: Event returned non-zero value.\n");
		printf("LUA Error: %s\n", lua_tostring(game->LUA, 1));
	}
}


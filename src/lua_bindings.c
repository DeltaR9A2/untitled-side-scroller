#define LUA_IMPL

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include "lua_bindings.h"
#pragma GCC diagnostic pop


static game_t *GAME = NULL;
static map_t *MAP = NULL;

static lua_State *LUA = NULL;

static void lua_bind_functions();

lua_State *lua_create(){
	if(LUA == NULL){
		LUA = luaL_newstate();
		luaL_openlibs(LUA);

		lua_bind_functions();
	}

	return LUA;
}

void lua_delete(){
	lua_close(LUA);
}

void lua_set_game(game_t *game){
	GAME = game;
}

static int lua_add_map(lua_State *L){
	const char *map_name = luaL_checkstring(L,1);
	const char *map_fn = luaL_checkstring(L,2);
	const char *map_image_fn = luaL_checkstring(L,3);
	
	map_t *map = map_dict_get(GAME->maps, map_name);
	
	map_init(map, map_fn, map_image_fn);
	
	MAP = map;
	
	return 0;
}

static int lua_edit_map(lua_State *L){
	const char *map_name = luaL_checkstring(L,1);

	map_t *map = map_dict_get(GAME->maps, map_name);

	MAP = map;
	
	return 0;
}

static int lua_add_fset(lua_State *L){
	const char *fset_name = luaL_checkstring(L,1);
	const char *file_name = luaL_checkstring(L,2);
	int cols = (int)luaL_checkinteger(L,3);
	int rows = (int)luaL_checkinteger(L,4);
	bool flip = lua_toboolean(L,5);

	#ifdef DEBUG
		printf("Adding FSet %s %s %i %i %i\n", fset_name, file_name, cols, rows, flip);
	#endif

	fset_t *fset = fset_dict_get(GAME->fsets, fset_name);
	fset_init(fset, file_name, cols, rows, flip);
	
	return 0;
}

static int lua_add_anim(lua_State *L){
	const char *fset_name = luaL_checkstring(L,1);
	const char *anim_name = luaL_checkstring(L,2);
	int start = (int)luaL_checkinteger(L,3);
	int length = atoi(luaL_checkstring(L,4));
	int rate = (int)luaL_checkinteger(L,5);
	
	#ifdef DEBUG
        printf("Adding Anim %s %s %i %i %i\n", fset_name, anim_name, start, length, rate);
    #endif

	fset_t *fset = fset_dict_get(GAME->fsets, fset_name);
	anim_t *anim = anim_dict_get(GAME->anims, anim_name);

	anim_init(anim, fset, start, length, rate);
	
	return 0;
}

static int lua_add_item(lua_State *L){
	int item_x = luaL_checkinteger(L,1);
	int item_y = luaL_checkinteger(L,2);
	const char *anim_name = luaL_checkstring(L,3);
	
	item_t *item = item_list_get_dead(MAP->items);
	
	item->body->rect->x = item_x;
	item->body->rect->y = item_y;
	item->body->rect->w = 8;
	item->body->rect->h = 8;
	
	sprite_set_anim(item->sprite, anim_dict_get(GAME->anims, anim_name));
	
	item->flags |= ITEM_ALIVE;
	item->flags |= ITEM_FALLS;
	
	item_update(item);
	
	return 0;
}

static int lua_add_event(lua_State *L){
	const char *event_name = luaL_checkstring(L,1);
	const char *func_name = luaL_checkstring(L,2);
	const char *config_string = luaL_checkstring(L,3);
	
	#ifdef DEBUG
		printf("Adding Event: %s %s\n", event_name, func_name);
	#endif

	event_t *event = event_dict_get(GAME->events, event_name);

	lua_getglobal(L, func_name);
	event->lua_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	sprintf(event->config, "%s", config_string);

	#ifdef DEBUG
		printf(">> Ref Num: %i\n", event->lua_ref);
	#endif

	return 0;
}

static int lua_add_target(lua_State *L){
	const char *target_name = luaL_checkstring(L, 1);
	int target_x = (int)luaL_checknumber(L,2);
	int target_y = (int)luaL_checknumber(L,3);
	const char *anim_name = luaL_checkstring(L,4);
	const char *event_name = luaL_checkstring(L,5);

	#ifdef DEBUG
		printf("Configuring Target: %s %i %i %s %s\n", target_name, target_x, target_y, anim_name, event_name);
	#endif

	target_t *target = target_dict_get(MAP->targets, target_name);
	target->rect->x = target_x;
	target->rect->y = target_y;
	target->rect->w = 8;
	target->rect->h = 8;

	target->sprite = sprite_create();
	sprite_set_anim(target->sprite, anim_dict_get(GAME->anims, anim_name));
	rect_set_l_edge(target->sprite->rect, rect_get_l_edge(target->rect));
	rect_set_t_edge(target->sprite->rect, rect_get_t_edge(target->rect));

	rect_match_to(target->rect, target->sprite->rect);

	target->event = event_dict_get(GAME->events, event_name);
	
	return 0;
}

static int lua_simple_dialogue(lua_State *L){
	const char *portrait = luaL_checkstring(L,1);
	const char *message = luaL_checkstring(L,2);

	game_set_dialogue(GAME, portrait, message);
	GAME->mode = GAME_MODE_DIALOGUE;

	return 0;
}

static int lua_simple_message(lua_State *L){
	const char *message = luaL_checkstring(L,1);
	game_set_message(GAME, message);
	
	return 0;
}

static int lua_move_player_to_map(lua_State *L){
	const char *map_name = luaL_checkstring(L, 1);
	game_select_map(GAME, map_name);
	
	return 0;
}

static int lua_move_player_to_target(lua_State *L){
	const char *target_name = luaL_checkstring(L, 1);
	
	#ifdef DEBUG
		printf("Moving player to target %s \n", target_name);
	#endif

	//GAME->active_map = MAP;
	target_t *target = target_dict_get(GAME->active_map->targets, target_name);

	#ifdef DEBUG
		printf(">>> Move to position %f, %f \n", rect_get_mid_x(target->rect), rect_get_mid_y(target->rect));
	#endif

	rect_move_to(GAME->player->body->rect, target->rect);

	return 0;
}

typedef struct lua_binding_t{
	int (*function)(lua_State *L);
	char *lua_name;
} lua_binding_t;

static lua_binding_t bindings[] = {
	{lua_add_fset, "add_fset"},
	{lua_add_anim, "add_anim"},

	{lua_add_map, "add_map"},
	{lua_add_item, "add_item"},
	{lua_add_event, "add_event"},
	{lua_add_target, "add_target"},

	{lua_edit_map, "edit_map"},
	
	{lua_simple_dialogue, "simple_dialogue"},
	{lua_simple_message,  "simple_message"},

	{lua_move_player_to_map, "move_player_to_map"},
	{lua_move_player_to_target, "move_player_to_target"},

	{NULL, NULL} // Last element marker.
};

static void lua_bind_functions(){
  #ifdef DEBUG
  printf("Binding lua functions...\n");
  #endif
  
	for(int i=0; bindings[i].function != NULL; i++){
	  #ifdef DEBUG
	  printf(">>> %s ... ", bindings[i].lua_name);
	  #endif
	  
		lua_pushcfunction(LUA, bindings[i].function);
		lua_setglobal(LUA, bindings[i].lua_name);
		
		#ifdef DEBUG
		printf("OK\n");
		#endif
	}
}


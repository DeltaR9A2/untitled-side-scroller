#include <stdlib.h>

#include "target.h"
    
#include "game.h"
#include "stb_ds.h"

const uint8_t ACTION_NONE = 0;
const uint8_t ACTION_MATH = 0;
const uint8_t ACTION_MESSAGE = 1;
const uint8_t ACTION_NOTIFICATION = 2;

struct action_t{
    uint8_t operation;
    char *arguments;
};

action_t *action_create(void){
    action_t *action = malloc(sizeof(action_t));
    action->operation = ACTION_NONE;
    action->arguments = NULL;
    return action;
}

void action_delete(action_t *action){
    free(action->arguments);
    free(action);
}

void action_execute(action_t *act){
    return;
}

struct target_t{
	rect_t *rect;
    action_t *action;
};

static struct { char *key; target_t *value; } *target_index = NULL;

target_t *target_create(void){
	target_t *target = malloc(sizeof(target_t));

	target->rect = rect_create();
    target->action = action_create();
	
	return target;
}

void target_delete(target_t *target){
    action_delete(target->action);
	rect_delete(target->rect);
	free(target);
}

target_t *get_target_by_name(const char *name){
    target_t *target = shget(target_index, name);
    
    if(target == NULL){
        target = target_create();
        shput(target_index, name, target);
    }
    
    return target;
}

void target_activate(target_t *target, game_t *game){
    return;
}

void load_targets(char *fn){
    return;
}

void unload_targets(void){
    return;
}

void activate_closest_target_at(rect_t *rect){
    return;
}
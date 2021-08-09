#include "event.h"

event_t *event_create(void){
	event_t *event = malloc(sizeof(event_t));
	event->lua_ref = 0;
	return event;
}

void event_delete(event_t *event){
	free(event);
}

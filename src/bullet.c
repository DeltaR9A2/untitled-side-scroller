#include "item.h"

const uint32_t ITEM_ALIVE = 0x00000001;
const uint32_t ITEM_FALLS = 0x00000002;
const uint32_t ITEM_SEEKS = 0x00000004;

item_t *item_create(void){
	item_t *item = malloc(sizeof(item_t));
	
	item->type = 0;
	item->body = body_create();
	item->sprite = sprite_create();
	item->flags = 0;
	
	return item;
}

void item_delete(item_t *item){
	sprite_delete(item->sprite);
	body_delete(item->body);
	
	free(item);
}

void item_update(item_t *item){
	if(item->flags & ITEM_FALLS){
		item->body->vy = fmin(10.0, item->body->vy + 0.35);
	}

	item->sprite->step += 1;
	rect_move_to(item->sprite->rect, item->body->rect);
}

item_t *item_list_get_dead(item_list_t *list){
    item_node_t *iter = list->head;
    
    while(iter != NULL){
    	if((iter->data->flags & ITEM_ALIVE) == 0){
    		return iter->data;
        }else{
            iter = iter->next;
        }
    }

    return item_list_get(list);
}


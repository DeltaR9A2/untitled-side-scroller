#include "bullet.h"

const uint32_t BULLET_ALIVE = 0x00000001;
const uint32_t BULLET_FALLS = 0x00000002;
const uint32_t BULLET_SEEKS = 0x00000004;

bullet_t *bullet_create(void){
	bullet_t *item = malloc(sizeof(bullet_t));
	
	item->type = 0;
	item->body = body_create();
	item->sprite = sprite_create();
	item->flags = 0;
	
	return item;
}

void bullet_delete(bullet_t *item){
	sprite_delete(item->sprite);
	body_delete(item->body);
	
	free(item);
}

void bullet_update(bullet_t *item){
	if(item->flags & BULLET_FALLS){
		item->body->vy = fmin(10.0, item->body->vy + 0.35);
	}

	item->sprite->step += 1;
	rect_move_to(item->sprite->rect, item->body->rect);
}

bullet_t *bullet_list_get_dead(bullet_list_t *list){
    bullet_node_t *iter = list->head;
    
    while(iter != NULL){
    	if((iter->data->flags & BULLET_ALIVE) == 0){
    		return iter->data;
        }else{
            iter = iter->next;
        }
    }

    return bullet_list_get(list);
}


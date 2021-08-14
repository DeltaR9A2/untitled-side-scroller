#include "bullet.h"

struct bullet_t{
	uint32_t type;
	body_t *body;
	sprite_t *sprite;
	uint32_t flags;
	
	uint8_t bounces;
	uint32_t lifespan;
};

const uint32_t BULLET_ALIVE = 0x00000001;
const uint32_t BULLET_FALLS = 0x00000002;
const uint32_t BULLET_SEEKS = 0x00000004;

bullet_t *bullet_create(void){
	bullet_t *bullet = malloc(sizeof(bullet_t));
	
	bullet->flags = 0;
	bullet->flags |= BULLET_ALIVE;
	bullet->lifespan = 100;

	bullet->body = body_create();
	body_set_size(bullet->body, 2, 2);

	bullet->sprite = sprite_create();
	
	return bullet;
}

void bullet_delete(bullet_t *bullet){
	sprite_delete(bullet->sprite);
	body_delete(bullet->body);
	
	free(bullet);
}

void bullet_init(bullet_t *bullet, uint32_t lifespan, anim_t *anim){
		bullet->flags |= BULLET_ALIVE;
		bullet->lifespan = lifespan;

		sprite_set_anim(bullet->sprite, anim);
}

void bullet_update(bullet_t *bullet){
	if(bullet->lifespan > 0){
		bullet->lifespan -= 1;
	}else{
		bullet->flags &= !BULLET_ALIVE;
	}
	
	if(bullet->flags & BULLET_FALLS){
		bullet->body->vy = fmin(10.0, bullet->body->vy + 0.35);
	}

	sprite_update(bullet->sprite);
	sprite_move_to(bullet->sprite, bullet->body->rect);
}

bool bullet_is_alive(bullet_t *bullet){
	return (bullet->flags & BULLET_ALIVE);
}

sprite_t *bullet_get_sprite(bullet_t *bullet){
	return bullet->sprite;
}

body_t *bullet_get_body(bullet_t *bullet){
	return bullet->body;
}

void bullet_move_to(bullet_t *bullet, rect_t *rect){
	body_move_to(bullet->body, rect);
	sprite_move_to(bullet->sprite, rect);
}

void bullet_set_velocity(bullet_t *bullet, double vx, double vy){
	body_set_velocity(bullet->body, vx, vy);
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


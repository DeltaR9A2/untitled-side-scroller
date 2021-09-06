#include "physics.h"

const uint32_t BLOCKED_L = 0x00000001;
const uint32_t BLOCKED_R = 0x00000002;
const uint32_t BLOCKED_U = 0x00000004;
const uint32_t BLOCKED_D = 0x00000008;
const uint32_t BLOCKED_MASK = 0x0000000F;

const uint32_t PLAT_DROP = 0x00000010;

body_t *body_create(void){
	body_t *body = malloc(sizeof(body_t));
	body->rect = rect_create();
	body->vx = 0;
	body->vy = 0;
	body->flags = 0;
	return body;
}

void body_delete(body_t *body){
	rect_delete(body->rect);
	free(body);
}

rect_t *body_get_rect(body_t *body){
	return body->rect;
}

void body_set_size(body_t *body, double w, double h){
	rect_set_size(body->rect, w, h);
}

void body_move_to(body_t *body, rect_t *rect){
	rect_move_to(body->rect, rect);
}

void body_set_velocity(body_t *body, double vx, double vy){
	body->vx = vx;
	body->vy = vy;
}

void do_physics_to_it(body_t *body, rect_list_t *terr_rects, rect_list_t *plat_rects){
	rect_t *start = rect_create();
	rect_match_to(start, body->rect);
	
	rect_node_t *iter;
	
	body->flags &= ~BLOCKED_MASK;
	
	body->rect->x += body->vx;
	iter = terr_rects->head;
	while(iter != NULL){
		if(rect_overlap(body->rect, iter->data)){
			if(body->vx > 0 && rect_get_r_edge(body->rect) >= rect_get_l_edge(iter->data)){
				body->vx = 0;
				rect_set_r_edge(body->rect, rect_get_l_edge(iter->data));
				body->flags |= BLOCKED_R;
			}else if(body->vx < 0 && rect_get_l_edge(body->rect) <= rect_get_r_edge(iter->data)){
				body->vx = 0;
				rect_set_l_edge(body->rect, rect_get_r_edge(iter->data));
				body->flags |= BLOCKED_L;
			}
			break;
		}
		iter = iter->next;
	}

	body->rect->y += body->vy;
	iter = terr_rects->head;
	while(iter != NULL){
		if(rect_overlap(body->rect, iter->data)){
			if(body->vy > 0 && rect_get_b_edge(body->rect) >= rect_get_t_edge(iter->data)){
				body->vy = 0;
				rect_set_b_edge(body->rect, rect_get_t_edge(iter->data));
				body->flags |= BLOCKED_D;
			}else if(body->vy < 0 && rect_get_t_edge(body->rect) <= rect_get_b_edge(iter->data)){
				body->vy = 0;
				rect_set_t_edge(body->rect, rect_get_b_edge(iter->data));
				body->flags |= BLOCKED_U;
			}
			break;
		}
		iter = iter->next;
	}
	
	if(body->vy > 0 && !(body->flags & PLAT_DROP)){
		iter = plat_rects->head;
		while(iter != NULL){
			if(rect_overlap(body->rect, iter->data)){
				if(rect_get_b_edge(start) <= rect_get_t_edge(iter->data)){
					body->vy = 0;
					rect_set_b_edge(body->rect, rect_get_t_edge(iter->data));
					body->flags |= BLOCKED_D;
					break;
				}

			}
			iter = iter->next;
		}
	}
	
	rect_delete(start);
}


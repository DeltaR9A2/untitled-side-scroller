#include "physics.h"

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


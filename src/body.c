#include <stdlib.h>

#include "body.h"

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
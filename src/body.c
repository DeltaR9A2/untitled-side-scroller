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


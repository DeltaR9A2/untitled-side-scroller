#ifndef bullet_h
#define bullet_h

typedef struct bullet_t bullet_t;

#include <stdint.h>

#include "body.h"
#include "sprite.h"

extern const uint32_t BULLET_ALIVE;
extern const uint32_t BULLET_FALLS;
extern const uint32_t BULLET_SEEKS;

struct bullet_t{
	uint32_t type;
	body_t *body;
	sprite_t *sprite;
	uint32_t flags;
	
	uint8_t bounces;
	uint32_t expiration_frame;
};

bullet_t *bullet_create(void);
void bullet_delete(bullet_t *item);

void bullet_update(bullet_t *item);

#include "bullet_list.h"

bullet_t *bullet_list_get_dead(bullet_list_t *list);

#endif

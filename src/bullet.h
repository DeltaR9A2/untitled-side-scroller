#ifndef bullet_h
#define bullet_h

typedef struct bullet_t bullet_t;

#include <stdint.h>

#include "body.h"
#include "sprite.h"

extern const uint32_t BULLET_ALIVE;
extern const uint32_t BULLET_FALLS;
extern const uint32_t BULLET_SEEKS;

bullet_t *bullet_create(void);
void bullet_delete(bullet_t *bullet);
void bullet_init(bullet_t *bullet, uint32_t lifespan, anim_t *anim);

void bullet_update(bullet_t *bullet);

bool bullet_is_alive(bullet_t *bullet);

body_t *bullet_get_body(bullet_t *bullet);
sprite_t *bullet_get_sprite(bullet_t *bullet);

void bullet_move_to(bullet_t *bullet, rect_t *rect);
void bullet_set_velocity(bullet_t *bullet, double vx, double vy);
#include "bullet_list.h"

bullet_t *bullet_list_get_dead(bullet_list_t *list);

#endif

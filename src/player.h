#ifndef player_h
#define player_h

#include <math.h>

typedef struct player_t player_t;

#include "game.h"
#include "sprite.h"
#include "physics.h"

extern const uint32_t DIR_X;
extern const uint32_t DIR_R;
extern const uint32_t DIR_L;

player_t *player_create(void);
void player_delete(player_t *player);

void player_update(player_t *player, game_t *game);
void player_update_controls(player_t *player, game_t *game);
void player_update_animation(player_t *player, game_t *game);

sprite_t *player_get_sprite(player_t *player);
rect_t *player_get_rect(player_t *player);
uint32_t player_get_facing(player_t *player);
void player_move_to_coord(player_t *player, double x, double y);

#endif

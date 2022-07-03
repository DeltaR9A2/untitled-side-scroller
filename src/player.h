#ifndef player_h
#define player_h

#include <math.h>

typedef struct player_t player_t;

#include "rect.h"
#include "game.h"
#include "sprite.h"

extern const uint32_t BLOCKED_L;
extern const uint32_t BLOCKED_R;
extern const uint32_t BLOCKED_U;
extern const uint32_t BLOCKED_D;
extern const uint32_t BLOCKED_MASK;

extern const uint32_t PLAT_DROP;

extern const uint32_t DIR_X;
extern const uint32_t DIR_R;
extern const uint32_t DIR_L;

player_t *player_get_only(void);
void player_cleanup(void);

void player_update(player_t *player, game_t *game);
void player_move_to_rect(player_t *player, rect_t *coord);
uint32_t player_get_facing(player_t *player);

sprite_t *player_get_sprite(player_t *player);
rect_t *player_get_rect(player_t *player);

#endif

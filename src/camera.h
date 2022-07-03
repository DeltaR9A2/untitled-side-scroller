#ifndef camera_h
#define camera_h

typedef struct camera_t camera_t;

#include "rect.h"
#include "game.h"
#include "player.h"


camera_t *camera_create(void);
void camera_init(camera_t *camera, int32_t w, int32_t h);
void camera_delete(camera_t *camera);

void camera_draw_game(camera_t *camera, game_t *game);

void camera_set_fade(camera_t *camera, int32_t color);

void camera_look_at(camera_t *camera, rect_t *rect);
void camera_limit_to(camera_t *camera, rect_t *rect);
SDL_Surface *camera_get_surface(camera_t *camera);

#endif

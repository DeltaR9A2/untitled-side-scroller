#ifndef core_h
#define core_h

#include <stdbool.h>

#include <SDL.h>

#include "sprite.h"

typedef struct core_t core_t;

core_t *core_get_only();
void core_cleanup();

void core_window_redraw(core_t *core);
void core_window_resize(core_t *core, int32_t w, int32_t h);
void core_toggle_fullscreen(core_t *core);

bool core_is_running(core_t *core);
void core_stop_running(core_t *core);

rect_t core_get_window_size(core_t *core);
double core_get_window_scale(core_t *core);
rect_t core_get_mouse_pos(core_t *core);

SDL_Surface *core_get_screen_surface(core_t *core);

#endif

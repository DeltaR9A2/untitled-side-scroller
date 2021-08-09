#ifndef font_h
#define font_h

#include <string.h>

#include <SDL.h>

#include "sdl_helpers.h"

#define MAX_GLYPHS 256
typedef struct font_t font_t;
struct font_t{
	SDL_Surface *glyphs[MAX_GLYPHS];
	uint32_t head_kerns[MAX_GLYPHS];
	uint32_t tail_kerns[MAX_GLYPHS];
};

font_t *font_create(const char *image_fn);
void font_delete(font_t *font);

void font_draw_all_glyphs(font_t *font, int x, int y, SDL_Surface *target);
void font_draw_string(font_t *font, const char *string, int32_t x, int32_t y, SDL_Surface *target);
void font_draw_string_part(font_t *font, const char *string, int32_t len, int32_t x, int32_t y, SDL_Surface *target);
void font_wrap_string(font_t *font, const char *string, int32_t x, int32_t y, int32_t w, SDL_Surface *target);

int32_t font_get_width(font_t *font, const char *string);
int32_t font_get_height(font_t *font);

#endif

#ifndef sdl_helpers_h
#define sdl_helpers_h

#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
//#include <SDL_image.h>
//#include <SDL2_rotozoom.h>

#include "image_dict.h"

SDL_Surface *create_surface(int32_t w, int32_t h);
SDL_Surface *load_image(const char *fn);
SDL_Surface *convert_surface(SDL_Surface *src);
SDL_Surface *flip_surface(SDL_Surface *src);

SDL_Texture *create_texture(SDL_Renderer *render, int32_t w, int32_t h);
SDL_Texture *create_streaming_texture(SDL_Renderer *render, int32_t w, int32_t h);

void clear_image_cache(void);

bool sdl_rect_overlap(SDL_Rect *a, SDL_Rect *b);

#endif

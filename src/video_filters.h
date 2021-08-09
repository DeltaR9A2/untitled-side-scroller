#include <stdlib.h>

#include <SDL.h>

void video_filter(SDL_Surface *surface, uint32_t (*function)(uint32_t, SDL_PixelFormat*));

uint32_t filter_negative(uint32_t pixel, SDL_PixelFormat *format);
uint32_t filter_grayscale(uint32_t pixel, SDL_PixelFormat *format);
uint32_t filter_sepia_tone(uint32_t pixel, SDL_PixelFormat *format);
uint32_t filter_hue_rotation(uint32_t pixel, SDL_PixelFormat *format);

uint32_t filter_hsl_grayscale(uint32_t pixel, SDL_PixelFormat *format);

extern uint32_t hue_rotation;
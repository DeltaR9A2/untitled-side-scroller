#ifndef image_h
#define image_h

typedef struct image_t image_t;

#include <SDL.h>

struct image_t{
	SDL_Surface *surface;
};

image_t *image_create(void);
void image_delete(image_t *image);

#endif

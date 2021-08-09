#ifndef fset_h
#define fset_h

#include <string.h>

#include "sdl_helpers.h"

typedef struct fset_t fset_t;
struct fset_t{
	int len;
	SDL_Surface **frames;
};

fset_t *fset_create(void);
void fset_init(fset_t *fset, const char *fn, int cols, int rows, bool flip);
void fset_delete(fset_t *fset);

#endif

#include "image.h"

image_t *image_create(void){
	image_t *image = malloc(sizeof(image_t));
	image->surface = NULL;
	return image;
}

void image_delete(image_t *image){
	SDL_FreeSurface(image->surface);
	free(image);
}

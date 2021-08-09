#include <stdlib.h>

#include "fset.h"

fset_t *fset_create(void){
	fset_t *fset = malloc(sizeof(fset_t));
	fset->len = 0;
	fset->frames = NULL;
	return fset;
}

void fset_init(fset_t *fset, const char *fn, int cols, int rows, bool flip){
  printf("fset_init %s %i %i %s \n", fn, cols, rows, flip ? "true" : "false");
	fset->len = cols * rows;
	fset->frames = malloc(sizeof(SDL_Surface*)*(fset->len));

	SDL_Surface *tmp = load_image(fn);
	SDL_Surface *frame = NULL;

	SDL_Rect frame_rect;
	frame_rect.x = 0;
	frame_rect.y = 0;
	frame_rect.w = tmp->w / cols;
	frame_rect.h = tmp->h / rows;
	
	for(int r=0; r<rows; r++){
		for(int c=0; c<cols; c++){ // lol c++ >_>
			frame = create_surface(frame_rect.w, frame_rect.h);
			frame_rect.x = c * frame_rect.w;
			frame_rect.y = r * frame_rect.h;
			SDL_BlitSurface(tmp, &frame_rect, frame, NULL);
			fset->frames[c+(r*cols)] = frame;
		}
	}
}

void fset_delete(fset_t *fset){
	if(fset->frames != NULL){
		for(int i=0; i<fset->len; i++){
			SDL_FreeSurface(fset->frames[i]);
		}
		free(fset->frames);
	}
	
	free(fset);
}


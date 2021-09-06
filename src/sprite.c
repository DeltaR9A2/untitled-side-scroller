#include <stdlib.h>

#include "sprite.h"

#include "stb_ds.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

static struct { char *key; SDL_Surface *value; } *image_cache = NULL;

SDL_Surface *create_surface(int32_t w, int32_t h){
	return SDL_CreateRGBSurface(0,w,h,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
}

SDL_Surface *convert_surface(SDL_Surface *src){
	return SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_RGBA8888, 0);
}

SDL_Surface *load_image(const char *fn){
	SDL_Surface *image = shget(image_cache, fn);

	if(image == NULL){
		int w, h, of;
		unsigned char *data = stbi_load(fn, &w, &h, &of, 4);
		SDL_Surface *tmp = SDL_CreateRGBSurfaceFrom((void*)data, w, h, 32, 4*w,0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
		image = convert_surface(tmp);
		SDL_FreeSurface(tmp);
		stbi_image_free(data);

		shput(image_cache, fn, image);
	}

	return image;
}

struct fset_t{
	int len;
	SDL_Surface **frames;
};

static struct { char *key; fset_t *value; } *fset_cache = NULL;

fset_t *fset_load(const char *fn, uint32_t cols, uint32_t rows){
	fset_t *fset = shget(fset_cache, fn);
	
	if(fset == NULL){
		fset = malloc(sizeof(fset_t));

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
			for(int c=0; c<cols; c++){
				frame = create_surface(frame_rect.w, frame_rect.h);
				frame_rect.x = c * frame_rect.w;
				frame_rect.y = r * frame_rect.h;
				SDL_BlitSurface(tmp, &frame_rect, frame, NULL);
				fset->frames[c+(r*cols)] = frame;
			}
		}
		
		shput(fset_cache, fn, fset);
	}
	
	return fset;
}


struct anim_t{
	int fps; 
	int len; 
	SDL_Surface **frames;
};

static struct { char *key; anim_t *value; } *anim_cache = NULL;

anim_t *anim_create(const char *name, const fset_t *fset, int start, int len, int fps){
	anim_t *anim = shget(anim_cache, name);
	
	if(anim != NULL){
		free(anim);
	}
	
	anim = malloc(sizeof(fset_t));

	anim->len = len;
	anim->fps = fps;

	anim->frames = malloc(sizeof(SDL_Surface*)*(anim->len));

	for(int i=0; i < anim->len; i++){
		anim->frames[i] = fset->frames[start + i];
	}
	
	shput(anim_cache, name, anim);
	
	return anim;
}

anim_t *anim_get(const char *name){
	return shget(anim_cache, name);
}

void anim_draw(anim_t *anim, int step, SDL_Surface *target, SDL_Rect *dest){
	// Magic number 0.01 is the inverse of 100
	// and 100 is the refresh rate of the main loop
	// so step*0.01 turns steps back into seconds.
	SDL_BlitSurface(anim->frames[((int)((step*0.01) * anim->fps)) % anim->len], NULL, target, dest);
}


struct sprite_t{
	rect_t *rect;
	anim_t *anim;
	uint32_t step;
};

sprite_t *sprite_create(){
	sprite_t *sprite = malloc(sizeof(sprite_t));
	sprite->rect = rect_create();
	sprite->anim = NULL;
	sprite->step = 0;
	return sprite;
}

void sprite_delete(sprite_t *sprite){
	rect_delete(sprite->rect);
	free(sprite);
}

void sprite_update(sprite_t *sprite){ sprite->step += 1; }

void sprite_move_to(sprite_t *sprite, rect_t *rect){ rect_move_to(sprite->rect, rect); }

void sprite_anim_set(sprite_t *sprite, anim_t *anim){
	if(sprite->anim != anim){
		sprite->anim = anim;
		sprite->step = 0;
		
		if(anim != NULL && anim->frames != NULL && anim->frames[0] != NULL){
			sprite->rect->w = anim->frames[0]->w;
			sprite->rect->h = anim->frames[0]->h;
		}
	}
}

void sprite_anim_set_by_name(sprite_t *sprite, const char *name){ sprite_anim_set(sprite, anim_get(name)); }

void sprite_draw(sprite_t *sprite, SDL_Surface *target, uint32_t x_offset, uint32_t y_offset){
	//if(sprite->anim != NULL){
		SDL_Rect draw_rect;

		rect_copy_to_sdl(sprite->rect, &draw_rect);
		draw_rect.x -= x_offset;
		draw_rect.y -= y_offset;
		anim_draw(sprite->anim, sprite->step, target, &draw_rect);
	//}
}

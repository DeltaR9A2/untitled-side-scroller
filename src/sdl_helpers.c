#include "sdl_helpers.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

image_dict_t *image_cache = NULL;
image_dict_t *flipped_cache = NULL;

SDL_Surface *create_surface(int32_t w, int32_t h){
	return SDL_CreateRGBSurface(0,w,h,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
}

SDL_Surface *convert_surface(SDL_Surface *src){
	return SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_RGBA8888, 0);
}

SDL_Surface *load_image(const char *fn){
	image_t *im;

	if(image_cache == NULL){
		image_cache = image_dict_create();
	}

	
	im = image_dict_get(image_cache, fn);
	
	if(im->surface == NULL){
		int w, h, of;
		unsigned char *data = stbi_load(fn, &w, &h, &of, 4);
		
		SDL_Surface *tmp, *final;

		//tmp = IMG_Load(fn);
		tmp = SDL_CreateRGBSurfaceFrom((void*)data, w, h, 32, 4*w,0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
		
		if(tmp == NULL){
			printf("LOAD IMAGE FAILED %s\n", fn);
			printf("This will probably crash the game...\n");	 
			return NULL;
		}
	
		final = convert_surface(tmp);
		SDL_FreeSurface(tmp);
		stbi_image_free(data);

		im->surface = final;
	}

	return im->surface;
}

SDL_Surface *stb_load_image(const char *fn){
	printf("Enter load_image...\n");
	printf("Loading %s\n", fn);
	image_t *im;
	SDL_Surface *tmp, *final;
	
	if(image_cache == NULL){
		image_cache = image_dict_create();
	}
	
	im = image_dict_get(image_cache, fn);
	
	if(im->surface == NULL){
		int w,h;
		int original_format;
		int depth = 32;
		int pitch = 4*w;
		unsigned char *data = stbi_load(fn, &w, &h, &original_format, STBI_rgb_alpha);
		if(data == NULL){
			return NULL;
		}
		tmp = SDL_CreateRGBSurfaceFrom((void*)data, w, h, depth, pitch, 0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
		if(tmp == NULL){
			printf("LOAD IMAGE FAILED %s\n", fn);
			printf("This will probably crash the game...\n");	 
			return NULL;
		}
	
		final = convert_surface(tmp);
		SDL_FreeSurface(tmp);
		stbi_image_free(data);

		im->surface = final;
	}

	return im->surface;
}

void clear_image_cache(void){
	if(image_cache != NULL){ image_dict_delete(image_cache); }
	if(flipped_cache != NULL){ image_dict_delete(flipped_cache); }
}

SDL_Texture *create_texture(SDL_Renderer *render, int32_t w, int32_t h){
	return SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, w, h);
}

SDL_Texture *create_streaming_texture(SDL_Renderer *render, int32_t w, int32_t h){
	return SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
}

bool sdl_rect_overlap(SDL_Rect *a, SDL_Rect *b){
		if((a->x + a->w) <= (b->x) || (a->x) >= (b->x + b->w) ||
		   (a->y + a->h) <= (b->y) || (a->y) >= (b->y + b->h)){
			return false;
		}else{
			return true;
		}
}


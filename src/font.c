#include <stdlib.h>
#include <SDL_image.h>

#include "font.h"

//const char *glyph_order = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

const char *glyph_order = " 1234567890-=`!@#$%^&*()_+~abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\;',./{}|:\"<>?";

//const char *glyph_order = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456780";

font_t *font_create(const char *image_fn){
	font_t *font = malloc(sizeof(font_t));
	memset(font, 0, sizeof(font_t));

	SDL_Surface *font_img = load_image(image_fn);
	
	uint32_t *pixels = font_img->pixels;

	int32_t this_mark = 0;
	int32_t prev_mark = 0;
	
	uint32_t mark_color = pixels[0];

	uint32_t glyph_index = 0;
	SDL_Rect glyph_rect;
	SDL_Surface *glyph_surface;

	uint32_t glyph_count = strlen(glyph_order);
	
	int32_t kern_mark = 0;
	uint32_t kern_color = pixels[1];
	uint32_t kern_counter = 0;

	while(glyph_index < glyph_count){
		uint8_t ascii_code = (int)glyph_order[glyph_index];
		
		// Seek to next glyph
		while(pixels[this_mark] == mark_color){
			if(this_mark > font_img->w){ break; }
			this_mark += 1;
		}
		
		// Measure Head Kern
		kern_mark = this_mark;
		kern_counter = 0;
		while(pixels[kern_mark] == kern_color){
			kern_mark += 1;
			kern_counter += 1;
		}
		font->head_kerns[ascii_code] = kern_counter;
		
		// Measure Glyph
		prev_mark = this_mark;
		while(pixels[this_mark] != mark_color){
			if(this_mark > font_img->w){ break; }
			this_mark += 1;
		}
		
		// Measure Tail Kern
		kern_mark = this_mark - 1;
		kern_counter = 0;
		while(pixels[kern_mark] == kern_color){
			kern_mark -= 1;
			kern_counter += 1;
		}
		font->tail_kerns[ascii_code] = kern_counter;
		
		// Check if we have run out of glyphs early.
		if(this_mark > font_img->w){
			fprintf(stderr, "Warning: font_init: Font source shorter than glyph list.\n");
			break;
		}
		
		// Record glyph to its own surface
		glyph_rect.x = prev_mark;
		glyph_rect.w = this_mark - prev_mark;
		glyph_rect.y = 1;
		glyph_rect.h = font_img->h-1;

		glyph_surface = create_surface(glyph_rect.w, glyph_rect.h);
		SDL_BlitSurface(font_img, &glyph_rect, glyph_surface, NULL);
		
		font->glyphs[(int)glyph_order[glyph_index]] = glyph_surface;
		
		glyph_index += 1;
	}
	
	return font;
}

void font_delete(font_t *font){
	for(int i=0; i<MAX_GLYPHS; i++){
		if(font->glyphs[i] != NULL){
			SDL_FreeSurface(font->glyphs[i]);
		}
	}
	
	free(font);
}

void font_draw_all_glyphs(font_t *font, int x, int y, SDL_Surface *target){
	font_draw_string(font, glyph_order, x, y, target);
}

void font_draw_string(font_t *font, const char *string, int32_t x, int32_t y, SDL_Surface *target){
	SDL_Rect target_rect;
	target_rect.x = x;
	target_rect.y = y;
	for(uint32_t i=0; i<strlen(string); i++){
		uint8_t ascii_code = (int)string[i];
		
		if(font->glyphs[ascii_code] != NULL){
			target_rect.x -= font->head_kerns[ascii_code];
			SDL_BlitSurface(font->glyphs[ascii_code], NULL, target, &target_rect);
			target_rect.x += font->glyphs[ascii_code]->w;
			target_rect.x -= font->tail_kerns[ascii_code];
			target_rect.x += 1;
		}
	}
}

void font_draw_string_part(font_t *font, const char *string, int32_t len, int32_t x, int32_t y, SDL_Surface *target){
	char temp[1024];

	strncpy(temp, string, len);
	temp[len] = '\0';

	font_draw_string(font, temp, x, y, target);
}

void font_wrap_string(font_t *font, const char *string, int32_t x, int32_t y, int32_t w, SDL_Surface *target){
	char temp[1024];
	int32_t h = font_get_height(font);
	int32_t line_start = 0;
	int32_t line_end = 0;

//  This line generates a warning and doesn't appear to do anything.
//  I don't remember why I put it here, so I'm commenting it out for now.
//	strncpy(temp, string+line_start, line_end-line_start);
	
	while(line_end < strlen(string)){
		line_end += 1;

		strncpy(temp, &(string[line_start]), line_end-line_start);
		temp[line_end-line_start] = '\0';

		if(font_get_width(font, temp) > w){
			int32_t temp_end = line_end-1;
			while(string[line_end] != ' '){
				line_end -= 1;
				if(line_end == 0){
					line_end = temp_end;
					break;
				}
			}
			
			strncpy(temp, &(string[line_start]), line_end-line_start);
			temp[line_end-line_start] = '\0';

			font_draw_string(font, temp, x, y, target);
			line_start = line_end + 1;
			line_end = line_start + 1;
			y += h;
		}
		
		if(line_end == strlen(string)){
			font_draw_string(font, &(string[line_start]), x, y, target);
		}
	}
}

int32_t font_get_width(font_t *font, const char *string){
	int32_t w = 0;
	for(uint32_t i=0; i<strlen(string); i++){
		uint8_t ascii_code = (int)string[i];
		if(font->glyphs[ascii_code] != NULL){
			w -= font->head_kerns[ascii_code];
			w += font->glyphs[ascii_code]->w;
			w -= font->tail_kerns[ascii_code];
			w += 1;
		}
	}
	return w;
}

int32_t font_get_height(font_t *font){
	return font->glyphs[(int)glyph_order[0]]->h;
}



#include "video_filters.h"

void video_filter(SDL_Surface *surface, uint32_t (*function)(uint32_t, SDL_PixelFormat*)){
	SDL_LockSurface(surface);

	uint32_t *pixels = (uint32_t*)surface->pixels;
	for(int i = 0; i < surface->w * surface->h; i++){
		pixels[i] = function(pixels[i], surface->format);
	}
	
	SDL_UnlockSurface(surface);
}

uint32_t filter_negative(uint32_t pixel, SDL_PixelFormat *format){
	uint8_t r, g, b, a;
	SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
	
	r = 255 - r;
	g = 255 - g;
	b = 255 - b;
	
	return SDL_MapRGBA(format, r, g, b, a);
}

uint32_t filter_grayscale(uint32_t pixel, SDL_PixelFormat *format){
	uint8_t r, g, b, a;
	SDL_GetRGBA(pixel, format, &r, &g, &b, &a);

	r = g = b = ((r+g+b)/3);
	
	return SDL_MapRGBA(format, r, g, b, a);
}

typedef struct{
	double r, g, b;
}RGB_Pixel;

typedef struct{
	double h, s, l;
}HSL_Pixel;

double vf_fmin(double a, double b){ return (a < b) ? a : b; }
double vf_fmax(double a, double b){ return (a > b) ? a : b; }

void rgb_to_hsl(RGB_Pixel *src, HSL_Pixel *dst){
	double min, max;
	min = vf_fmin(src->r, vf_fmin(src->g, src->b));
	max = vf_fmax(src->r, vf_fmax(src->g, src->b));
	
	dst->l = (min + max) / 2.0;
	
	if(max == min){
		dst->s = 0.0;
	}else if(dst->l < 0.5){
		dst->s = (max - min) / (max + min);
	}else if(dst->l >= 0.5){
		dst->s = (max - min) / (2.0 - max - min);
	}
	
	if(max == min){
		dst->h = 0.0;
	}else if(src->r == max){
		dst->h = (src->g-src->b)/(max-min);
	}else if(src->g == max){
		dst->h = 2.0 + (src->b-src->r)/(max-min);
	}else if(src->b == max){
		dst->h = 4.0 + (src->r-src->g)/(max-min);
	}
	
	dst->h *= 60.0;
	
	if(dst->h < 0.0){
		dst->h += 360.0;
	}
}

double hsl_to_rgb_sub(double tc, double t1, double t2){
	if((6.0 * tc) < 1.0){
		return t2 + (t1-t2) * 6.0 * tc;
	}else if((2.0 * tc) < 1.0){
		return t1;
	}else if((3.0 * tc) < 2.0){
		return t2 + (t1-t2) * (0.666-tc) * 6.0;
	}else{
		return t2;
	}
}

void hsl_to_rgb(HSL_Pixel *src, RGB_Pixel *dst){
	if(src->s < 0.001){
		dst->r = dst->g = dst->b = src->l;
	}else{
		double t1, t2;
		
		if(src->l < 0.5){
			t1 = src->l * (1.0 + src->s);
		}else{ // src->l >= 0.5
			t1 = (src->l+src->s) - (src->l*src->s);
		}
		
		t2 = (2 * src->l) - t1;
		
		src->h = src->h/360.0;
		
		double tr, tg, tb;
		tr = src->h + 0.333;
		tg = src->h;
		tb = src->h - 0.333;
		
		if(tr < 0.0){ tr += 1.0; }else if(tr > 1.0){ tr -= 1.0; }
		if(tg < 0.0){ tg += 1.0; }else if(tg > 1.0){ tg -= 1.0; }
		if(tb < 0.0){ tb += 1.0; }else if(tb > 1.0){ tb -= 1.0; }
		
		dst->r = hsl_to_rgb_sub(tr, t1, t2);
		dst->g = hsl_to_rgb_sub(tg, t1, t2);
		dst->b = hsl_to_rgb_sub(tb, t1, t2);
	}
}

void rgb_from_pixel(RGB_Pixel *rgb, uint32_t pixel, SDL_PixelFormat *format){
	uint8_t r, g, b, a;
	SDL_GetRGBA(pixel, format, &r, &g, &b, &a);

	rgb->r = (double)r / 255.0;
	rgb->g = (double)g / 255.0;
	rgb->b = (double)b / 255.0;
}

uint32_t pixel_from_rgb(RGB_Pixel *rgb, SDL_PixelFormat *format){
	uint8_t r, g, b;

	r = (uint8_t)(rgb->r * 255.0);
	g = (uint8_t)(rgb->g * 255.0);
	b = (uint8_t)(rgb->b * 255.0);
	
	return SDL_MapRGBA(format, r, g, b, 255);
}

uint32_t filter_hsl_grayscale(uint32_t pixel, SDL_PixelFormat *format){
	RGB_Pixel rgb;
	HSL_Pixel hsl;
	
	rgb_from_pixel(&rgb, pixel, format);
	
	rgb_to_hsl(&rgb, &hsl);
	
	hsl.s = vf_fmax(0.2, hsl.s * 0.25);
	hsl.h = 35;
	
	hsl_to_rgb(&hsl, &rgb);

	return pixel_from_rgb(&rgb, format);
}

uint32_t hue_rotation = 0;

uint32_t filter_hue_rotation(uint32_t pixel, SDL_PixelFormat *format){
	RGB_Pixel rgb;
	RGB_Pixel new;
	
	rgb_from_pixel(&rgb, pixel, format);

	new.r = rgb.r * ((double)(hue_rotation % 360) / 360.0);
	new.g = rgb.g * ((double)((hue_rotation + 120) % 360) / 360.0);
	new.b = rgb.b * ((double)((hue_rotation + 240) % 360) / 360.0);
	
	return pixel_from_rgb(&new, format);
}

uint32_t filter_sepia_tone(uint32_t pixel, SDL_PixelFormat *format){
	RGB_Pixel rgb;
	RGB_Pixel new;
	
	rgb_from_pixel(&rgb, pixel, format);

	double avg = ((rgb.r + rgb.g + rgb.b) / 3.0);

	avg *= 0.9;

	new.r = vf_fmin(1.00, (rgb.r * 0.393) + (rgb.g * 0.769) + (rgb.b * 0.189));
	new.g = vf_fmin(1.00, (rgb.r * 0.349) + (rgb.g * 0.686) + (rgb.b * 0.168));
	new.b = vf_fmin(1.00, (rgb.r * 0.272) + (rgb.g * 0.534) + (rgb.b * 0.131));
	
	return pixel_from_rgb(&new, format);
}
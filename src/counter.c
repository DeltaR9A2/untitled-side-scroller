#include "counter.h"

counter_t *counter_create(hud_t *hud){
	counter_t *counter = malloc(sizeof(counter_t));
	
	counter->hud = hud;
	counter->x = 0;
	counter->y = 0;
	sprintf(counter->label,"%s","New Counter");
	counter->count = 0;
	
	return counter;
}

void counter_delete(counter_t *counter){
	free(counter);
}

void counter_set_label(counter_t *counter, const char *string){
	sprintf(counter->label, "%s", string);
}

void counter_draw(counter_t *counter, SDL_Surface *target){
	char string_buffer[32];
	sprintf(string_buffer, "%s: %04i", counter->label, counter->count);

	uint32_t string_width = font_get_width(counter->hud->font, string_buffer);

	SDL_Rect draw_rect;
	draw_rect.w = string_width + 12;
	draw_rect.h = font_get_height(counter->hud->font) + 8;

	draw_rect.x = 640-draw_rect.w;
	draw_rect.y = counter->y;

	SDL_FillRect(target, &draw_rect, 0x666666FF);
	draw_rect.x += 2; draw_rect.y += 2; draw_rect.w -= 4; draw_rect.h -= 4;
	SDL_FillRect(target, &draw_rect, 0x333333FF);
	
	font_draw_string(counter->hud->font, string_buffer, draw_rect.x + 4, draw_rect.y + 3, target);
}


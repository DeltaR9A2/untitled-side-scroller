#ifndef cmap_h
#define cmap_h

#include <stdbool.h>

#include "rect.h"
#include "rect_list.h"

typedef struct cmap_t cmap_t;
struct cmap_t{
	rect_t *rect;
	int8_t *data;
};

cmap_t *cmap_create(void);
void cmap_init(cmap_t *cmap, int32_t x, int32_t y, int32_t w, int32_t h);
void cmap_delete(cmap_t *cmap);
void cmap_reset(cmap_t *cmap);

void cmap_copy_data_from(cmap_t *cmap, int8_t *data);

int32_t cmap_xy_to_i(cmap_t *cmap, int32_t x, int32_t y);
bool cmap_xy_is_valid(cmap_t *cmap, int32_t x, int32_t y);

void cmap_rect_mark(cmap_t *cmap, rect_t *rect);
bool cmap_rect_check(cmap_t *cmap, rect_t *rect);

void cmap_add_to_rect_list(cmap_t *cmap, rect_list_t *rects);

#endif

#ifndef rect_list_h
#define rect_list_h

typedef struct rect_node_t rect_node_t;
typedef struct rect_list_t rect_list_t;

#include "rect.h"

struct rect_node_t{
    rect_t *data;
    rect_node_t *next;
};

struct rect_list_t{
    rect_node_t *head;
};

rect_list_t *rect_list_create(void);
rect_t *rect_list_get(rect_list_t *list);
void rect_list_delete(rect_list_t *list);

int32_t rect_list_length(rect_list_t *list);

#endif

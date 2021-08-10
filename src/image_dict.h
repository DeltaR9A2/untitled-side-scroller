#ifndef image_dict_h
#define image_dict_h

typedef struct image_node_t image_node_t;
typedef struct image_dict_t image_dict_t;

#include <string.h>
#include "image.h"

struct image_node_t{
    image_t *data;
    image_node_t *next;
    char name[64];
};

struct image_dict_t{
    image_node_t *head;
};

image_dict_t *image_dict_create(void);
image_t *image_dict_get(image_dict_t *dict, const char *name);
void image_dict_delete(image_dict_t *dict);

#endif

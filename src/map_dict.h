#ifndef map_dict_h
#define map_dict_h

typedef struct map_node_t map_node_t;
typedef struct map_dict_t map_dict_t;

#include <string.h>
#include "map.h"

struct map_node_t{
    map_t *data;
    map_node_t *next;
    char name[64];
};

struct map_dict_t{
    map_node_t *head;
};

map_dict_t *map_dict_create(void);
map_t *map_dict_get(map_dict_t *dict, const char *name);
void map_dict_delete(map_dict_t *dict);

#endif

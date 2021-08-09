#ifndef TYPE_dict_h
#define TYPE_dict_h

typedef struct TYPE_node_t TYPE_node_t;
typedef struct TYPE_dict_t TYPE_dict_t;

#include <string.h>
#include "TYPE.h"

struct TYPE_node_t{
    TYPE_t *data;
    TYPE_node_t *next;
    char name[64];
};

struct TYPE_dict_t{
    TYPE_node_t *head;
};

TYPE_dict_t *TYPE_dict_create(void);
TYPE_t *TYPE_dict_get(TYPE_dict_t *dict, const char *name);
void TYPE_dict_delete(TYPE_dict_t *dict);

#endif

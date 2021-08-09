#ifndef TYPE_list_h
#define TYPE_list_h

typedef struct TYPE_node_t TYPE_node_t;
typedef struct TYPE_list_t TYPE_list_t;

#include "TYPE.h"

struct TYPE_node_t{
    TYPE_t *data;
    TYPE_node_t *next;
};

struct TYPE_list_t{
    TYPE_node_t *head;
};

TYPE_list_t *TYPE_list_create(void);
TYPE_t *TYPE_list_get(TYPE_list_t *list);
void TYPE_list_delete(TYPE_list_t *list);

int32_t TYPE_list_length(TYPE_list_t *list);

#endif

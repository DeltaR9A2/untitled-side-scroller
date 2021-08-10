#ifndef item_list_h
#define item_list_h

typedef struct item_node_t item_node_t;
typedef struct item_list_t item_list_t;

#include "item.h"

struct item_node_t{
    item_t *data;
    item_node_t *next;
};

struct item_list_t{
    item_node_t *head;
};

item_list_t *item_list_create(void);
item_t *item_list_get(item_list_t *list);
void item_list_delete(item_list_t *list);

int32_t item_list_length(item_list_t *list);

#endif

#ifndef bullet_list_h
#define bullet_list_h

typedef struct bullet_node_t bullet_node_t;
typedef struct bullet_list_t bullet_list_t;

#include "bullet.h"

struct bullet_node_t{
    bullet_t *data;
    bullet_node_t *next;
};

struct bullet_list_t{
    bullet_node_t *head;
};

bullet_list_t *bullet_list_create(void);
bullet_t *bullet_list_get(bullet_list_t *list);
void bullet_list_delete(bullet_list_t *list);

int32_t bullet_list_length(bullet_list_t *list);

#endif

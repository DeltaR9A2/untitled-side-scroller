#ifndef target_dict_h
#define target_dict_h

typedef struct target_node_t target_node_t;
typedef struct target_dict_t target_dict_t;

#include <string.h>
#include "target.h"

struct target_node_t{
    target_t *data;
    target_node_t *next;
    char name[64];
};

struct target_dict_t{
    target_node_t *head;
};

target_dict_t *target_dict_create(void);
target_t *target_dict_get(target_dict_t *dict, const char *name);
void target_dict_delete(target_dict_t *dict);

#endif

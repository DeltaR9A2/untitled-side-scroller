#ifndef event_dict_h
#define event_dict_h

typedef struct event_node_t event_node_t;
typedef struct event_dict_t event_dict_t;

#include <string.h>
#include "event.h"

struct event_node_t{
    event_t *data;
    event_node_t *next;
    char name[64];
};

struct event_dict_t{
    event_node_t *head;
};

event_dict_t *event_dict_create(void);
event_t *event_dict_get(event_dict_t *dict, const char *name);
void event_dict_delete(event_dict_t *dict);

#endif

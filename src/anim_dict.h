#ifndef anim_dict_h
#define anim_dict_h

typedef struct anim_node_t anim_node_t;
typedef struct anim_dict_t anim_dict_t;

#include <string.h>
#include "anim.h"

struct anim_node_t{
    anim_t *data;
    anim_node_t *next;
    char name[64];
};

struct anim_dict_t{
    anim_node_t *head;
};

anim_dict_t *anim_dict_create(void);
anim_t *anim_dict_get(anim_dict_t *dict, const char *name);
void anim_dict_delete(anim_dict_t *dict);

#endif

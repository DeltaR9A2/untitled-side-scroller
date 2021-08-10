#ifndef fset_dict_h
#define fset_dict_h

typedef struct fset_node_t fset_node_t;
typedef struct fset_dict_t fset_dict_t;

#include <string.h>
#include "fset.h"

struct fset_node_t{
    fset_t *data;
    fset_node_t *next;
    char name[64];
};

struct fset_dict_t{
    fset_node_t *head;
};

fset_dict_t *fset_dict_create(void);
fset_t *fset_dict_get(fset_dict_t *dict, const char *name);
void fset_dict_delete(fset_dict_t *dict);

#endif

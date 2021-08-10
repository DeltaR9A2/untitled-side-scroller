#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "map_dict.h"

map_node_t *_map_node_create(const char *name){
		#ifdef DEBUG
		printf("Creating new map_dict node for name %s.\n", name);
		#endif

    map_node_t *node = malloc(sizeof(map_node_t));
    node->data = map_create();
    node->next = NULL;
    strncpy(node->name, name, 63);
    return node;
}

void _map_node_delete(map_node_t *node){
    map_delete(node->data);
    free(node);
}

map_dict_t *map_dict_create(void){
    map_dict_t *temp = malloc(sizeof(map_dict_t));
    temp->head = NULL;
    return temp;
}

map_t *map_dict_get(map_dict_t *dict, const char *name){
    if(dict->head == NULL){
        dict->head = _map_node_create(name);
        return dict->head->data;
    }

    map_node_t *iter = dict->head;
    
    while(iter != NULL){
        if(strncmp(name, iter->name, 63) == 0){
            return iter->data;
        }else if(iter->next == NULL){
            iter->next = _map_node_create(name);
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }

    return NULL; // Should not reach this unless shenanigans are afoot.
}

void map_dict_delete(map_dict_t *dict){
    map_node_t *iter = dict->head;
    map_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _map_node_delete(iter);
        iter = next;
    }
    
    free(dict);
}

int32_t map_dict_length(map_dict_t *dict){
    map_node_t *iter = dict->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


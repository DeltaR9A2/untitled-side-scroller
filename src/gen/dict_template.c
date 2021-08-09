#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "TYPE_dict.h"

TYPE_node_t *_TYPE_node_create(const char *name){
		#ifdef DEBUG
		printf("Creating new TYPE_dict node for name %s.\n", name);
		#endif

    TYPE_node_t *node = malloc(sizeof(TYPE_node_t));
    node->data = TYPE_create();
    node->next = NULL;
    strncpy(node->name, name, 63);
    return node;
}

void _TYPE_node_delete(TYPE_node_t *node){
    TYPE_delete(node->data);
    free(node);
}

TYPE_dict_t *TYPE_dict_create(void){
    TYPE_dict_t *temp = malloc(sizeof(TYPE_dict_t));
    temp->head = NULL;
    return temp;
}

TYPE_t *TYPE_dict_get(TYPE_dict_t *dict, const char *name){
    if(dict->head == NULL){
        dict->head = _TYPE_node_create(name);
        return dict->head->data;
    }

    TYPE_node_t *iter = dict->head;
    
    while(iter != NULL){
        if(strncmp(name, iter->name, 63) == 0){
            return iter->data;
        }else if(iter->next == NULL){
            iter->next = _TYPE_node_create(name);
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }

    return NULL; // Should not reach this unless shenanigans are afoot.
}

void TYPE_dict_delete(TYPE_dict_t *dict){
    TYPE_node_t *iter = dict->head;
    TYPE_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _TYPE_node_delete(iter);
        iter = next;
    }
    
    free(dict);
}

int32_t TYPE_dict_length(TYPE_dict_t *dict){
    TYPE_node_t *iter = dict->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


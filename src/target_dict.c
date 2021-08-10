#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "target_dict.h"

target_node_t *_target_node_create(const char *name){
		#ifdef DEBUG
		printf("Creating new target_dict node for name %s.\n", name);
		#endif

    target_node_t *node = malloc(sizeof(target_node_t));
    node->data = target_create();
    node->next = NULL;
    strncpy(node->name, name, 63);
    return node;
}

void _target_node_delete(target_node_t *node){
    target_delete(node->data);
    free(node);
}

target_dict_t *target_dict_create(void){
    target_dict_t *temp = malloc(sizeof(target_dict_t));
    temp->head = NULL;
    return temp;
}

target_t *target_dict_get(target_dict_t *dict, const char *name){
    if(dict->head == NULL){
        dict->head = _target_node_create(name);
        return dict->head->data;
    }

    target_node_t *iter = dict->head;
    
    while(iter != NULL){
        if(strncmp(name, iter->name, 63) == 0){
            return iter->data;
        }else if(iter->next == NULL){
            iter->next = _target_node_create(name);
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }

    return NULL; // Should not reach this unless shenanigans are afoot.
}

void target_dict_delete(target_dict_t *dict){
    target_node_t *iter = dict->head;
    target_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _target_node_delete(iter);
        iter = next;
    }
    
    free(dict);
}

int32_t target_dict_length(target_dict_t *dict){
    target_node_t *iter = dict->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "event_dict.h"

event_node_t *_event_node_create(const char *name){
		#ifdef DEBUG
		printf("Creating new event_dict node for name %s.\n", name);
		#endif

    event_node_t *node = malloc(sizeof(event_node_t));
    node->data = event_create();
    node->next = NULL;
    strncpy(node->name, name, 63);
    return node;
}

void _event_node_delete(event_node_t *node){
    event_delete(node->data);
    free(node);
}

event_dict_t *event_dict_create(void){
    event_dict_t *temp = malloc(sizeof(event_dict_t));
    temp->head = NULL;
    return temp;
}

event_t *event_dict_get(event_dict_t *dict, const char *name){
    if(dict->head == NULL){
        dict->head = _event_node_create(name);
        return dict->head->data;
    }

    event_node_t *iter = dict->head;
    
    while(iter != NULL){
        if(strncmp(name, iter->name, 63) == 0){
            return iter->data;
        }else if(iter->next == NULL){
            iter->next = _event_node_create(name);
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }

    return NULL; // Should not reach this unless shenanigans are afoot.
}

void event_dict_delete(event_dict_t *dict){
    event_node_t *iter = dict->head;
    event_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _event_node_delete(iter);
        iter = next;
    }
    
    free(dict);
}

int32_t event_dict_length(event_dict_t *dict){
    event_node_t *iter = dict->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


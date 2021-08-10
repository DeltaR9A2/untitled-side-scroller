#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "anim_dict.h"

anim_node_t *_anim_node_create(const char *name){
		#ifdef DEBUG
		printf("Creating new anim_dict node for name %s.\n", name);
		#endif

    anim_node_t *node = malloc(sizeof(anim_node_t));
    node->data = anim_create();
    node->next = NULL;
    strncpy(node->name, name, 63);
    return node;
}

void _anim_node_delete(anim_node_t *node){
    anim_delete(node->data);
    free(node);
}

anim_dict_t *anim_dict_create(void){
    anim_dict_t *temp = malloc(sizeof(anim_dict_t));
    temp->head = NULL;
    return temp;
}

anim_t *anim_dict_get(anim_dict_t *dict, const char *name){
    if(dict->head == NULL){
        dict->head = _anim_node_create(name);
        return dict->head->data;
    }

    anim_node_t *iter = dict->head;
    
    while(iter != NULL){
        if(strncmp(name, iter->name, 63) == 0){
            return iter->data;
        }else if(iter->next == NULL){
            iter->next = _anim_node_create(name);
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }

    return NULL; // Should not reach this unless shenanigans are afoot.
}

void anim_dict_delete(anim_dict_t *dict){
    anim_node_t *iter = dict->head;
    anim_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _anim_node_delete(iter);
        iter = next;
    }
    
    free(dict);
}

int32_t anim_dict_length(anim_dict_t *dict){
    anim_node_t *iter = dict->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


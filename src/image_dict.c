#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "image_dict.h"

image_node_t *_image_node_create(const char *name){
		#ifdef DEBUG
		printf("Creating new image_dict node for name %s.\n", name);
		#endif

    image_node_t *node = malloc(sizeof(image_node_t));
    node->data = image_create();
    node->next = NULL;
    strncpy(node->name, name, 63);
    return node;
}

void _image_node_delete(image_node_t *node){
    image_delete(node->data);
    free(node);
}

image_dict_t *image_dict_create(void){
    image_dict_t *temp = malloc(sizeof(image_dict_t));
    temp->head = NULL;
    return temp;
}

image_t *image_dict_get(image_dict_t *dict, const char *name){
    if(dict->head == NULL){
        dict->head = _image_node_create(name);
        return dict->head->data;
    }

    image_node_t *iter = dict->head;
    
    while(iter != NULL){
        if(strncmp(name, iter->name, 63) == 0){
            return iter->data;
        }else if(iter->next == NULL){
            iter->next = _image_node_create(name);
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }

    return NULL; // Should not reach this unless shenanigans are afoot.
}

void image_dict_delete(image_dict_t *dict){
    image_node_t *iter = dict->head;
    image_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _image_node_delete(iter);
        iter = next;
    }
    
    free(dict);
}

int32_t image_dict_length(image_dict_t *dict){
    image_node_t *iter = dict->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


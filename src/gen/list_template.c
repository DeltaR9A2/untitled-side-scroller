#include <stdlib.h>

#include "TYPE_list.h"

TYPE_node_t *_TYPE_node_create(void){
    TYPE_node_t *node = malloc(sizeof(TYPE_node_t));
    node->data = TYPE_create();
    node->next = NULL;
    return node;
}

void _TYPE_node_delete(TYPE_node_t *node){
    TYPE_delete(node->data);
    free(node);
}

TYPE_list_t *TYPE_list_create(void){
    TYPE_list_t *temp = malloc(sizeof(TYPE_list_t));
    temp->head = NULL;
    return temp;
}

TYPE_t *TYPE_list_get(TYPE_list_t *list){
    if(list->head == NULL){
        list->head = _TYPE_node_create();
        return list->head->data;
    }
    
    TYPE_node_t *iter = list->head;
    
    while(iter != NULL){
        if(iter->next == NULL){
            iter->next = _TYPE_node_create();
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }
    
    return NULL; // Should not reach this unless shenanigans are afoot.
}

void TYPE_list_delete(TYPE_list_t *list){
    TYPE_node_t *iter = list->head;
    TYPE_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _TYPE_node_delete(iter);
        iter = next;
    }
    
    free(list);
}

int32_t TYPE_list_length(TYPE_list_t *list){
    TYPE_node_t *iter = list->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


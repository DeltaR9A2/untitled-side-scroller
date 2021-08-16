#include <stdlib.h>

#include "rect_list.h"

rect_node_t *_rect_node_create(void){
    rect_node_t *node = malloc(sizeof(rect_node_t));
    node->data = rect_create();
    node->next = NULL;
    return node;
}

void _rect_node_delete(rect_node_t *node){
    rect_delete(node->data);
    free(node);
}

rect_list_t *rect_list_create(void){
    rect_list_t *temp = malloc(sizeof(rect_list_t));
    temp->head = NULL;
    return temp;
}

rect_t *rect_list_get(rect_list_t *list){
    if(list->head == NULL){
        list->head = _rect_node_create();
        return list->head->data;
    }
    
    rect_node_t *iter = list->head;
    
    while(iter != NULL){
        if(iter->next == NULL){
            iter->next = _rect_node_create();
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }
    
    return NULL; // Should not reach this unless shenanigans are afoot.
}

void rect_list_delete(rect_list_t *list){
    rect_node_t *iter = list->head;
    rect_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _rect_node_delete(iter);
        iter = next;
    }
    
    free(list);
}

int32_t rect_list_length(rect_list_t *list){
    rect_node_t *iter = list->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


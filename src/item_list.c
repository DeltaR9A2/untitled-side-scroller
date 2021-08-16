#include <stdlib.h>

#include "item_list.h"

item_node_t *_item_node_create(void){
    item_node_t *node = malloc(sizeof(item_node_t));
    node->data = item_create();
    node->next = NULL;
    return node;
}

void _item_node_delete(item_node_t *node){
    item_delete(node->data);
    free(node);
}

item_list_t *item_list_create(void){
    item_list_t *temp = malloc(sizeof(item_list_t));
    temp->head = NULL;
    return temp;
}

item_t *item_list_get(item_list_t *list){
    if(list->head == NULL){
        list->head = _item_node_create();
        return list->head->data;
    }
    
    item_node_t *iter = list->head;
    
    while(iter != NULL){
        if(iter->next == NULL){
            iter->next = _item_node_create();
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }
    
    return NULL; // Should not reach this unless shenanigans are afoot.
}

void item_list_delete(item_list_t *list){
    item_node_t *iter = list->head;
    item_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _item_node_delete(iter);
        iter = next;
    }
    
    free(list);
}

int32_t item_list_length(item_list_t *list){
    item_node_t *iter = list->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


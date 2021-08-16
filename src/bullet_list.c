#include <stdlib.h>

#include "bullet_list.h"

bullet_node_t *_bullet_node_create(void){
    bullet_node_t *node = malloc(sizeof(bullet_node_t));
    node->data = bullet_create();
    node->next = NULL;
    return node;
}

void _bullet_node_delete(bullet_node_t *node){
    bullet_delete(node->data);
    free(node);
}

bullet_list_t *bullet_list_create(void){
    bullet_list_t *temp = malloc(sizeof(bullet_list_t));
    temp->head = NULL;
    return temp;
}

bullet_t *bullet_list_get(bullet_list_t *list){
    if(list->head == NULL){
        list->head = _bullet_node_create();
        return list->head->data;
    }
    
    bullet_node_t *iter = list->head;
    
    while(iter != NULL){
        if(iter->next == NULL){
            iter->next = _bullet_node_create();
            return iter->next->data;
        }else{
            iter = iter->next;
        }
    }
    
    return NULL; // Should not reach this unless shenanigans are afoot.
}

void bullet_list_delete(bullet_list_t *list){
    bullet_node_t *iter = list->head;
    bullet_node_t *next = NULL;
    
    while(iter != NULL){
        next = iter->next;
        _bullet_node_delete(iter);
        iter = next;
    }
    
    free(list);
}

int32_t bullet_list_length(bullet_list_t *list){
    bullet_node_t *iter = list->head;

    int32_t count = 0;
    while(iter != NULL){
        count += 1;
        iter = iter->next;
    }
    return count;
}


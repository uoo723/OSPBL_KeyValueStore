#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "msg.h"

node_t *init() {
    node_t *head = NULL;
    head = malloc(sizeof(node_t));
    if (head == NULL) {
        return NULL;
    }

    head->next = NULL;

    return head;
}

int insert(node_t *head, unsigned int key, char value[VALUESIZE]) {
    if (head == NULL) {
        return -1;
    }

    node_t *node = get(head, key);

    if (node != NULL) {
        return -1;
    }

    node = malloc(sizeof(node_t));

    if (node == NULL) {
        return -1;
    }

    node->key = key;
    strcpy(node->value, value);

    node_t *iter = head;

    while (iter->next != NULL) {
        iter = iter->next;
    }

    iter->next = node;

    return 0;
}

int delete(node_t *head, unsigned int key) {
    if (head == NULL) {
        return -1;
    }

    node_t *iter = head->next;

    while (iter != NULL) {
        if (iter->next != NULL && iter->next->key == key) {
            break;
        }

        iter = iter->next;
    }

    if (iter == NULL) {
        return -1;
    }

    node_t *temp = iter->next;

    iter->next = iter->next->next;

    free(temp);

    return 0;
}

node_t *get(node_t *head, unsigned int key) {
    if (head == NULL) {
        return NULL;
    }

    node_t *iter = head;
    while ((iter = iter->next) != NULL) {
        if (iter->key == key) {
            break;
        }
    }

    if (iter == NULL) {
        return NULL;
    }

    return iter;
}

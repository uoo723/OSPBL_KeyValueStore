#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "msg.h"

node_t *ll_create() {
    node_t *head = NULL;
    head = malloc(sizeof(node_t));
    if (head == NULL) {
        return NULL;
    }

    head->next = NULL;

    return head;
}

int ll_insert(node_t *head, unsigned int key, char *value) {
    if (head == NULL) {
        return -1;
    }

    node_t *node = ll_get(head, key);

    if (node != NULL) {
        return -1;
    }

    node = malloc(sizeof(node_t));

    if (node == NULL) {
        return -1;
    }

    node->key = key;
    strcpy(node->value, value);

    node->next = head->next;
    head->next = node;

    return 0;
}

int ll_remove(node_t *head, unsigned int key) {
    if (head == NULL) {
        return -1;
    }

    node_t *iter = head;

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

node_t *ll_get(node_t *head, unsigned int key) {
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

int ll_destroy(node_t *head) {
    if (head == NULL)
        return -1;

    node_t *iter = head;
    while (iter != NULL) {
        node_t *temp = iter;
        iter = iter->next;
        free(temp);
    }

    return 0;
}

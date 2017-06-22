#include <stdio.h>

#include "node.h"

void print(node_t *);

int main() {
    node_t *list = ll_create();

    if (list == NULL) {
        return 0;
    }

    ll_insert(list, 1, "hello");
    ll_insert(list, 2, "world");
    ll_insert(list, 3, "Han");
    ll_insert(list, 4, "Sangwoo");
    ll_insert(list, 5, "woo");
    ll_insert(list, 5, "aa");
    print(list);

    printf("ll_remove test\n");
    ll_remove(list, 6);
    ll_remove(list, 4);
    print(list);

    printf("ll_get test\n");
    node_t *temp = ll_get(list, 1);
    printf("key: %d, value: %s\n", temp->key, temp->value);
    temp = ll_get(list, 6);
    if (temp == NULL) {
        printf("ll_get is null\n");
    }

    ll_destroy(list);
    return 0;
}

void print(node_t *list) {
    node_t *iter = list;

    while ((iter = iter->next) != NULL) {
        printf("key: %d, value: %s\n", iter->key, iter->value);
    }
}

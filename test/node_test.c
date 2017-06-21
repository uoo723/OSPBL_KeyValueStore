#include <stdio.h>

#include "node.h"

void print(node_t *);

int main() {
    node_t *list = init();

    if (list == NULL) {
        return 0;
    }

    insert(list, 1, "hello");
    insert(list, 2, "world");
    insert(list, 3, "Han");
    insert(list, 4, "Sangwoo");
    insert(list, 5, "woo");
    insert(list, 5, "aa");
    print(list);

    printf("delete test\n");
    delete(list, 6);
    delete(list, 4);
    print(list);

    printf("get test\n");
    node_t *temp = get(list, 1);
    printf("key: %d, value: %s\n", temp->key, temp->value);
    temp = get(list, 6);
    if (temp == NULL) {
        printf("get is null\n");
    }
    return 0;
}

void print(node_t *list) {
    node_t *iter = list;

    while ((iter = iter->next) != NULL) {
        printf("key: %d, value: %s\n", iter->key, iter->value);
    }
}

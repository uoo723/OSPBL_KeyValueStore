#include <stdio.h>
#include <stdbool.h>
#include "hashtable.h"
#include "node.h"

void print_node(node_t *);

int main() {
    hashtable_t *hashtable;

    printf("Start hash test\n");
    if ((hashtable = ht_create(1000000)) == NULL) {
        printf("Failed allocating hashtable\n");
        return 0;
    }

    printf("ht_set test\n");
    ht_set(hashtable, 238, "Han");
    ht_set(hashtable, 2880, "Sang");
    ht_set(hashtable, 23023, "Woo");
    ht_set(hashtable, 23023, "W");

    print_node(ht_get(hashtable, 238));
    print_node(ht_get(hashtable, 2880));
    print_node(ht_get(hashtable, 23023));
    print_node(ht_get(hashtable, 3));

    printf("ht_remove test\n");
    ht_remove(hashtable, 3);
    if (ht_remove(hashtable, 2880) < 0) {
        printf("remove failed\n");
    }
    print_node(ht_get(hashtable, 2880));

    ht_destroy(hashtable);
    return 0;
}

void print_node(node_t *node) {
    if (node == NULL) {
        printf("node is null\n");
        return;
    }
    printf("key: %d, value: %s\n", node->key, node->value);
}

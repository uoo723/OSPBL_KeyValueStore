#include <stdlib.h>

#include "hashtable.h"
#include "node.h"

hashtable_t *ht_create(int size) {
    hashtable_t *hashtable = NULL;

    if (size < 1) {
        return NULL;
    }

    if ((hashtable = malloc(sizeof(hashtable_t))) == NULL) {
        return NULL;
    }

    if ((hashtable->table = calloc(size, sizeof(node_t *))) == NULL) {
        return NULL;
    }

    hashtable->size = size;

    return hashtable;
}

int ht_hash(hashtable_t *hashtable, unsigned int key) {
    return key % hashtable->size;
}

int ht_set(hashtable_t *hashtable, unsigned int key, char *value) {
    int index;
    node_t *head;

    if (hashtable == NULL) {
        return -1;
    }

    index = ht_hash(hashtable, key);
    head = hashtable->table[index];

    if (head == NULL) {
        if ((head = ll_create()) == NULL) {
            return -1;
        }

        hashtable->table[index] = head;
    }

    if (ll_insert(head, key, value) < 0) {
        return -1;
    }

    return 0;
}

node_t *ht_get(hashtable_t *hashtable, unsigned int key) {
    int index;
    node_t *head;
    node_t *node;

    if (hashtable == NULL) {
        return NULL;
    }

    index = ht_hash(hashtable, key);

    if ((head = hashtable->table[index]) == NULL) {
        return NULL;
    }

    if ((node = ll_get(head, key)) == NULL) {
        return NULL;
    }

    return node;
}

int ht_remove(hashtable_t *hashtable, unsigned int key) {
    int index;
    node_t *head;

    if (hashtable == NULL) {
        return -1;
    }

    index = ht_hash(hashtable, key);

    if ((head = hashtable->table[index]) == NULL) {
        return -1;
    }

    if (ll_remove(head, key) < 0) {
        return -1;
    }

    return 0;
}

int ht_destroy(hashtable_t *hashtable) {
    if (hashtable == NULL)
        return -1;

    int i;

    for (i = 0; i < hashtable->size; i++) {
        ll_destroy(hashtable->table[i]);
    }

    return 0;
}

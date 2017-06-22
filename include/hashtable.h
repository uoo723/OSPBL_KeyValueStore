#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "node.h"
#include "msg.h"

typedef struct hastable {
    int size;
    node_t **table;
} hashtable_t;

hashtable_t *ht_create(int);
int ht_hash(hashtable_t *, unsigned int);
int ht_set(hashtable_t *, unsigned int, char *);
int ht_remove(hashtable_t *, unsigned int);
node_t *ht_get(hashtable_t *, unsigned int);
int ht_destroy(hashtable_t *);

#endif

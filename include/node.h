#ifndef NODE_H
#define NODE_H

#include "msg.h"

typedef struct node {
    unsigned int key;
    char value[VALUESIZE];
    struct node *next;
} node_t;

node_t *ll_create();
int ll_insert(node_t *, unsigned int, char[VALUESIZE]);
int ll_delete(node_t *, unsigned int);
node_t *ll_get(node_t *, unsigned int);

#endif

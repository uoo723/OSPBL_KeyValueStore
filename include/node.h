#ifndef NODE_H
#define NODE_H

#include "msg.h"

typedef struct node {
    unsigned int key;
    char value[VALUESIZE];
    struct node *next;
} node_t;

node_t init();
int insert(unsigned int, char[VALUESIZE]);
int delete(unsigned int);
node_t get(unsigned int);

#endif

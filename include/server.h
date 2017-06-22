#ifndef SERVER_H
#define SERVER_H

void server();

void rcv_put(unsigned int, char *);
void rcv_get(unsigned int);
void rcv_remove(unsigned int);

#endif

#ifndef CLIENT_H
#define CLIENT_H

void client();

void req_put(unsigned int, char *);
void req_get(unsigned int);
void req_remove(unsigned int);
void req_quit();

#endif

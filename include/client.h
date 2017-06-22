#ifndef CLIENT_H
#define CLIENT_H

#ifndef CLIENT_TEST
    #define client() client_non_test()
#else
    #define client() client_test()
#endif

void req_put(unsigned int, char *);
void req_get(unsigned int);
void req_remove(unsigned int);
void req_quit();

#endif

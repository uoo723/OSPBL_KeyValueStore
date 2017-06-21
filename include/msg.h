#ifndef MSGTYPE_H
#define MSGTYPE_H

#define VALUESIZE 100
#define KEYID 2817

typedef struct msgbuf {
    long mtype;
    unsigned int key;
    char value[VALUESIZE];
} msgbuf_t;

#define MSGSIZE (sizeof(msgbuf_t) - sizeof(long))

#endif

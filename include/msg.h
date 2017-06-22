#ifndef MSGTYPE_H
#define MSGTYPE_H

#define VALUESIZE 100
#define KEYID 2817

#define TYPE_SERVER 1
#define TYPE_CLIENT 2

#define TYPE_REQ_PUT 1
#define TYPE_REQ_GET 2
#define TYPE_REQ_REMOVE 3
#define TYPE_RES_GET 4
#define TYPE_QUIT 5

#define NODATA -1

typedef struct msgbuf {
    long mtype;
    long type;
    unsigned int key;
    char value[VALUESIZE];
} msgbuf_t;

#define MSGSIZE (sizeof(msgbuf_t) - sizeof(long))

#endif

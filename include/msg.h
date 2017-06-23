#ifndef MSGTYPE_H
#define MSGTYPE_H

#define VALUESIZE 100
#define SERVER_KEYID ftok(".", 'S')
#define CLIENT_KEYID ftok(".", 'C')

#define TYPE_SERVER 1
#define TYPE_CLIENT 2

#define TYPE_REQ_PUT 10
#define TYPE_REQ_GET 20
#define TYPE_REQ_REMOVE 30
#define TYPE_RES_GET 40
#define TYPE_QUIT 50

#define NODATA ""

typedef struct msgbuf {
    /* message queue type */
    long mtype;
    /* custom type */
    long type;
    unsigned int key;
    char value[VALUESIZE];
} msgbuf_t;

#define MSGSIZE (sizeof(msgbuf_t) - sizeof(long))

#endif

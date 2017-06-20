#ifndef MSGTYPE_H
#define MSGTYPE_H

#define MTEXTSIZE 128
#define KEYID 2817

struct msgbuf {
    long mtype;
    char mtext[MTEXTSIZE];
};

#endif

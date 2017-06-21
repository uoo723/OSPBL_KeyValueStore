#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "msg.h"

void client() {
    key_t key_id;
    msgbuf_t msg;
    char *text;

    key_id = msgget(KEYID, IPC_CREAT|0666);

    if (key_id < 0) {
        perror("msgget error ");
        exit(0);
    }

    printf("Start client\n");

    while (1) {
        printf("input > ");
        scanf("%s", text);
        msg.mtype = 1;

        // strcpy(msg.mtext, text);

        if (msgsnd(key_id, &msg, MSGSIZE, 0) < 0) {
            perror("msgsnd error ");
            exit(0);
        }

        // printf("client: send %s\n", msg.key);

        sleep(1);
    }
}

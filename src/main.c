#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "server.h"
#include "client.h"

#include "msg.h"

void int_handler(int signo) {
    key_t key_id;

    key_id = msgget(KEYID, IPC_CREAT|0666);

    if (key_id < 0) {
        perror("msgget error ");
        exit(0);
    }

    msgctl(key_id, IPC_RMID, 0);
}

int main() {
    signal(SIGINT, int_handler);

    if (fork() == 0) {
        client();
    } else {
        server();
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "msg.h"
#include "client.h"

void menu();
void menu_get();
void menu_put();
void menu_remove();
void menu_quit();

void snd_msg();

key_t key_id;
msgbuf_t msg;

void client() {
    if ((key_id = msgget(KEYID, IPC_CREAT|0666)) < 0) {
        perror("msgget error ");
        exit(0);
    }

    printf("Welcome to Key Value Store\n");

    while (1) {
        menu();
    }
}

void menu() {
    printf("1. put, 2. get, 3. delete, 4. quit: ");
    int num;
    scanf("%d", &num);

    switch (num) {
        case 1:
        menu_put();
        break;

        case 2:
        menu_get();
        break;

        case 3:
        menu_remove();
        break;

        case 4:
        menu_quit();
        break;

        default:
        printf("invalid num\n");

    }
}

void menu_put() {
    unsigned int key;
    char value[VALUESIZE];

    printf("key > ");
    scanf("%d", &key);
    printf("value > ");
    scanf("%s", value);

    req_put(key, value);
}

void menu_get() {
    unsigned int key;

    printf("key > ");
    scanf("%d", &key);

    req_get(key);
}

void menu_remove() {
    unsigned int key;

    printf("key > ");
    scanf("%d", &key);

    req_remove(key);
}

void menu_quit() {
    printf("Bye\n");
    msg.mtype = TYPE_QUIT;
    snd_msg();
    exit(0);
}

void req_put(unsigned int key, char *value) {
    msg.mtype = TYPE_REQ_PUT;
    msg.key = key;
    strcpy(msg.value, value);
    snd_msg();
}

void req_get(unsigned int key) {
    msg.mtype = TYPE_REQ_GET;
    msg.key = key;
    snd_msg();

    if (msgrcv(key_id, &msg, MSGSIZE, TYPE_RES_GET, 0) < 0) {
        perror("msgrcv error ");
        return;
    }

    if (msg.key == NODATA) {
        printf("no data\n");
    } else {
        printf("value: %s\n", msg.value);
    }
}

void req_remove(unsigned int key) {
    msg.mtype = TYPE_REQ_REMOVE;
    msg.key = key;
    snd_msg();
}

void snd_msg() {
    if (msgsnd(key_id, &msg, MSGSIZE, 0) < 0) {
        perror("msgsnd error ");
        return;
    }
}

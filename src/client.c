#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "msg.h"
#include "client.h"
#include "client_view.h"

static void menu();
static void menu_get();
static void menu_put();
static void menu_remove();
static void menu_quit();

static void snd_msg();

static key_t key_id;
static msgbuf_t msg;

void client_non_test() {
    if ((key_id = msgget(KEYID, IPC_CREAT|0666)) < 0) {
        perror("msgget error ");
        exit(0);
    }

    while (1) {
        client_view();
    }
}

void req_put(unsigned int key, char *value) {
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_REQ_PUT;
    msg.key = key;
    strcpy(msg.value, value);
    snd_msg();
}

void req_get(unsigned int key) {
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_REQ_GET;
    msg.key = key;
    snd_msg();

    if (msgrcv(key_id, &msg, MSGSIZE, TYPE_CLIENT, 0) < 0) {
        perror("msgrcv error ");
        return;
    }

    show_get_result(msg.key, msg.value);
}

void req_remove(unsigned int key) {
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_REQ_REMOVE;
    msg.key = key;
    snd_msg();
}

void req_quit() {
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_QUIT;
    snd_msg();
    exit(0);
}

static void snd_msg() {
    if (msgsnd(key_id, &msg, MSGSIZE, 0) < 0) {
        perror("msgsnd error ");
        return;
    }
}

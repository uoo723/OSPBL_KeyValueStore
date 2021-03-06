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

static key_t server_key_id;
static key_t client_key_id;

void client() {
    if ((server_key_id = msgget(SERVER_KEYID, IPC_CREAT|0666)) < 0) {
        perror("msgget error in client ");
        exit(0);
    }

    if ((client_key_id = msgget(CLIENT_KEYID, IPC_CREAT|0666)) < 0) {
        perror("msgget error in client ");
        exit(0);
    }

    #ifndef CLIENT_TEST
    while (1) {
        client_view();
    }
    #endif
}

void req_put(unsigned int key, char *value) {
    msgbuf_t msg;
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_REQ_PUT;
    msg.key = key;
    strcpy(msg.value, value);
    snd_msg(&msg);
}

void req_get(unsigned int key) {
    msgbuf_t msg;
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_REQ_GET;
    msg.key = key;
    snd_msg(&msg);

    if (msgrcv(client_key_id, &msg, MSGSIZE, 0, 0) < 0) {
        perror("msgrcv error in client ");
        return;
    }

    show_get_result(msg.key, msg.value);
}

void req_remove(unsigned int key) {
    msgbuf_t msg;
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_REQ_REMOVE;
    msg.key = key;
    snd_msg(&msg);
}

void req_quit() {
    msgbuf_t msg;
    msg.mtype = TYPE_SERVER;
    msg.type = TYPE_QUIT;
    snd_msg(&msg);
    exit(0);
}

static void snd_msg(msgbuf_t *msg) {
    if (msgsnd(server_key_id, msg, MSGSIZE, 0) < 0) {
        perror("msgsnd error in client ");
        return;
    }
}

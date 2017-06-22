#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>

#include "msg.h"
#include "server.h"
#include "hashtable.h"
#include "node.h"

key_t key_id;
msgbuf_t msg;
hashtable_t *hashtable;

void int_handler(int signo) {
    msgctl(key_id, IPC_RMID, 0);
}

void server() {
	signal(SIGINT, int_handler);

	if ((key_id = msgget(KEYID, IPC_CREAT|0666)) < 0) {
		perror("msgget error ");
		exit(0);
	}

	if ((hashtable = ht_create(1000000)) == NULL) {
		perror("hashtable is null ");
		exit(0);
	}

	while (1) {
		if (msgrcv(key_id, &msg, MSGSIZE, TYPE_SERVER, 0) < 0) {
			perror("msgrcv error ");
			return;
		}

		switch (msg.type) {
			case TYPE_REQ_PUT:
			rcv_put(msg.key, msg.value);
			break;

			case TYPE_REQ_GET:
			rcv_get(msg.key);
			break;

			case TYPE_REQ_REMOVE:
			rcv_remove(msg.key);
			break;

			case TYPE_QUIT:
			int_handler(2);
			exit(0);
			break;

			default:
			printf("type is invalid\n");
		}
	}
}

void rcv_put(unsigned int key, char *value) {
	ht_set(hashtable, key, value);
}

void rcv_get(unsigned int key) {
	node_t *node = ht_get(hashtable, key);

	if (node == NULL) {
		msg.key = NODATA;
	} else {
		strcpy(msg.value, node->value);
	}

	msg.mtype = TYPE_CLIENT;
	msg.type = TYPE_RES_GET;

	if (msgsnd(key_id, &msg, MSGSIZE, 0) < 0) {
		perror("msgsnd error ");
		return;
	}

	msg.mtype = 0;
}

void rcv_remove(unsigned int key) {
	ht_remove(hashtable, key);
}

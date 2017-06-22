#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>

#include "msg.h"
#include "server.h"
#include "hashtable.h"
#include "node.h"
#include "thpool.h"

typedef threadpool threadpool_t;

typedef struct {
    unsigned int key;
    char value[VALUESIZE];
} args_t;

static key_t key_id;
static msgbuf_t msg;
static hashtable_t *hashtable;
static threadpool_t thpool;

static pthread_rwlock_t rwlock;

static void rcv_put_internal(void *);
static void rcv_get_internal(void *);
static void rcv_remove_internal(void *);

static void int_handler(int signo) {
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

    if ((thpool = thpool_init(4)) == NULL) {
        perror("thpool is null ");
        exit(0);
    }

    if (pthread_rwlock_init(&rwlock, NULL) < 0) {
        perror("failed initializing rwlock ");
        exit(0);
    }

    args_t *args = NULL;

	while (1) {
		if (msgrcv(key_id, &msg, MSGSIZE, TYPE_SERVER, 0) < 0) {
			perror("msgrcv error ");
			return;
		}

		switch (msg.type) {
			case TYPE_REQ_PUT:
            args = malloc(sizeof(args_t));
            args->key = msg.key;
            strcpy(args->value, msg.value);

            thpool_add_work(thpool, (void *) rcv_put_internal, (void *) args);
			break;

			case TYPE_REQ_GET:
            args = malloc(sizeof(args_t));
            args->key = msg.key;

            thpool_add_work(thpool, (void *) rcv_get_internal, (void *) args);
			break;

			case TYPE_REQ_REMOVE:
            args = malloc(sizeof(args_t));
            args->key = msg.key;

            thpool_add_work(thpool, (void *) rcv_remove_internal, (void *) args);
			break;

			case TYPE_QUIT:
            thpool_destroy(thpool);
            pthread_rwlock_destroy(&rwlock);
			int_handler(2);
			exit(0);
			break;

			default:
			printf("type is invalid\n");
		}
	}
}

static void rcv_put_internal(void *arguments) {
    args_t *args = (args_t *) arguments;

    pthread_rwlock_wrlock(&rwlock);
    rcv_put(args->key, args->value);
    pthread_rwlock_unlock(&rwlock);

    free(args);
}

static void rcv_get_internal(void *arguments) {
    args_t *args = (args_t *) arguments;

    pthread_rwlock_rdlock(&rwlock);
    rcv_get(args->key);
    pthread_rwlock_unlock(&rwlock);

    free(args);
}

static void rcv_remove_internal(void *arguments) {
    args_t *args = (args_t *) arguments;

    pthread_rwlock_wrlock(&rwlock);
    rcv_remove(args->key);
    pthread_rwlock_unlock(&rwlock);

    free(args);
}

void rcv_put(unsigned int key, char *value) {
	ht_set(hashtable, key, value);
}

void rcv_get(unsigned int key) {
	node_t *node = ht_get(hashtable, key);

	if (node == NULL) {
		strcpy(msg.value, NODATA);
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

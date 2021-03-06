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

#define THREAD 4

typedef threadpool threadpool_t;

typedef struct {
    unsigned int key;
    char value[VALUESIZE];
} args_t;

static key_t server_key_id;
static key_t client_key_id;

static hashtable_t *hashtable;
static threadpool_t thpool;

static pthread_rwlock_t rwlock;

static void rcv_put_internal(void *);
static void rcv_get_internal(void *);
static void rcv_remove_internal(void *);

static void int_handler(int signo) {
    msgctl(server_key_id, IPC_RMID, 0);
    msgctl(client_key_id, IPC_RMID, 0);
}

static int count;

void server() {
	signal(SIGINT, int_handler);

	if ((server_key_id = msgget(SERVER_KEYID, IPC_CREAT|0666)) < 0) {
		perror("msgget error in client ");
		exit(0);
	}

    if ((client_key_id = msgget(CLIENT_KEYID, IPC_CREAT|0666)) < 0) {
        perror("msgget error in client ");
		exit(0);
    }

	if ((hashtable = ht_create(1000000)) == NULL) {
		perror("hashtable is null ");
		exit(0);
	}

    if ((thpool = thpool_init(THREAD)) == NULL) {
        perror("thpool is null ");
        exit(0);
    }

    if (pthread_rwlock_init(&rwlock, NULL) < 0) {
        perror("failed initializing rwlock ");
        exit(0);
    }

    args_t *args = NULL;
    msgbuf_t msg;

	while (1) {
		if (msgrcv(server_key_id, &msg, MSGSIZE, 0, 0) < 0) {
			perror("msgrcv error in server ");
			return;
		}

        // printf("msgrcv %d\n", count++);
		switch (msg.type) {
			case TYPE_REQ_PUT:

            #ifndef SINGLE_THREAD
            args = malloc(sizeof(args_t));
            args->key = msg.key;
            strcpy(args->value, msg.value);

            thpool_add_work(thpool, (void *) rcv_put_internal, (void *) args);
            #else
            rcv_put(msg.key, msg.value);
            #endif

			break;

			case TYPE_REQ_GET:

            #ifndef SINGLE_THREAD
            args = malloc(sizeof(args_t));
            args->key = msg.key;

            thpool_add_work(thpool, (void *) rcv_get_internal, (void *) args);
            #else
            rcv_get(msg.key);
            #endif

			break;

			case TYPE_REQ_REMOVE:

            #ifndef SINGLE_THREAD
            args = malloc(sizeof(args_t));
            args->key = msg.key;

            thpool_add_work(thpool, (void *) rcv_remove_internal, (void *) args);
            #else
            rcv_remove(msg.key);
            #endif

			break;

			case TYPE_QUIT:
            thpool_destroy(thpool);
            pthread_rwlock_destroy(&rwlock);
            ht_destroy(hashtable);

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
    msgbuf_t msg;

	if (node == NULL) {
		strcpy(msg.value, NODATA);
	} else {
		strcpy(msg.value, node->value);
	}

	msg.mtype = TYPE_CLIENT;
	msg.type = TYPE_RES_GET;
    msg.key = key;
	if (msgsnd(client_key_id, &msg, MSGSIZE, 0) < 0) {
		perror("msgsnd error in server ");
		return;
	}
}

void rcv_remove(unsigned int key) {
	ht_remove(hashtable, key);
}

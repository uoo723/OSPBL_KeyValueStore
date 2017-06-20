#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "msg.h"

void server() {
	key_t key_id;
	struct msgbuf msg;

	key_id = msgget(KEYID, IPC_CREAT|0666);

	if (key_id < 0) {
		perror("msgget error ");
		exit(0);
	}

	printf("Start server\n");
	
	while (1) {
		if (msgrcv(key_id, &msg, sizeof(msg.mtext), 0, 0) < 0) {
			perror("msgrcv error ");
			exit(0);
		}
		printf("server: recevied %s\n", msg.mtext);
	}
}

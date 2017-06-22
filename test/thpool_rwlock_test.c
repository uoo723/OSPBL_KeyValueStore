#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "thpool.h"

typedef threadpool threadpool_t;

static pthread_rwlock_t rwlock;
static threadpool_t thpool;

void read_task() {
    pthread_rwlock_rdlock(&rwlock);
    int i;
    for (i = 0; i < 20; i++) {
        printf("read_task_%d in %u\n", i, (int) pthread_self());
    }
    pthread_rwlock_unlock(&rwlock);
}

void write_task() {
    pthread_rwlock_wrlock(&rwlock);
    int i;
    for (i = 0; i < 20; i++) {
        printf("write_task_%d in %u\n", i, (int) pthread_self());
    }
    pthread_rwlock_unlock(&rwlock);
}

int main() {
    thpool = thpool_init(4);
    pthread_rwlock_init(&rwlock, NULL);

    int i;

    for (i = 0; i < 4; i++) {
        if (i == 2) {
            thpool_add_work(thpool, (void *) write_task, NULL);
        }
        thpool_add_work(thpool, (void *) read_task, NULL);
    }

    sleep(1);
    thpool_destroy(thpool);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}

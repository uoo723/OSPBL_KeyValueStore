#define MAX_CONTENTS 100000
#define MAX_OPERATION 500000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "client.h"
#include "node.h"
#include "server.h"
#include "msg.h"
#include "thpool.h"

static void fill_contents();
static void client_test_put();
static void client_test_get();

static void start_timer();
static void stop_timer();

typedef threadpool threadpool_t;

typedef struct {
    unsigned int key;
    char value[VALUESIZE];

} args_t;

static unsigned int key_list[MAX_CONTENTS];
static threadpool_t thpool;

static time_t start_time;
static time_t end_time;

void client_test() {
    printf("client_test!!\n");

    if ((thpool = thpool_init(4)) == NULL) {
        perror("thpool is null ");
        exit(0);
    }

    printf("fill_contents()\n");
    start_timer();
    fill_contents();
    stop_timer();

    printf("operation: %d, put(): 5%%, get(): 95%%\n", MAX_OPERATION);
    start_timer();
    int i;
    for (i = 0; i < MAX_OPERATION; i++) {
        int p = (int) (rand() / (float) RAND_MAX * 100);

        // thpool_add_work(thpool, (void *) client_test_get, NULL);
        if (p > 5)
            thpool_add_work(thpool, (void *) client_test_get, NULL);
        else
            thpool_add_work(thpool, (void *) client_test_put, NULL);
    }
    thpool_wait(thpool);
    stop_timer();
    req_quit();
}

int main() {
    srand(time(NULL));
    if (fork() == 0) {
        client();
        client_test();
    } else {
        server();
    }

    return 0;
}

void show_get_result(unsigned int key, char *value) {
    // if (strcmp(value, NODATA) == 0) {
    //     printf("value: no data\n");
    // } else {
    //     printf("key: %u, value: %s in %u\n", key, value, (int) pthread_self());
    // }
}

static void fill_contents() {
    int i;
    for (i = 0; i < MAX_CONTENTS; i++) {
        unsigned int key = rand();
        char value[VALUESIZE];
        sprintf(value, "%u", key);
        req_put(key, value);
        key_list[i] = key;
    }
}

static void client_test_put() {
    // printf("client_test_put() in %u\n", (int) pthread_self());
    unsigned int key = rand();
    char value[VALUESIZE];
    sprintf(value, "%u", key);
    req_put(key, value);
}

static void client_test_get() {
    // printf("client_test_get() in %u\n", (int) pthread_self());
    int index = rand() % MAX_CONTENTS;
    req_get(key_list[index]);
}

static void start_timer() {
    start_time = time(NULL);
}

static void stop_timer() {
    end_time = time(NULL);
    printf("elapsed: %ld second(s)\n", end_time - start_time);
}

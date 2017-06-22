#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "client.h"
#include "client_view.h"
#include "msg.h"

static void menu();
static void menu_get();
static void menu_put();
static void menu_remove();
static void menu_quit();

static bool first_loop = true;

void client_view() {
    if (first_loop) {
        printf("Welcome to Key Value Store\n");
        first_loop = false;
    }

    menu();
}

void show_get_result(unsigned int key, char *value) {
    if (strcmp(value, NODATA) == 0) {
        printf("value: no data\n");
    } else {
        printf("value: %s\n", value);
    }
}

static void menu() {
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

static void menu_put() {
    unsigned int key;
    char value[VALUESIZE];

    printf("key > ");
    scanf("%d", &key);
    printf("value > ");
    scanf("%s", value);

    req_put(key, value);
}

static void menu_get() {
    unsigned int key;

    printf("key > ");
    scanf("%d", &key);

    req_get(key);
}

static void menu_remove() {
    unsigned int key;

    printf("key > ");
    scanf("%d", &key);

    req_remove(key);
}

static void menu_quit() {
    printf("Bye\n");
    req_quit();
}

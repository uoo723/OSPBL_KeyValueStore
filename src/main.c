#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "server.h"
#include "client.h"

#include "msg.h"

int main() {
    if (fork() == 0) {
        client();
    } else {
        server();
    }
    return 0;
}

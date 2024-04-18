#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t received = 0;

void handler(int sig) {
    received = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <catcher_pid> <mode>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t catcherPid = (pid_t) atoi(argv[1]);
    int mode = atoi(argv[2]);

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    union sigval value;
    value.sival_int = mode;
    sigqueue(catcherPid, SIGUSR1, value);

    while (!received) {
        pause();
    }

    return 0;
}

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sigusr1_handler(int sig) {
    printf("Otrzymano SIGUSR1\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s [none|ignore|handler|mask]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
    } else if (strcmp(argv[1], "handler") == 0) {
        signal(SIGUSR1, sigusr1_handler);
    } else if (strcmp(argv[1], "mask") == 0) {
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);

        raise(SIGUSR1);
        sigpending(&set);
        if (sigismember(&set, SIGUSR1)) {
            printf("SIGUSR1 jest oczekujący\n");
        } else {
            printf("SIGUSR1 nie jest oczekujący\n");
        }

        sigprocmask(SIG_UNBLOCK, &set, NULL);
        printf("Sygnał SIGUSR1 został odblokowany\n");

        return EXIT_SUCCESS;
    }

    raise(SIGUSR1);

    sleep(1);

    return EXIT_SUCCESS;
}

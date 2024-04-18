#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s liczba_procesów\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Proces potomny
            printf("PID rodzica: %d, PID potomka: %d\n", getppid(), getpid());
            exit(EXIT_SUCCESS);
        }
    }

    while(wait(NULL) > 0);
    printf("Liczba utworzonych procesów: %s\n", argv[1]);
    return EXIT_SUCCESS;
}

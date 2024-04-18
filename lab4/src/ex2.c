#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s ścieżka_katalogu\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Program główny\n");
    int local = 0;

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        printf("Proces potomny\n");
        global++;
        local++;
        printf("PID potomka: %d, PID rodzica: %d\n", getpid(), getppid());
        printf("local potomka: %d, global potomka: %d\n", local, global);

        int ret = execl("/bin/ls", "ls", argv[1], NULL);
        if (ret == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("Proces rodzica\n");
        printf("PID rodzica: %d, PID potomka: %d\n", getpid(), pid);
        if (WIFEXITED(status)) {
            printf("Kod zakończenia potomka: %d\n", WEXITSTATUS(status));
        }
        printf("local rodzica: %d, global rodzica: %d\n", local, global);
    }

    return EXIT_SUCCESS;
}

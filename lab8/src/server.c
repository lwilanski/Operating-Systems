#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PRINTERS 3
#define PRINT_JOB_SIZE 10
#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

struct print_job {
    char text[PRINT_JOB_SIZE];
    int in_use;
    int client_id;
};

void init_sem(int sem_id, int sem_val) {
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    argument.val = sem_val;
    if (semctl(sem_id, 0, SETVAL, argument) < 0) {
        perror("semctl");
        exit(1);
    }
}

void P(int sem_id) {
    struct sembuf operations[1] = {{0, -1, 0}};
    if (semop(sem_id, operations, 1) < 0) {
        perror("semop P");
        exit(1);
    }
}

void V(int sem_id) {
    struct sembuf operations[1] = {{0, 1, 0}};
    if (semop(sem_id, operations, 1) < 0) {
        perror("semop V");
        exit(1);
    }
}

void printer_process(struct print_job *jobs, int sem_id, int printer_id) {
    while (1) {
        P(sem_id);
        if (jobs[printer_id].in_use) {
            for (int j = 0; j < PRINT_JOB_SIZE; j++) {
                printf("%c", jobs[printer_id].text[j]);
                fflush(stdout);
                sleep(1);
            }
            printf("\n");
            jobs[printer_id].in_use = 0;
        }
        V(sem_id);
    }
}

int main() {
    int shm_id = shmget(SHM_KEY, sizeof(struct print_job) * MAX_PRINTERS, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    struct print_job *jobs = shmat(shm_id, NULL, 0);
    if (jobs == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    for (int i = 0; i < MAX_PRINTERS; i++) {
        jobs[i].in_use = 0;
    }

    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id < 0) {
        perror("semget");
        exit(1);
    }

    init_sem(sem_id, 1);

    for (int i = 0; i < MAX_PRINTERS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            printer_process(jobs, sem_id, i);
            exit(0);
        }
    }

    for (int i = 0; i < MAX_PRINTERS; i++) {
        wait(NULL);
    }

    return 0;
}

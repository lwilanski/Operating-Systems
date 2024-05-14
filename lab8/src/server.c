#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

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

    init_sem(sem_id, MAX_PRINTERS);

    while (1) {
        for (int i = 0; i < MAX_PRINTERS; i++) {
            if (jobs[i].in_use) {
                P(sem_id);
                printf("Client %d Printing: ", jobs[i].client_id);
                for (int j = 0; j < PRINT_JOB_SIZE; j++) {
                    printf("%c", jobs[i].text[j]);
                    fflush(stdout);
                    sleep(1);
                }
                printf("\n");
                jobs[i].in_use = 0;
                V(sem_id);
            }
        }
    }

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>

#define MAX_TEXT_SIZE 10
#define PRINT_JOB_SIZE 10
#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define MAX_PRINTERS 3

struct print_job {
    char text[MAX_TEXT_SIZE];
    int in_use;
    int client_id;
};

int main() {

    int client_id = getpid();
    printf("Client PID: %d\n", client_id);

    int shm_id = shmget(SHM_KEY, sizeof(struct print_job) * MAX_PRINTERS, 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    struct print_job *jobs = shmat(shm_id, NULL, 0);
    if (jobs == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    int sem_id = semget(SEM_KEY, 1, 0666);
    if (sem_id < 0) {
        perror("semget");
        exit(1);
    }

    srand(time(NULL));
    while (1) {
    	struct print_job new_job;
    	for (int i = 0; i < MAX_TEXT_SIZE; i++) {
        	new_job.text[i] = 'a' + rand() % 26;
    	}
    	new_job.client_id = client_id;
    	new_job.in_use = 1;

    	int job_assigned = 0;
    	for (int i = 0; i < MAX_PRINTERS; i++) {
        	if (!jobs[i].in_use) {
            		memcpy(&jobs[i], &new_job, sizeof(struct print_job));
            		printf("Client %d sent print job: '%s'\n", client_id, new_job.text);
            		job_assigned = 1;
            		break;
        	}
    	}

    	if (!job_assigned) {
        	printf("Client %d: No available printers. Retrying...\n", client_id);
    	}

    	sleep(rand() % 10 + 1);
	}

    return 0;
}
	

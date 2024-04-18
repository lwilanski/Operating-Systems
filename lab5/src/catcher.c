#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile int mode = 1;
volatile int modeChangeRequests = 0;
volatile sig_atomic_t canSend = 0;
pid_t senderPid = 0;

void handler(int sig, siginfo_t *info, void *ucontext) {
    (void)ucontext;
    senderPid = info->si_pid;
    mode = info->si_value.sival_int;
    canSend = 1;
    modeChangeRequests++;
}

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    printf("Catcher PID: %d\n", getpid());

    while (mode != 3) {
        if (canSend) {
            if (mode == 1) {
                for (int i = 1; i <= 100; ++i) {
                    printf("%d\n", i);
                }
            } else if (mode == 2) {
                printf("Mode change requests: %d\n", modeChangeRequests);
            }
            kill(senderPid, SIGUSR1);
            canSend = 0;
        } else {
            pause();
        }
    }

    if (mode == 3 && senderPid != 0) {
	printf("Mode 3 recevied, exiting.\n");
        kill(senderPid, SIGUSR1);
    }

    return 0;
}


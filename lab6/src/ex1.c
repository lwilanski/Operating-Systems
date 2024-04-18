#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

double calculatePart(double start, double p_width, double width) {
    int intervals = (int)round(p_width / width);
    double sum = 0.0;
    for (int i = 0; i < intervals; ++i) {
        double x = start + (i + 0.5) * width;
        sum += 4.0 / (x * x + 1);
    }
    return sum * width;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <width> <n_processes>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double width = atof(argv[1]);
    int n_processes = atoi(argv[2]);
    double process_width = 1 / (double)n_processes;

    int pipes[n_processes][2];
    pid_t pids[n_processes];

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < n_processes; ++i) {
        pipe(pipes[i]);
        if ((pids[i] = fork()) == 0) {
            close(pipes[i][0]);
            double partSum = calculatePart(i * process_width, process_width, width);
            write(pipes[i][1], &partSum, sizeof(partSum));
            close(pipes[i][1]);
            exit(EXIT_SUCCESS);
        }
        close(pipes[i][1]);
    }

    double totalSum = 0.0;
    for (int i = 0; i < n_processes; ++i) {
        double partSum;
        read(pipes[i][0], &partSum, sizeof(partSum));
        totalSum += partSum;
        close(pipes[i][0]);
    }

    for (int i = 0; i < n_processes; ++i) {
        waitpid(pids[i], NULL, 0);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);

    printf("Całkowita wartość całki: %.12f\n", totalSum);
    printf("Czas wykonania: %f sekund\n", elapsed);

    FILE *report = fopen("report.txt", "a");
    if (report == NULL) {
        perror("Nie można otworzyć pliku report.txt");
        return EXIT_FAILURE;
    }
    fprintf(report, "Szerokość: %.12f, Procesy: %d, Całka: %.12f, Czas: %f sekund\n", width, n_processes, totalSum, elapsed);
    fclose(report);

    return EXIT_SUCCESS;
}


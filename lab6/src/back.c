#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

double calculateIntegral(double lower, double upper) {
    double sum = 0.0;
    double dx = (upper - lower) / 1000;
    for (int i = 0; i < 1000; i++) {
        double x = lower + dx * (i + 0.5);
        sum += 4.0 / (x * x + 1) * dx;
    }
    return sum;
}

int main() {
    double lower, upper;
    int fd;
    char* myfifo = "/tmp/myfifo";

    fd = open(myfifo, O_RDONLY);
    read(fd, &lower, sizeof(lower));
    read(fd, &upper, sizeof(upper));
    close(fd);

    double result = calculateIntegral(lower, upper);

    fd = open(myfifo, O_WRONLY);
    write(fd, &result, sizeof(result));
    close(fd);

    return 0;
}


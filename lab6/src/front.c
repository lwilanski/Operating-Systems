#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    double lower, upper;
    printf("Podaj dolny i górny przedział całkowania: ");
    scanf("%lf %lf", &lower, &upper);

    int fd;
    char* myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);
    write(fd, &lower, sizeof(lower));
    write(fd, &upper, sizeof(upper));
    close(fd);

    fd = open(myfifo, O_RDONLY);
    double result;
    read(fd, &result, sizeof(result));
    printf("Wartość całki: %lf\n", result);
    close(fd);

    unlink(myfifo);

    return 0;
}


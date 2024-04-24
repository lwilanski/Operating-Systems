#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    DIR *dir;
    struct dirent *entry;
    struct stat buf;
    long long total_size = 0;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &buf) == -1) {
            perror("stat");
            continue;
        }

        if (!S_ISDIR(buf.st_mode)) {
            printf("Rozmiar: %lld bajtów, Nazwa: %s\n", (long long)buf.st_size, entry->d_name);
            total_size += buf.st_size;
        }
    }

    closedir(dir);
    printf("Sumaryczny rozmiar plików: %lld bajtów\n", total_size);

    return EXIT_SUCCESS;
}


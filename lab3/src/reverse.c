#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void reverse_file(const char *input_path, const char *output_path) {
    int input_fd = open(input_path, O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    int output_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        exit(EXIT_FAILURE);
    }

    off_t file_size = lseek(input_fd, 0, SEEK_END);
    char buffer;

    for (off_t i = 1; i <= file_size; i++) {
        lseek(input_fd, -i, SEEK_END);
        if (read(input_fd, &buffer, 1) != 1) {
            perror("Error reading file");
            close(input_fd);
            close(output_fd);
            exit(EXIT_FAILURE);
        }
        if (write(output_fd, &buffer, 1) != 1) {
            perror("Error writing file");
            close(input_fd);
            close(output_fd);
            exit(EXIT_FAILURE);
        }
    }

    close(input_fd);
    close(output_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    reverse_file(argv[1], argv[2]);

    return EXIT_SUCCESS;
}


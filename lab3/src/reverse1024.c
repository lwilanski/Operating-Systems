#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 1024

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
    char buffer[BLOCK_SIZE];
    ssize_t bytes_read;

    off_t remaining = file_size % BLOCK_SIZE;
    off_t to_read = remaining > 0 ? remaining : BLOCK_SIZE;

    for (off_t i = file_size; i > 0; i -= to_read) {
        lseek(input_fd, -i, SEEK_END);
        bytes_read = read(input_fd, buffer, to_read);
        if (bytes_read == -1) {
            perror("Error reading file");
            close(input_fd);
            close(output_fd);
            exit(EXIT_FAILURE);
        }

        char reversed_buffer[BLOCK_SIZE];
        for (ssize_t j = 0; j < bytes_read; j++) {
            reversed_buffer[j] = buffer[bytes_read - 1 - j];
        }
        if (write(output_fd, reversed_buffer, bytes_read) != bytes_read) {
            perror("Error writing file");
            close(input_fd);
            close(output_fd);
            exit(EXIT_FAILURE);
        }

        to_read = BLOCK_SIZE;
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define NAME_LEN 32

int sockfd;
char name[NAME_LEN];

void handle_exit() {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "STOP");
    send(sockfd, buffer, strlen(buffer), 0);
    close(sockfd);
    printf("\nDisconnected from server.\n");
    exit(0);
}

void handle_signal(int sig) {
    handle_exit();
}

void *receive_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    int n;
    while ((n = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("%s\n", buffer);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <NAME> <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(name, argv[1], NAME_LEN);
    char *ip = argv[2];
    int port = atoi(argv[3]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    send(sockfd, name, strlen(name), 0);

    signal(SIGINT, handle_signal);

    pthread_t tid;
    if (pthread_create(&tid, NULL, receive_handler, NULL) != 0) {
        perror("pthread_create");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    pthread_detach(tid);

    char buffer[BUFFER_SIZE];
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        send(sockfd, buffer, strlen(buffer), 0);
    }

    return 0;
}


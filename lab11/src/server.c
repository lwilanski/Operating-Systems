#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define NAME_LEN 32

typedef struct {
    int socket;
    char name[NAME_LEN];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_to_all(char *message, int exclude_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket != 0 && clients[i].socket != exclude_sock) {
            if (send(clients[i].socket, message, strlen(message), 0) < 0) {
                perror("send");
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_one(char *message, char *recipient) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket != 0 && strcmp(clients[i].name, recipient) == 0) {
            if (send(clients[i].socket, message, strlen(message), 0) < 0) {
                perror("send");
            }
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void list_clients(int sock) {
    pthread_mutex_lock(&clients_mutex);

    char list[BUFFER_SIZE] = "Active clients:\n";
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket != 0) {
            strcat(list, clients[i].name);
            strcat(list, "\n");
        }
    }
    send(sock, list, strlen(list), 0);

    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket == sock) {
            clients[i].socket = 0;
            bzero(clients[i].name, NAME_LEN);
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void handle_client(int sock) {
    char buffer[BUFFER_SIZE];
    char name[NAME_LEN];
    int n;

    if ((n = recv(sock, name, NAME_LEN, 0)) <= 0) {
        close(sock);
        return;
    }
    name[n] = '\0';

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket == 0) {
            clients[i].socket = sock;
            strncpy(clients[i].name, name, NAME_LEN);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    while ((n = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';

        if (strncmp(buffer, "LIST", 4) == 0) {
            list_clients(sock);
        } else if (strncmp(buffer, "2ALL ", 5) == 0) {
            char message[BUFFER_SIZE];
            snprintf(message, sizeof(message), "%s: %s", name, buffer + 5);
            send_to_all(message, sock);
        } else if (strncmp(buffer, "2ONE ", 5) == 0) {
            char *recipient = strtok(buffer + 5, " ");
            char *message_text = strtok(NULL, "");
            char message[BUFFER_SIZE];
            snprintf(message, sizeof(message), "%s: %s", name, message_text);
            send_to_one(message, recipient);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            break;
        }
    }

    close(sock);
    remove_client(sock);
}

void *client_handler(void *arg) {
    int sock = *(int *)arg;
    handle_client(sock);
    free(arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", ip, port);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;
        if (pthread_create(&tid, NULL, client_handler, pclient) != 0) {
            perror("pthread_create");
            free(pclient);
            continue;
        }
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>

#define SERVER_QUEUE "/server_queue"
#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 1024

struct client {
    mqd_t queue;
    char queue_name[128];
};

struct client clients[MAX_CLIENTS];
int client_count = 0;

void handle_init(mqd_t server, char* client_queue_name) {
    if (client_count >= MAX_CLIENTS) {
        printf("Reached maximum number of clients.\n");
        return;
    }

    struct client *new_client = &clients[client_count];
    strcpy(new_client->queue_name, client_queue_name);
    new_client->queue = mq_open(client_queue_name, O_WRONLY);

    char msg[128];
    sprintf(msg, "INIT %d", client_count);
    mq_send(new_client->queue, msg, strlen(msg) + 1, 0);

    printf("New client registered: %s with ID %d\n\n", client_queue_name, client_count);
	client_count++;
}

void broadcast_message(const char* msg) {
    for (int i = 0; i < client_count; i++) {
        mq_send(clients[i].queue, msg, strlen(msg) + 1, 0);
    }
}

int main() {
    mqd_t server_queue;
    struct mq_attr attr = {0, 10, MAX_MSG_SIZE, 0};
    mq_unlink(SERVER_QUEUE);
    server_queue = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);

    char buffer[MAX_MSG_SIZE];
    while (1) {
        ssize_t bytes_read = mq_receive(server_queue, buffer, MAX_MSG_SIZE, NULL);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            if (strncmp(buffer, "INIT ", 5) == 0) {
                handle_init(server_queue, buffer + 5);
            } else {
                broadcast_message(buffer);
                printf("%s\n", buffer);
            }
        }
    }

    mq_close(server_queue);
    mq_unlink(SERVER_QUEUE);
    return 0;
}

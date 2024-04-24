#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <client queue name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *client_queue_name = argv[1];
    mqd_t client_queue, server_queue;
    struct mq_attr attr = {0, 10, MAX_MSG_SIZE, 0};

    mq_unlink(client_queue_name);
    client_queue = mq_open(client_queue_name, O_CREAT | O_RDONLY, 0666, &attr);
    server_queue = mq_open("/server_queue", O_WRONLY);

    char msg[MAX_MSG_SIZE];
    sprintf(msg, "INIT %s", client_queue_name);
    mq_send(server_queue, msg, strlen(msg) + 1, 0);

    printf("Client started. Waiting for server response...\n");
    int client_id;
    ssize_t bytes_read = mq_receive(client_queue, msg, MAX_MSG_SIZE, NULL);
	
    if (bytes_read > 0) {
        msg[bytes_read] = '\0';
        if (sscanf(msg, "INIT %d", &client_id) == 1) {
            printf("Received from server: %s\n", msg);
            printf("Assigned client ID: %d\n\n", client_id);
        } else {
            fprintf(stderr, "Failed to parse client ID from message\n");
        }
    } else {
        fprintf(stderr, "Failed to receive message from server\n");
    }
	
	pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            ssize_t bytes_read = mq_receive(client_queue, msg, MAX_MSG_SIZE, NULL);
            if (bytes_read > 0) {
                msg[bytes_read] = '\0';
		int sender_id;
		char content[MAX_MSG_SIZE];
		sscanf(msg, "Client %d: %[^\n]", &sender_id, content);
		if(client_id != sender_id){
		   printf("%s", msg);
		}
            }
        }
        exit(EXIT_SUCCESS);
    }else{
		while (fgets(msg, MAX_MSG_SIZE, stdin) != NULL) {
        char formatted_msg[MAX_MSG_SIZE];
        snprintf(formatted_msg, sizeof(formatted_msg), "Client %d: %s", client_id, msg);
        mq_send(server_queue, formatted_msg, strlen(formatted_msg) + 1, 0);
		}
	}
	
    mq_close(client_queue);
    mq_close(server_queue);
    mq_unlink(client_queue_name);

    return 0;
}

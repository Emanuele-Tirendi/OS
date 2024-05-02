#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../shared/constants.h"

#define HTML_FILE "index.html"

/*
Here we will make the main if else case distinction for the client commands. So
if you introduce a new command coming from the client, make a new else if case in this 
function.
*/
void handle_client_command(char* client_input, int sock) {
    if (strcmp_wl(client_input, "echo\n") == 0) {
        usleep(2000000);
        printf("client sent %s", client_input);
        send(sock, client_input, strlen(client_input), 0);
    } else {
        printf("not valid client command: %s\n", client_input);
    }
}

void *handle_client(void *socket_desc) {
    int* socket = (int*) socket_desc;
    int sock = *socket;

    // buffer for messages sent from client.
    char client_input[CLIENT_INPUT_SIZE];
    /*
    This loop is the main loop for the thread to get messages of the client
    and handle them.
    */
    while (1) {
        // zero out the buffer
        memset(client_input, 0, CLIENT_INPUT_SIZE);
        // receive message from client
        ssize_t bytes_received = recv(sock, client_input, CLIENT_INPUT_SIZE, 0);
        // detect if clients isn't connected anymore
        if (bytes_received == 0) {
            printf("client not connected anymore\n");
            close(sock);
            return NULL;
        }
        handle_client_command(client_input, sock);
    }
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("creating server socket failed\n");
        return 0;
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        printf("setting server socket options failed\n");
        return 0;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to localhost and specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("binding server socket to localhost and specified port failed\n");
        return 0;
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        printf("listening for connections in server failed\n");
        return 0;
    }
    
    while (1) {
        // accept incoming client
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        // if accepting doesn't work, just skip
        if (new_socket < 0) {
            printf("failed to aaccept a client\n");
            continue;
        }
        // create for each client a thread
        int thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)&new_socket) < 0) {
            printf("could not create thread for client\n");
            close(new_socket);
            continue;
        }
        
        // if thread terminates, give back resources to system without
        // need to join it
        pthread_detach(thread_id);
    }
    return 0;
}
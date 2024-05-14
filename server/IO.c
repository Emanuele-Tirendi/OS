#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../shared/constants.h"
#include "../shared/log.h"
#include "../shared/time.h"

bool need_to_close = false;

void handle_echo(int sock) {
    log_m('s', 'l', "echo");
    send(sock, "echo\n", strlen("echo\n"), 0);
}

void close_socket(int sock) {
    log_m('s', 'l', "connection lost");
    printf("CONNECTION LOST");
    close(sock);
    need_to_close = true;
}

void* send_ping(void* socket_desc) {
    int* socket = (int*) socket_desc;
    int sock = *socket;

    usleep(PINGPONG_WAIT);
    send(sock, "PING", strlen("PING"), 0);
    return NULL;
}

void handle_pingpong(int sock) {
    if (in_time()) {
        log_m('s', 'p', "PONG");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, send_ping, (void*)&sock) < 0) {
            printf("could not create thread for pingpong\n");
        }
        
        // if thread terminates, give back resources to system without
        // need to join it
        pthread_detach(thread_id);
    } else {
        close_socket(sock);
    }
}

/*
Here we will make the main if else case distinction for the client commands. So
if you introduce a new command coming from the client, make a new else if case in this 
function.
*/
void handle_client_command(char* client_input, int sock) {
    if (strcmp_wl(client_input, "echo\n") == 0) {
        handle_echo(sock);
    } else if (strcmp_wl(client_input, "PONG") == 0) {
        handle_pingpong(sock);
    } else {
        printf("not valid client command: %s\n", client_input);
    }
}

void *handle_client(void *socket_desc) {
    int* socket = (int*) socket_desc;
    int sock = *socket;

    // buffer for messages sent from client.
    char client_input[CLIENT_INPUT_SIZE];

    send(sock, "PING", strlen("PING"), 0);

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
            close_socket(sock);
        }
        if (need_to_close) {
            return NULL;
        }
        handle_client_command(client_input, sock);
    }
    return NULL;
}
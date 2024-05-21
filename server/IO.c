#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <limits.h>

#include "html.h"
#include "IO.h"
#include "../shared/constants.h"
#include "../shared/log.h"
#include "../shared/time.h"

bool need_to_close[NUMBER_OF_CLIENTS][2];

time_t last[NUMBER_OF_CLIENTS];

void handle_echo(int sock) {
    log_m('s', 'l', 0, "echo");
    send(sock, "echo\n", strlen("echo\n"), 0);
}

void close_socket(int sock, int id) {
    close(sock);
    need_to_close[id][0] = true;
}

void* send_ping(void* socket_desc) {
    int* socket = (int*) socket_desc;
    int sock = *socket;

    usleep(PINGPONG_WAIT);
    send(sock, "PING", strlen("PING"), 0);
    return NULL;
}

void handle_pingpong(int sock, int id) {
    if (in_time(&last[id])) {
        log_m('s', 'p', 0, "PONG");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, send_ping, (void*)&sock) < 0) {
            printf("could not create thread for pingpong\n");
        }
        
        // if thread terminates, give back resources to system without
        // need to join it
        pthread_detach(thread_id);
    } else {
        log_m('s', 'l', 0, "connection lost due to pingpong");
        printf("CONNECTION LOST DUE TO PINGPONG");
        close_socket(sock, id);
    }
}

/*
Here we will make the main if else case distinction for the client commands. So
if you introduce a new command coming from the client, make a new else if case in this 
function.
*/
void handle_client_command(char* client_input, int sock, int id) {
    if (strcmp_wl(client_input, "echo\n") == 0) {
        handle_echo(sock);
    } else if (strcmp_wl(client_input, "PONG") == 0) {
        handle_pingpong(sock, id);
    } else if (strcmp_wl(client_input, "html\n") == 0){
        send_html(sock);
    } else if (strcmp_wl(client_input, "quit\n") == 0) {
        log_m('s', 'l', 0, "quit connection");
        close_socket(sock, id);
    } else if (strncmp(client_input, "insert", strlen("insert")) == 0){
        handle_insert(client_input);
        send(sock, "insert line complete", strlen(client_input), 0);
    } else if (strncmp(client_input, "delete", strlen("delete")) == 0){
        handle_delete(client_input);
        send(sock, "change line complete", strlen(client_input), 0);
    } else if (strncmp(client_input, "change", strlen("change")) == 0) {
        handle_change(client_input);
        send(sock, "change line complete", strlen(client_input), 0);
    } else {
        printf("not valid client command: %s\n", client_input);
    }
}

void *handle_client(void *socket_desc) {
    struct thread_arg* arg = (struct thread_arg*) socket_desc;
    int sock = arg->sock;
    int id = arg->id;

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
            if (!need_to_close[id][0]) {
                close_socket(sock, id);
            }
        }
        if (need_to_close[id][0]) {
            char buffer[strlen("close connection for ") + get_no_digits(INT_MAX)];
            sprintf(buffer, "close connection for %d", id);
            log_m('s', 'l', 0, buffer);
            need_to_close[id][1] = false;
            return NULL;
        }
        handle_client_command(client_input, sock, id);
    }
    return NULL;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<stdio.h>
#include<sys/mman.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include "IO.h"
#include "html.h"
#include "../shared/constants.h"
#include "../shared/time.h"
#include "../shared/log.h"

/*
The idea of the software design is that every action that happens goes as follows:
1. User makes an input.
2. Client handles it (e.g. by sending commands to the server and receiving commands
   from the server).

In this process, sometimes it is better when the user is temporarily disabled to
make new user inputs (so that the client and server can handle the already entered
user inputs). So, the handle_user sets the boolean user_input_possible immediately after
receiving a user input to false. Then the user is disabled to provide user input unless
another procedure sets it to true again.
*/
bool user_input_possible = false;
bool need_to_close = false;

time_t last = -1;

void handle_file_operations(int sock, char* user_input, char op, int id) {
    if (id) {
        user_input +=3;
    }
    int i = valid_file_operation_input(user_input, op, id);
    if (i == 1) {
        if (id) {
            user_input -=3;
        }
        send(sock, user_input, strlen(user_input), 0);
    } if (i == 0) {
        printf("Either syntax error or newline in string or number smaller than or equal to zero.\n");
        user_input_possible = true;
    }
    if (i == -1) {
        printf("Line doesn't exist.\n");
        user_input_possible = true; 
    }
}

void close_socket(int sock) {
    close(sock);
    need_to_close = true;
    const char *filename = "index.html";
    if (access(filename, F_OK) != -1) {
        remove("index.html");
    }
}

void* send_pong(void* socket_desc) {
    int* socket = (int*) socket_desc;
    int sock = *socket;

    usleep(PINGPONG_WAIT);
    send(sock, "PONG", strlen("PONG"), 0);
    return NULL;
}

void handle_pingpong(int sock) {
    if (true) {
        log_m('c', 'p', getpid(), "PING");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, send_pong, (void*)&sock) < 0) {
            printf("could not create thread for pingpong\n");
        }
        
        // if thread terminates, give back resources to system without
        // need to join it
        pthread_detach(thread_id);
    } else {
        //close_socket(sock);
    }
}

void handle_echo() {
    printf("echo\n");
    user_input_possible = true;
}

void handleQuit(int sock) {
        send(sock, "quit\n", strlen("quit\n"), 0);
        log_m('c', 'l', getpid(), "quit application");
        printf("quit application");
        close_socket(sock);
}

/*
Here we will make the main if else case distinction for the user commands. So
if you introduce a new command coming from the user, make a new else if case in this 
function.
*/
void handle_user_command(char* user_input, int sock) {
    if (strcmp_wl(user_input, "echo\n") == 0) {
        send(sock, user_input, strlen(user_input), 0);
    } else if (strcmp_wl(user_input, "quit\n") == 0){
        handleQuit(sock);
    } else if (strcmp_wl(user_input, "get_html\n") == 0) {
        send(sock, user_input, strlen(user_input), 0);
    } else if (strcmp_wl(user_input, "open_html\n") == 0) {
        open_html();
        user_input_possible = true;
    } else if (strcmp_wl(user_input, "pid\n") == 0) {
        printf("pid: %d\n", (int) getpid());
        user_input_possible = true;
    } else if (strncmp(user_input, "insert", strlen("insert")) == 0){
        handle_file_operations(sock, user_input, 'i', 0);
    } else if (strncmp(user_input, "delete", strlen("delete")) == 0){
        handle_file_operations(sock, user_input, 'd', 0);
    } else if (strncmp(user_input, "change", strlen("change")) == 0){
        handle_file_operations(sock, user_input, 'c', 0);
    } else if (strncmp(user_input, "id$insert", strlen("id$insert")) == 0){
        handle_file_operations(sock, user_input, 'i', 1);
    } else if (strncmp(user_input, "id$delete", strlen("id$delete")) == 0){
        handle_file_operations(sock, user_input, 'd', 1);
    } else if (strncmp(user_input, "id$change", strlen("id$change")) == 0){
        handle_file_operations(sock, user_input, 'c', 1);
    } else if (strncmp(user_input, "check_id", strlen("check_id")) == 0) {
        print_ids();
        send(sock, user_input, strlen(user_input), 0);
        user_input_possible = true;
    } else {
        printf("not valid user command: %s", user_input);
        user_input_possible = true;
    }
}

/*
Here we will make the main if else case distinction for the server commands. So
if you introduce a new command coming from the server, make a new else if case in this 
function.
*/
void handle_server_command(char* server_input, int sock) {
    if (strcmp_wl(server_input, "echo\n") == 0) {
        handle_echo();
    } else if (strcmp_wl(server_input, "PING") == 0) {
        handle_pingpong(sock);
    } else if (strncmp(server_input, "html", strlen("html")) == 0) {
        get_html(server_input);
        user_input_possible = true;
    } else if(strncmp(server_input, "insert line complete", strlen("insert line complete")) == 0){
        printf("server sent inserting successful!\n");
        user_input_possible = true;
    } else if (strncmp(server_input, "delete line complete", strlen("delete line complete")) == 0){
        printf("server sent deleting successful!\n");
        user_input_possible = true;
    } else if (strncmp(server_input, "change line complete", strlen("change line complete")) == 0){
        printf("server sent changing successful!\n");
        user_input_possible = true;
    } else if(strncmp(server_input, "insert line not complete", strlen("insert line not complete")) == 0){
        printf("server sent inserting not successful!\n");
        user_input_possible = true;
    } else if (strncmp(server_input, "delete line not complete", strlen("delete line not complete")) == 0){
        printf("server sent deleting not successful!\n");
        user_input_possible = true;
    } else if (strncmp(server_input, "change line not complete", strlen("change line not complete")) == 0){
        printf("server sent changing not successful!\n");
        user_input_possible = true;
    } else {
        printf("not valid server command: %s\n", server_input);
    }
}

void* handle_user(void* input) {
    // get input
    struct thread_arg* args = (struct thread_arg*) input;
    int sock = args->sock;

    // initialize buffer for user input
    char user_input[USER_INPUT_SIZE];
    user_input_possible = true;
    /*
    This loop is the main loop for the thread to get messages from the user
    and handle them.
    */
    while(1) {
        if (!user_input_possible) {
            usleep(100000);
            continue;
        }
        // get and handle user input
        memset(user_input, 0, USER_INPUT_SIZE);
        printf("Enter a command:\n");
        fgets(user_input, USER_INPUT_SIZE, stdin);
        user_input_possible = false;
        handle_user_command(user_input, sock);
    }
}

void* handle_server(void* input) {
    // get input
    struct thread_arg* args = (struct thread_arg*) input;
    int sock = args->sock;

    // initialize buffer for server input
    char server_input[SERVER_INPUT_SIZE];
    /*
    This loop is the main loop for the thread to get messages from the server
    and handle them.
    */
    while (1) {
        // get and handle server input
        memset(server_input, 0, SERVER_INPUT_SIZE);
        ssize_t bytes_received = recv(sock, server_input, SERVER_INPUT_SIZE, 0);
        // detect if connection is lost
        if (bytes_received == 0) {
            if (!need_to_close) {
                log_m('c', 'l', getpid(), "connection lost");
                printf("CONNECTION LOST");
                close_socket(sock);
            }
        }
        if (need_to_close) {
            exit(0);
        }
        handle_server_command(server_input, sock);
    }
}
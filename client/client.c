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


#include "../shared/constants.h"

struct thread_arg {
    int sock;
};

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

/*
Here we will make the main if else case distinction for the user commands. So
if you introduce a new command coming from the user, make a new else if case in this 
function.
*/
void handle_user_command(char* user_input, int sock) {
    if (strcmp_wl(user_input, "echo\n") == 0) {
        send(sock, user_input, strlen(user_input), 0);
    } 
    else if (strcmp_wl(user_input, "html\n") == 0) {
        send(sock, user_input, strlen(user_input), 0);
    }
    else if (strcmp_wl(user_input, "quit\n") == 0){
        send(sock, user_input, strlen(user_input), 0);
        printf("Disconnect from the server\n");
        close(sock);
    }
    else {
        printf("not valid user command: %s", user_input);
    }
}

/*
Here we will make the main if else case distinction for the server commands. So
if you introduce a new command coming from the server, make a new else if case in this 
function.
*/
void handle_server_command(char* server_input, int sock) {
    if (strcmp_wl(server_input, "echo\n") == 0) {
        printf("server sent %s", server_input);
        user_input_possible = true;
    } 
    else if (strncmp(server_input, "<!DOCTYPE html>", strlen("<!DOCTYPE html>")) == 0){
        FILE *html;
        html = fopen("index.html", "w");
        fprintf(html, "%s" ,server_input);
        fclose(html);
        system("xdg-open index.html");
        user_input_possible = true;
    }
    else {
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
            printf("connection to server lost\n");
            close(sock);
            const char *filename = "index.html";
            if (access(filename, F_OK) != -1) {
                remove("index.html");
            }
            exit(0);
        }
        handle_server_command(server_input, sock);
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Client socket cannot be created\n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return 0;
    }
    
    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nClient cannot connect to server \n");
        return 0;
    }
    
    // initialize user handler and server handler
    pthread_t user_handler_id, server_handler_id;
    pthread_attr_t user_handler_attr, server_handler_attr;
    pthread_attr_init(&user_handler_attr);
    pthread_attr_init(&server_handler_attr);
    struct thread_arg args;
    args.sock = sock;
    struct thread_arg* arg_ptr = &args;
    void* void_ptr = (void*) arg_ptr;
    pthread_create(&user_handler_id, &user_handler_attr, &handle_user, void_ptr);
    pthread_create(&server_handler_id, &server_handler_attr, &handle_server, void_ptr);

    pthread_join(user_handler_id, NULL);
    pthread_join(server_handler_id, NULL);
    
    close(sock);
    return 0;
}
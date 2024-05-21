#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../shared/constants.h"
#include "../shared/time.h"
#include "../shared/log.h"
#include "IO.h"
#include "html.h"

int get_next_id() {
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        if (!need_to_close[i][1]) {
            need_to_close[i][1] = true;
            return i;
        }
    }
    return -1;
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
    
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        need_to_close[i][1] = 0;
        last[i] = -1;
    }

    log_m('s', 'l', 0, "start logging");

    initialize_html();

    while (1) {
        // accept incoming client
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        // if accepting doesn't work, just skip
        if (new_socket < 0) {
            printf("failed to aaccept a client\n");
            continue;
        }
        // create for each client a thread
        pthread_t thread_id;
        struct thread_arg arg;
        arg.sock = new_socket;
        arg.id = get_next_id();
        if(arg.id == -1) {
            usleep(1000000);
            continue;
        }
        need_to_close[arg.id][0] = false;
        last[arg.id] = -1;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)&arg) < 0) {
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
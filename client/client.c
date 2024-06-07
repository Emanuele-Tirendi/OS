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
#include "../shared/log.h"
#include "../shared/constants.h"

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

    log_m('c', 'l', (int) getpid(), "start logging");

    initialize_ids();

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
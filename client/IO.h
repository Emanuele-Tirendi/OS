#ifndef CLIENT_IO_H
#define CLIENT_IO_H

struct thread_arg {
    int sock;
};

void* handle_server(void* input);

void* handle_user(void* input);

#endif
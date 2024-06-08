#ifndef SERVER_IO_H
#define SERVER_IO_H

#include "../shared/constants.h"

#include <stdbool.h>
#include <pthread.h>
#include <time.h>

struct thread_arg {
    int sock;
    int id;
};

extern bool need_to_close[NUMBER_OF_CLIENTS][2];

extern time_t last[NUMBER_OF_CLIENTS];

void *handle_client(void *socket_desc);

void initialize_IO();

#endif
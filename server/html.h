#ifndef SERVER_HTML_H
#define SERVER_HTML_H

#include "../shared/constants.h"

void handle_insert(char* client_input);

void handle_delete(char* client_input);

void handle_change(char* client_input);

void initialize_html();

void send_html(int sock);

#endif
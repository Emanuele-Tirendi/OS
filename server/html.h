#ifndef SERVER_HTML_H
#define SERVER_HTML_H

#include "../shared/constants.h"

int handle_insert(char* client_input);

int handle_delete(char* client_input);

int handle_change(char* client_input);

int handle_id_insert(char* client_input);

int handle_id_delete(char* client_input);

int handle_id_change(char* client_input);

void initialize_html();

void send_html(int sock);

#endif
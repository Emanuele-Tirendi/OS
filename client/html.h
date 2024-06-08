#ifndef CLIENT_HTML_H
#define CLIENT_HTML_H

#include <stdbool.h>

int valid_file_operation_input(char* user_input, char opc, int id);

void get_html(char html[]);

void open_html();

void print_ids();

void initialize_ids();

#endif
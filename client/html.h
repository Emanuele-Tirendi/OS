#ifndef CLIENT_HTML_H
#define CLIENT_HTML_H

#include <stdbool.h>

bool valid_file_operation_input(char* user_input, char op);

void get_html(char html[]);

void open_html();

#endif
#ifndef SERVER_HTML_H
#define SERVER_HTML_H

#include "../shared/constants.h"

#define HTML_NAME "example.html"
#define TEMP_NAME "temp.html"

struct html_input {
    int line_index;
    char content[CLIENT_INPUT_SIZE];
};

void parse_html_input(char* client_input, struct html_input* parsed_inputs, char mode);

// for appending from parameter "from" to the end of file without knowing the length
// just insert -1 for "to"
void append(char* target, char* source, int from, int to);

void append_line(char* target, char* content);

void send_html(int sock);

#endif
#ifndef SHARED_HTML_H
#define SHARED_HTML_H

#include "constants.h"
#include <stdbool.h>

struct html_input {
    bool valid;
    int line_index;
    char content[CLIENT_INPUT_SIZE];
};

void parse_html_input(char* client_input, struct html_input* parsed_inputs, char mode);

#endif
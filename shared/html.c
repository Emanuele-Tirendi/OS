#include "html.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void parse_html_input(char* client_input, struct html_input* parsed_inputs, char mode) {
    const char delimiter[] = "$";
    char *token;

    char copy[CLIENT_INPUT_SIZE];
    strcpy(copy, client_input);
    token = strtok(copy, delimiter);

    int no_parts;
    if (mode == 'd') {
        no_parts = 2;
    } else if (mode == 'i' || mode == 'c') {
        no_parts = 3;
    } else {
        // TODO: handle
    }

    int count = 0;
    char* parts[no_parts];
    for (int i = 0; i < no_parts; i++) {
        parts[no_parts] = NULL;
    }
    while (token != NULL /*&& count < no_parts*/) {
        parts[count] = token;
        count++;
        token = strtok(NULL, delimiter);
    }
    // store valin into parsed_inputs
    parsed_inputs->valid = true;

    // store line_index into parsed_inputs
    if (parts[1] == NULL) {
        parsed_inputs->valid = false;
    } else {
        // atoi will store 0 if parts[1] is not of integer type
        parsed_inputs->line_index = atoi(parts[1]);   
    }

    // store content into parsed_inputs
    if (mode == 'i' || mode == 'c') {
        if (parts[2] == NULL) {
            parsed_inputs->valid = false;
        } else {
            strcpy(parsed_inputs->content, parts[2]);
        }
    }
}
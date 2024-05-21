#include "html.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>

#include "../shared/constants.h"

void parse_html_input(char* client_input, struct html_input* parsed_inputs, char mode) {
    const char delimiter[] = "$";
    char *token;

    token = strtok(client_input, delimiter);

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
    while (token != NULL && count < no_parts) {
        parts[count] = token;
        count++;
        token = strtok(NULL, delimiter);
    }

    parsed_inputs->line_index = atoi(parts[1]);
    if (mode == 'i' || mode == 'c') {
        strcpy(parsed_inputs->content, parts[2]);
    }
}

void append(char* target, char* source, int from, int to) {
    FILE *sourcefile = fopen(source, "r");
    FILE *targetfile = fopen(target, "a");
    char buffer[CLIENT_INPUT_SIZE];
    int currentLine = 1;
    while (fgets(buffer, sizeof(buffer), sourcefile)) {
            if (currentLine >= from) {
                fprintf(targetfile, "%s", buffer);
            }
            if (currentLine == to) {
                break;
            }
            currentLine++;
    }
    if (currentLine < to) {
        // TODO: handle
    }
    fclose(sourcefile);
    fclose(targetfile);
}

void append_line(char* target, char* content) {
    FILE* targetfile = fopen(target, "a");
    fprintf(targetfile, "%s", content);
    fclose(targetfile);
}

void initialize_html() {
    append(HTML_NAME, ORIGINAL_NALE, 1, -1);
}

void send_html(int sock) {
    FILE *html_file;
    html_file = fopen(HTML_NAME, "r");
    char buffer[SERVER_INPUT_SIZE];
    int i = 0;
    char c;
    while((c = fgetc(html_file)) != EOF){
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';

    fclose(html_file);
    send(sock, buffer, strlen(buffer), 0);
}
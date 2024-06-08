#include "html.h"
#include "../shared/html.h"
#include "../shared/constants.h"

#include <stdio.h>
#include <stdlib.h>

int line_indices[FILE_LENGTH];

bool contains_newline_size_USER_INPUT(char content[]) {
        char curr = content[0];

        for (int i = 0; i < CLIENT_INPUT_SIZE; i++) {
                if (content[i] == '\n') {
                        return false;
                }
        }
        return true;
}

int valid_file_operation_input(char* user_input, char op, int id) {
        struct html_input parsed;
        parse_html_input(user_input, &parsed, op);
        if (!parsed.valid) return 0;
        if (parsed.line_index <= 0) return 0;
        if (op != 'd') {
                if (contains_newline_size_USER_INPUT(parsed.content)) return 0;
        }
        int b = 1;
        int new_id;
        if (id) {
                for (int i = 1; i < FILE_LENGTH; i++) {
                        b = -1;
                        if (line_indices[i] == parsed.line_index) {
                                b = 1;
                                new_id = i;
                                break;
                        }
                }
                if (b == 1) {
                        char buffer[CLIENT_INPUT_SIZE];
                        int pos = 0;
                        char operation[7];
                        if (op == 'd') snprintf(operation, 7, "delete");
                        if (op == 'c') snprintf(operation, 7, "change");
                        if (op == 'i') snprintf(operation, 7, "insert");;
                        pos += snprintf(buffer+pos, 7, "%s", operation);
                        pos += snprintf(buffer+pos, 2, "$");
                        pos += snprintf(buffer+pos, get_no_digits(new_id) + 1, "%d", new_id);
                        pos += snprintf(buffer+pos, 2, "$");
                        snprintf(buffer+pos, CLIENT_INPUT_SIZE, "%s", user_input+7+get_no_digits(parsed.line_index));
                        snprintf(user_input, CLIENT_INPUT_SIZE, "%s", buffer);
                }
        }
        return b;
}

int parse_ids(char* c) {
        int pos = 0;
        int id;
        while(id = atoi(c + pos)) {
                pos+=get_no_digits(id);
                pos+=2;
                int line = atoi(c + pos);
                pos+=get_no_digits(line);
                pos+=2;
                line_indices[id] = line;
        }
        return pos;
}

void get_html(char server_input[]) {
        FILE *html;
        html = fopen("index.html", "w");
        server_input+=4;
        server_input+=parse_ids(server_input);
        fprintf(html, "%s" ,server_input);
        fclose(html);
}

void open_html() {
        // if file is not present, it is printed:
        // gio: file:///home/emanuele/Documents/OS/OS/client/index.html: Error when getting information for file “/home/emanuele/Documents/OS/OS/client/index.html”: No such file or directory
        system("xdg-open index.html");
}

void print_ids() {
    for (int i = 1; i < FILE_LENGTH; i++) {
        printf("id: %d, line: %d\n", i, line_indices[i]);
    }
}

void initialize_ids() {
        for (int i = 0; i < FILE_LENGTH; i++) {
                line_indices[i] = 0;
        }
}
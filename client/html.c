#include "html.h"
#include "../shared/html.h"

#include <stdio.h>
#include <stdlib.h>

bool contains_newline_size_USER_INPUT(char content[]) {
        char curr = content[0];

        for (int i = 0; i < CLIENT_INPUT_SIZE; i++) {
                if (content[i] == '\n') {
                        return false;
                }
        }
        return true;
}

bool valid_file_operation_input(char* user_input, char op) {
        struct html_input parsed;
        parse_html_input(user_input, &parsed, op);
        if (!parsed.valid) return false;
        if (parsed.line_index <= 0) return false;
        if (contains_newline_size_USER_INPUT(parsed.content)) return false;
        return true;
}

void get_html(char server_input[]) {
        FILE *html;
        html = fopen("index.html", "w");
        server_input+=4;
        fprintf(html, "%s" ,server_input);
        server_input-=4;
        fclose(html);
}

void open_html() {
        // TODO: handle if file isn't on system
        system("xdg-open index.html");
}
#include "html.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>

#include "../shared/constants.h"
#include "../shared/log.h"
#include "../shared/html.h"
#include "id.h"

#define ORIGINAL_NALE "original.html"
#define HTML_NAME "example.html"
#define TEMP_NAME "temp.html"

int get_length(char* file_name) {
    FILE *fp;
    int count = 0; // Line counter (result)
    char c; // To store a character read from file
 
    // Open the file
    fp = fopen(file_name, "r");
 
    // Check if file exists
    if (fp == NULL)
    {
        return -1;
    }
 
    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
 
    return count;
}

// for appending from parameter "from" to the end of file without knowing the length
// just insert -1 for "to"
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

void insert(int line_index, char* content) {
    if (line_index > 1) {
        append(TEMP_NAME, HTML_NAME, 1, line_index -1);
    }
    append_line(TEMP_NAME, content);
    append(TEMP_NAME, HTML_NAME, line_index, -1);

    remove(HTML_NAME);
    rename(TEMP_NAME, HTML_NAME);
}

void delete(int line_index) {
    if (line_index > 1) {
        append(TEMP_NAME, HTML_NAME, 1, line_index -1);
    }
    append(TEMP_NAME, HTML_NAME, line_index + 1, -1);

    remove(HTML_NAME);
    rename(TEMP_NAME, HTML_NAME);
}

void change(int line_index, char* content) {
    if (line_index > 1) {
        append(TEMP_NAME, HTML_NAME, 1, line_index -1);
    }
    append_line(TEMP_NAME, content);
    append(TEMP_NAME, HTML_NAME, line_index + 1, -1);

    remove(HTML_NAME);
    rename(TEMP_NAME, HTML_NAME);
}

int handle_insert(char* client_input) {
    struct html_input parsed;
    parse_html_input(client_input, &parsed, 'i');
    int id = get_lineid_from_lineindex(parsed.line_index);
    if (id > 0) {
        if (!insert_id(id)) {
            return 0;
        }
        insert(parsed.line_index, parsed.content);
        return 1;
    }
    return 0;
}

int handle_id_insert(char* client_input) {
    struct html_input parsed;
    client_input += 3;
    parse_html_input(client_input, &parsed, 'i');
    int line_index = get_lineindex_from_lineid(parsed.line_index);
    if (line_index > 0) {
        if (!insert_id(parsed.line_index)) {
            return 0;
        }
        insert(line_index, parsed.content);
        return 1;
    }
    return 0;
}

int handle_delete(char* client_input) {
    struct html_input parsed;
    parse_html_input(client_input, &parsed, 'd');
    int id = get_lineid_from_lineindex(parsed.line_index);
    if (id > 0) {
        if (!delete_id(id)) {
            return 0;
        }
        delete(parsed.line_index);
        return 1;
    }
    return 0;
}

int handle_id_delete(char* client_input) {
    struct html_input parsed;
    client_input += 3;
    parse_html_input(client_input, &parsed, 'd');
    int line_index = get_lineindex_from_lineid(parsed.line_index);
    if (line_index > 0) {
        if (!delete_id(parsed.line_index) == 1) {
            return 0;
        }
        delete(line_index);
        return 1;
    }
    return 0;
}

int handle_change(char* client_input) {
    struct html_input parsed;
    parse_html_input(client_input, &parsed, 'c');
    int id = get_lineid_from_lineindex(parsed.line_index);
    if (id > 0) {
        change(parsed.line_index, parsed.content);
        return 1;
    }
    return 0;
}

int handle_id_change(char* client_input) {
    struct html_input parsed;
    client_input += 3;
    parse_html_input(client_input, &parsed, 'c');
    int line_index = get_lineindex_from_lineid(parsed.line_index);
    if (line_index > 0) {
        change(line_index, parsed.content);
        return 1;
    }
    return 0;
}

void initialize_html() {
    remove(HTML_NAME);
    append(HTML_NAME, ORIGINAL_NALE, 1, -1);
}

void send_html(int sock) {
    FILE *html_file;
    html_file = fopen(HTML_NAME, "r");
    char buffer[SERVER_INPUT_SIZE];
    buffer[0] = 'h';
    buffer[1] = 't';
    buffer[2] = 'm';
    buffer[3] = 'l';
    int i = get_ids(buffer+4) + 4;
    char c;
    while((c = fgetc(html_file)) != EOF){
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';

    fclose(html_file);
    send(sock, buffer, strlen(buffer), 0);
    log_m('s', 'l', 0, "sent html");
}
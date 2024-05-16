#include "html.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>

#include "../shared/constants.h"

void send_html(int sock) {
    FILE *html_file;
    html_file = fopen("example.html", "r");
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
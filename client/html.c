#include "html.h"

#include <stdio.h>
#include <stdlib.h>

void get_html_and_open(char server_input[]) {
        FILE *html;
        html = fopen("index.html", "w");
        fprintf(html, "%s" ,server_input);
        fclose(html);
        system("xdg-open index.html");
}
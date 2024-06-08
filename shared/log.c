#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "constants.h"

void log_f(char path[], char log_message[]) {
    FILE *file = fopen(path, "a"); // Open file in append mode
    if (file == NULL) {
        printf("ERROR LOGGING, COULD NOT OPEN FILE: %s, %s\n", path, log_message); 
        return;
    }
    
    // Get current time
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    // Format time
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // Write message with timestamp to file
    fprintf(file, "[%s] %s\n", timestamp, log_message);
    
    // Close file
    fclose(file);
}

void log_m(char cl_or_se, char log_mode, int id, char log_message[]) {
    // only used if cl_or_se == 'c'
    char part1[] = "../log/client";

    if (cl_or_se == 'c' && log_mode == 'l') {
        char part2[] = "_log.txt";
        char buffer[strlen(part1) + get_no_digits(INT_MAX) + strlen(part2)];
        sprintf(buffer,"%s%d%s", part1, id, part2);
        log_f(buffer, log_message);
    } else if (cl_or_se == 'c' && log_mode == 'p') {
        char part2[] = "_pingpong.txt";
        char buffer[strlen(part1) + get_no_digits(INT_MAX) + strlen(part2)];
        sprintf(buffer,"%s%d%s", part1, id, part2);
        log_f(buffer, log_message);
    } else if (cl_or_se == 's' && log_mode == 'l') {
        log_f("../log/server_log.txt", log_message);
    } else if (cl_or_se == 's' && log_mode == 'p') {
        log_f("../log/server_pingpong.txt", log_message);
    } else {
        printf("ERROR LOGGING, FLAGS WRONG: %c, %c, %s\n", cl_or_se, log_mode, log_message);
    }
}
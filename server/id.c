#include "id.h"
#include "../shared/constants.h"

#include "stdio.h"

// this array stores at position id the line with this id
int line_indices[FILE_LENGTH];

int get_lineindex_from_lineid(int i) {
    return line_indices[i];
}

int get_lineid_from_lineindex(int i) {
    for (int j = 1; j < FILE_LENGTH; j++) {
        if (line_indices[j] == i) {
            return j;
        }
    }
    return 0;
}

int get_new_id() {
    for (int i = 1; i < FILE_LENGTH; i++) {
        if (line_indices[i] == 0) {
            return i;
        }
    }
    return 0;
}

int insert_id(int id) {
    if (!line_indices[id]) {
        return 0;
    }
    int new_id;
    if (!(new_id = get_new_id())) {
        return 0;
    }
    for (int i = 1; i < FILE_LENGTH; i++) {
        if (i != new_id && i != id && line_indices[i] > line_indices[id]) {
            line_indices[i]++;
        }
    }
    line_indices[new_id] = line_indices[id];
    line_indices[id]++;
    return 1;
}

int delete_id(int id) {
    if (!line_indices[id]) {
        return 0;
    }
    for (int i = 1; i < FILE_LENGTH; i++) {
        if (i != id && line_indices[i] > line_indices[id]) {
            line_indices[i]--;
        }
    }
    line_indices[id] = 0;
    return 1;
}

void initialize_id() {
    for (int i = 1; i <= 37; i++) {
        line_indices[i] = i;
    }
    for (int i = 38; i < FILE_LENGTH; i++) {
        line_indices[i] = 0;
    }
}

void print_ids() {
    for (int i = 1; i < FILE_LENGTH; i++) {
        printf("id: %d, line: %d\n", i, line_indices[i]);
    }
}

int get_ids(char* c) {
    int pos = 0;
    for (int i = 1; i < FILE_LENGTH; i++) {
        pos += snprintf(c + pos, get_no_digits(i) + 1, "%d", i);
        (c+pos)[0] = (c+pos)[1] = '$';
        pos+=2; 
        pos += snprintf(c + pos, get_no_digits(line_indices[i]) + 1, "%d", line_indices[i]);
        (c+pos)[0] = (c+pos)[1] = '|';
        pos+=2;
    }
    return pos;
}
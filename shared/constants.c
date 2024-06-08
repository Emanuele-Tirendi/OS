#include "constants.h"

int get_no_digits(int n) {
    int count = 0;
    do {
        n /= 10;
        ++count;
    } while (n != 0);
    return count;
}

// string compare without length
int strcmp_wl(char* str1, char* str2) {
    int i = 0;
    while (1) {
        if ((str1[i] == '\0') && (str2[i] == '\0')) {
            return 0;
        } else if (str1[i] < str2[i]) {
            return -1;
        } else if (str1[i] > str2[i]) {
            return 1;
        }
        i++;
    }
}
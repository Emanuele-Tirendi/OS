#include <time.h>
#include <stdbool.h>
#include <stdio.h>

#include "time.h"

bool in_time(time_t* last) {
    // get current time
    time_t now = time(NULL);
    // if first time to check, initialize last
    if (*last == -1) {
        *last = now;
        return true;
    } else {
        // if last time checked more than 5 seconda ago, return false
        if ((now - *last) > 5) {
            return false;
        // else, update time and return true
        } else {
            *last = now;
            return true;
        }
    }
}
#include <stdio.h>
#include <string.h>

int main() {
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
        printf("Received: %s", buffer);
    }

    return 0;
}
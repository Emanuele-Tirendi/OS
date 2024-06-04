#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    printf("%s\n", msg);
    exit(1);
}

int main() {
    int pipe_in[2];  // Pipe for sending input to Program B
    int pipe_out[2]; // Pipe for reading output from Program B
    pid_t pid1, pid2;

    // Create pipes
    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
        error("pipe");
    }

    // Fork a child process
    pid1 = fork();
    if (pid1 == -1) {
        error("fork1");
    }

    if (pid1 == 0) { // Child process (Program B)
        // Redirect stdin to read from pipe_in[0]
        dup2(pipe_in[0], STDIN_FILENO);
        close(pipe_in[1]);  // Close unused write end

        // Redirect stdout to write to pipe_out[1]
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_out[0]); // Close unused read end

        // Execute Program B
        sleep(5);
        execlp("./../client/client", "./../client/client", NULL);
        error("execlp1");  // execlp only returns on error
    } else { // Parent process (Program A)
        close(pipe_in[0]);  // Close unused read end
        close(pipe_out[1]); // Close unused write end

        pid2 = fork();
        if (pid2 == -1) {
            error("fork2");
        } else if (pid2 == 0) {
            execlp("./../server/server", "./../server/server", NULL);
            error("execlp2");  // execlp only returns on error
        } else {
            // Communicate with Program B
            const char* echo = "echo\n";
            const char* quit = "quit\n";
            char buffer[BUFFER_SIZE];
            ssize_t bytes_read;

            // Send inputs to Program B
            write(pipe_in[1], echo, strlen(echo));

            // Close the write end to signal EOF to Program B
            //close(pipe_in[1]);

            // Read output from Program B
            while ((bytes_read = read(pipe_out[0], buffer, BUFFER_SIZE - 1)) > 0) {
                buffer[bytes_read] = '\0';
                printf("Program B output: %s", buffer);
            }

            while(1) {
                fp
            }

            write(pipe_in[1], quit, strlen(quit));

            kill(pid2, SIGKILL);

            // Close the read end
            close(pipe_out[0]);

            printf("Program A has finished.\n");
        }
    }

    return 0;
}
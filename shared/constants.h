#ifndef SHARED_CONSTANTS_H
#define SHARED_CONSTANTS_H

#define PORT 8080

// size of messages that user writes through stdin to client
#define USER_INPUT_SIZE 1024
// size of messages that are sent from server to client
#define SERVER_INPUT_SIZE 1024
// size of messages that are sent from client to server
#define CLIENT_INPUT_SIZE 1024

// wait how long for sending back pong
#define PINGPONG_WAIT 1000000

// string compare without length
int strcmp_wl(char* str1, char* str2);

#endif
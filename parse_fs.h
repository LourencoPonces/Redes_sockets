#ifndef PARSE_FS_H
#define PARSE_FS_H 1

#define DEFAULT_PORT "58041"


int input_command_server(int argc, char *argv[], char *port);
char* input_action(int numTokens, char** saveTokens, char* input, long int numberCar);
char* parse_command(char* message);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include "user.h"
#include "parse_user.h"
#include "commands_user.h"

char id_user[5];

void input_command_user(int argc, char *argv[], char *port, char *ip) {
    strcpy(port, DEFAULT_PORT);
    strcpy(ip, FLAG);

    if(argc == 1){
        return;
    }
    else if(argc == 3 && !(strcmp(argv[1],"-n"))) {
        strcpy(ip,argv[2]);
    }
    else if(argc == 3 && !(strcmp(argv[1],"-p"))) {
        strcpy(port,argv[2]);
    }
    else if(argc == 5 && !(strcmp(argv[1],"-n")) && !(strcmp(argv[3],"-p"))) {
        strcpy(ip, argv[2]);
        strcpy(port,argv[4]);
    }
    else{
        fprintf(stderr, "Invalid syntax!\n");
        exit(-1);
    }
}

void input_action(int numTokens, char** saveTokens, char* input, long int numberChar){
      char message[1024]; 

    if((!strcmp(saveTokens[0], "register") || !strcmp(saveTokens[0],"reg")) && numTokens == 2) {
        if(commandREGOK(numTokens, saveTokens, numberChar)){
            strcpy(id_user, saveTokens[1]);
            strcpy(message, "REG ");
            strcat(message, saveTokens[1]);
            strcat(message, "\n");
            send_commandUDP(message);
        }
        else{
            printf("User %s not registered\n", saveTokens[1]);
        }
    }
    else if(commandTLOK(numTokens, saveTokens, numberChar)) {
        send_commandUDP("LTP \n");
    }
    else if(commandTSOK(numTokens, saveTokens, numberChar)){
        printf("topic select or ts\n");
        //only works localy   
    }
    else if(commandTPOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "PTP ");
        //strcat(message, ID(temos que adicionar isto));
        strcat(message, saveTokens[1]);
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(commandQLOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "LQU ");
        //strcat(message, saveTokens[1]);
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(commandQGOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "GQU ");
        //adicionar o topico? nao sei de onde vem
        strcat(message, "\n");
        send_commandTCP(message);
    }
    else if(commandQSOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "QUS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        strcat(message, saveTokens[1]);
        //adicionar qsize, qdata, qimg
        strcat(message, "\n");
        send_commandTCP(message);
    }
    else if(commandASOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "ANS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        //adicionar a questao? nao sei de onde vem
        //adicionar asize, adata, aIMG
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        strcpy(message, "ERR\n");
        send_commandUDP(message);
    }
}


int parse_command() {

    int numTokens = 0;
    char *saveTokens[7];
    char input[50];
    int numberChar;
    if(fgets(input, 50, stdin) == NULL){
        return -1;
    }    

    numberChar = strlen(input);
    input[strcspn(input, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(input, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    input_action(numTokens, saveTokens, input, numberChar);
    return 0;
}



void input_action_received(int numTokens, char** saveTokens, char* buffer, long int numberChar){
      char message[1024]; 
    if((!strcmp(saveTokens[0],"RGR")) && numTokens == 2) {
        if(!strcmp(saveTokens[1], "OK")){
            printf("User %s registered\n", id_user);
        }
        else if(!strcmp(saveTokens[1], "NOK")){
            printf("User %s not registered\n", id_user);
        }
    }/*
    else if(commandTLOK(numTokens, saveTokens, numberChar)) {
        send_commandUDP("LTP \n");
    }
    else if(commandTSOK(numTokens, saveTokens, numberChar)){
        printf("topic select or ts\n");
        //only works localy   
    }
    else if(commandTPOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "PTP ");
        //strcat(message, ID(temos que adicionar isto));
        strcat(message, saveTokens[1]);
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(commandQLOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "LQU ");
        //strcat(message, saveTokens[1]);
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(commandQGOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "GQU ");
        //adicionar o topico? nao sei de onde vem
        strcat(message, "\n");
        send_commandTCP(message);
    }
    else if(commandQSOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "QUS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        strcat(message, saveTokens[1]);
        //adicionar qsize, qdata, qimg
        strcat(message, "\n");
        send_commandTCP(message);
    }
    else if(commandASOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "ANS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        //adicionar a questao? nao sei de onde vem
        //adicionar asize, adata, aIMG
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        strcpy(message, "ERR\n");
        send_commandUDP(message);
    }*/
}

void parse_command_received(char* buffer){
    int numTokens = 0;
    char *saveTokens[7];
    int numberChar;
    
    numberChar = strlen(buffer);
    buffer[strcspn(buffer, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(buffer, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    input_action_received(numTokens, saveTokens, buffer, numberChar);
}

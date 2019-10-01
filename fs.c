#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "parse.h"

#define max(A, B) ((A)>=(B)?(A):(B))

/*------------------------*/

int createSocket(struct addrinfo* res);

/*------------------------*/


int main(int argc, char *argv[]) {
    struct addrinfo hintsUDP,*resUDP;
    struct addrinfo hintsTCP,*resTCP;
    int fdUDP, fdTCP;
    ssize_t n;
    char buffer[128];
    char port[6];
    struct sockaddr_in addr;
    socklen_t addrlen;
    extern int errno;
    fd_set rfds;
    int maxDescriptor, counter;
    enum {idle, busy} state;

    input_command_server(argc, argv, port);
    printf("port: %s\n", port);

    //UDP--------------------------------------------------
    memset(&hintsUDP,0,sizeof hintsUDP);
    hintsUDP.ai_family=AF_INET;//IPv4
    hintsUDP.ai_socktype=SOCK_DGRAM;//UDP socket
    hintsUDP.ai_flags=AI_PASSIVE|AI_NUMERICSERV;

    n = getaddrinfo(NULL, port, &hintsUDP, &resUDP);
    fdUDP = createSocket(resUDP);

    n = bind(fdUDP,resUDP->ai_addr,resUDP->ai_addrlen);
    
    addrlen=sizeof(addr);
    n = recvfrom(fdUDP, buffer, 128, 0,(struct sockaddr*)&addr,&addrlen);
    
    write(1, "received UDP: ", 15);
    write(1, buffer, n);

    n = sendto(fdUDP, buffer, n, 0, (struct sockaddr*)&addr, addrlen);

    close(fdUDP);

    //TCP-----------------------------------------
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_PASSIVE|AI_NUMERICSERV;

    getaddrinfo(NULL, port, &hintsTCP, &resTCP);

    fdTCP = createSocket(resTCP);
    n = bind(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    memset(buffer, 0, strlen(buffer));
    listen(fdTCP, 5);
    
    int newfd = accept(fdTCP, (struct sockaddr*)&addr, &addrlen);
    
    int b = read(newfd, buffer, 128);
    write(1, "received TCP: ", 15);
    write(1, buffer, b); 

    b = write(newfd, buffer, b);
    close(fdTCP);
}


int createSocket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}


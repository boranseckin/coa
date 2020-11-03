//
//  server.c
//  coa
//
//  Created by Boran Seckin on 2020-11-03.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "common.h"

void server() {
    int sockfd, newsockfd, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("Error binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        if (newsockfd < 0)
            error("Error accepting");
        
        pid = fork();
        if (pid < 0)
            error("Error forking");
        if (pid == 0) {
            close(sockfd);
            handleConn(newsockfd);
            exit(0);
        }
        else close(newsockfd);
    }
}

void handleConn(int sock) {
    long n;
    char buffer[256];

    memset(&buffer, 0, 256);
    
    n = read(sock, buffer, 255);
    if (n < 0)
        error("Error reading from socket");

    printf("Message: %s\n", buffer);

    char * response = "Message has been recieved from server";
    n = write(sock, response, strlen(response));
    if (n < 0)
        error("Error writing to socket");
}

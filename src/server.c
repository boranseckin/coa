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
#include <time.h>

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
    char filename[40];

    char *buffer = malloc(SIZE);
    if (buffer == NULL) {
        error("Error allocating memory");
    }

    memset(buffer, 0, SIZE);

    n = read(sock, buffer, SIZE);
    if (n < 0)
        error("Error reading from socket");
    else if (n == 0) {
        printf("Not recieved any bytes... passing\n");
        exit(0);
    }

    printf("Recieved %ld bytes\n", n);

    // https://stackoverflow.com/a/1442131/10161292
    time_t ts = time(NULL);
    struct tm tm = *localtime(&ts);
    sprintf(filename, "%d-%02d-%02d_%02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error, opening new file\n");
        exit(0);
    }

    printf("Saved in %s\n", filename);

    n = fwrite(buffer, 1, strlen(buffer), file);
    if (n != strlen(buffer))
        error("Error writing into file");

    sprintf(buffer, "%lu", hash(buffer));
    
    n = write(sock, buffer, strlen(buffer));
    if (n < 0)
        error("Error writing to socket");

    free(buffer);
    fclose(file);
}

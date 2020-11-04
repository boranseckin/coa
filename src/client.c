//
//  client.c
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
#include <netdb.h>
#include <unistd.h>

#include "common.h"

void client(char *addr) {
    int sockfd;
    long n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    server = gethostbyname(addr);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(PORT);

    printf("Connecting to %s...", server->h_name);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Error connecting");

    printf("Done\n");
    printf("Enter message: ");

    memset(&buffer, 0, 256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("Error writing to socket");

    memset(&buffer, 0, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("Error reading from socket");

    printf("%s\n", buffer);
}

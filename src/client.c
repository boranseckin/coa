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

void client(char *addr, char *filename) {
    int sockfd;
    long n, size;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
        error("Error opening file");

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    if (size > SIZE) {
        fprintf(stderr, "Error, file is too big\n");
        exit(1);
    } else if (size == 0) {
        fprintf(stderr, "Error, file is empty\n");
        exit(1);
    }

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

    printf("Connecting to %s...\n", server->h_name);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Error connecting");

    char *buffer = malloc(sizeof(char) * size);
    if (buffer == NULL) {
        fprintf(stderr, "Error, allocating memory\n");
        exit(0);
    }

    long readSize = fread(buffer, 1, size, file);
    if (readSize != size)
        error("Error reading file into memory");

    printf("read: %ld, size: %ld\n", readSize, size);

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("Error writing to socket");

    unsigned long msgHash = hash(buffer);

    memset(buffer, 0, size);
    n = read(sockfd, buffer, size);
    if (n < 0)
        error("Error reading from socket");

    if (strtoul(buffer, NULL, 10) != msgHash) {
        fprintf(stderr, "Error, verifying checksum with server\n");
        exit(0);
    }

    printf("correct hash\n");

    free(buffer);
    fclose(file);
}

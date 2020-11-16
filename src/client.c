//
//  client.c
//  coa
//
//  Created by Boran Seckin on 2020-11-03.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "common.h"

void client(char *addr, int method, char *input) {
    int sockfd;
    long n, size, buffer_size, read_size;
    char *buffer;
    FILE *file;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (method == 0) {
        file = fopen(input, "r");
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
    }
    if (method == 1) {
        size = strlen(input);
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

    buffer = malloc(SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error, allocating memory\n");
        exit(0);
    }
    memset(buffer, 0, SIZE);

    if (method == 0) {
        read_size = fread(buffer, 1, size, file);
        if (read_size != size)
            error("Error reading file into memory");
    }
    if (method == 1) {
        strcpy(buffer, input);
        read_size = strlen(buffer);
        if (read_size != size)
            error("Error reading input into memory");
    }

    printf("read: %ld, size: %ld\n", read_size, size);

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("Error writing to socket");

    unsigned long msg_hash = hash(buffer);

    memset(buffer, 0, SIZE);
    n = read(sockfd, buffer, pow(2, sizeof(unsigned long)));
    if (n < 0)
        error("Error reading from socket");

    if (strtoul(buffer, NULL, 10) != msg_hash) {
        fprintf(stderr, "Error, verifying checksum with server\n");
        exit(0);
    }

    printf("correct hash: %s\n", buffer);

    free(buffer);
    if (method == 0)
        fclose(file);
}

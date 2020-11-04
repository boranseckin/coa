//
//  coa.c
//  coa
//
//  Created by Boran Seckin on 2020-11-03.
//  https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
//

#include <string.h>
#include "common.h"

void error(char *msg)
{
    perror(msg);
    exit(1);
}

// djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./coa [-s] [-c host] [-f filename] [-m string]\n");
        exit(1);
    }

    if (strcmp(argv[1], "-s") == 0) {
        server();
        return 0;
    }

    if (argc == 5) {
        if (strcmp(argv[3], "-f") == 0) {
            client(argv[2], 0, argv[4]);
            return 0;
        }

        if (strcmp(argv[3], "-m") == 0) {
            client(argv[2], 1, argv[4]);
            return 0;
        }
    }

    fprintf(stderr, "Usage: ./coa [-s] [-c host] [-f filename] [-m string]\n");
    exit(1);
}
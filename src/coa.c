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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./coa [Server/Client] [Host]\n");
        exit(1);
    }

    if (strcmp(argv[1], "-s") == 0) {
        server();
    }
    else if (strcmp(argv[1], "-c") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Error: Hostname was not supplied\n");
            exit(1);
        }
        client(argv[2]);
    }
}
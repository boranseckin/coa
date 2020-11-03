//
//  coa.c
//  coa
//
//  Created by Boran Seckin on 2020-11-03.
//

#include <string.h>
#include "common.h"

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (strcmp(argv[1], "-s") == 0) {
        server();
    }
    else if (strcmp(argv[1], "-c") == 0) {
        client(argv[2]);
    }
}
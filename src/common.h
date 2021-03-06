//
//  common.h
//  coa
//
//  Created by Boran Seckin on 2020-11-03.
//

#ifndef common_h
#define common_h

#include <stdlib.h>
#include <stdio.h>

#define PORT 8080
#define SIZE 65536

void error(char *msg);
unsigned long hash(char *str);
void server(int mode);
void handleConn(int sock, int *mode);
void client(char *addr, int method, char *input);

#endif /* common_h */

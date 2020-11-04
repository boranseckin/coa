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
void server();
void handleConn(int sock);
void client(char *addr, char *filename);

#endif /* common_h */

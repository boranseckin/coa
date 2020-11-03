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

void error(char *msg);
void server();
void handleConn(int sock);
void client(char *addr);

#endif /* common_h */

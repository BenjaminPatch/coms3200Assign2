#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "serverData.h"

void serverSetup(int* udpSock, struct sockaddr_in* toServerAddr,
        struct ServerData* ServerData);

#endif

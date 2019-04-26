#ifndef SERVERDATA_H
#define SERVERDATA_H

#include <inttypes.h>

struct ServerData {
    int* udpSock;
    uint16_t* seqNum;
    struct sockaddr_in* clientAddr;
    struct sockaddr_in* serverAddr;

};

#endif

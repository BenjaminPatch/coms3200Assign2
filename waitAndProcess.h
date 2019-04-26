#ifndef WAITANDPROCESS_H
#define WAITANDPROCESS_H

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "serverData.h"

#define MAX_PACK_LEN 1500
#define MAX_HEAD_AND_PAY 1472
#define MAX_PAY 1466

#define GET 32
#define DAT 16
#define NAK 64
#define ACK 128
#define FIN 8
#define DAT_ACK DAT + ACK

struct PacketData {
    uint8_t seqNum1;
    uint8_t seqNum2;
    uint8_t ackNum1;
    uint8_t ackNum2;
    uint8_t flags;
    uint8_t reserved;
    char payload[MAX_PAY];
};

struct OutPack {
    uint16_t seqNum;
    uint16_t ackNum;
    uint8_t flags;
    uint8_t reserved;
    char payload[MAX_PAY];
};

void waitAndProcess(int* udpSock, struct ServerData* serverData);

void processGet(struct PacketData* packData, struct ServerData* serverData);

void closeConnection(struct ServerData* serverData);

void setSocketTimeout(int* udpSock);


char* constructPayload(FILE* fileToTransmit);

#endif

#include "waitAndProcess.h"

void waitAndProcess(int* udpSock, struct ServerData* serverData) {
    socklen_t sockLen = sizeof(*(serverData->clientAddr));
    //char* buffer = (char*)malloc(sizeof(char) * MAX_HEAD_AND_PAY);
    //memset(buffer, 0, MAX_HEAD_AND_PAY);
    struct PacketData* packData = 
        (struct PacketData*)malloc(sizeof(struct PacketData));
    memset(packData, 0, sizeof(struct PacketData));

    while (1) {
        int numBytes;
        printf("recv'ing\n");
        fflush(stdout);
        if ((numBytes = recvfrom(*udpSock, packData, MAX_HEAD_AND_PAY, 0,
                (struct sockaddr *)serverData->clientAddr, &sockLen)) == -1) {
            fprintf(stderr, "recvfrom\n");
        }
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", 
                inet_ntoa(serverData->clientAddr->sin_addr), 
                ntohs(serverData->clientAddr->sin_port));

        //for(size_t i = 0; i < MAX_HEAD_AND_PAY; i++) {
                ////printf("%02x ", buffer[i]);
                //if (buffer[i] == 0) {
                    //printf("0 ");
                //} else { 
                    //printf("%hhu ", buffer[i]);
                //}
                //count++;
        //}
        //memcpy(packData, buffer, MAX_HEAD_AND_PAY);
        uint16_t seqNum = ((uint16_t)packData->seqNum1 << 8) | 
                packData->seqNum2;
        printf("seqNum: %hu\n", seqNum);
        //uint16_t ackNum = ((uint16_t)packData->ackNum1 << 8) | 
        //        packData->ackNum2;
        if (packData->flags == GET) {
            processGet(packData, serverData);
        }

        closeConnection(serverData);
        //uint16_t wahu = packData->seqNum1;
        /*
        printf("%d", (buffer[0] & 32768) >> 15);
        printf("%d", (buffer[0] & 16384) >> 14);
        printf("%d", (buffer[0] & 8192) >> 13);
        printf("%d", (buffer[0] & 4096) >> 12);
        printf("%d", (buffer[0] & 2048) >> 11);
        printf("%d", (buffer[0] & 1024) >> 10);
        printf("%d", (buffer[0] & 512) >> 9);
        printf("%d\n", (buffer[0] & 256) >> 8);

        printf("%d", (wahu & 32768) >> 15);
        printf("%d", (wahu & 16384) >> 14);
        printf("%d", (wahu & 8192) >> 13);
        printf("%d", (wahu & 4096) >> 12);
        printf("%d", (wahu & 2048) >> 11);
        printf("%d", (wahu & 1024) >> 10);
        printf("%d", (wahu & 512) >> 9);
        printf("%d", (wahu & 256) >> 8);
        printf("%d", (wahu & 128) >> 7);
        printf("%d", (wahu & 64) >> 6);
        printf("%d", (wahu & 32) >> 5);
        printf("%d", (wahu & 16) >> 4);
        printf("%d", (wahu & 8) >> 3);
        printf("%d", (wahu & 4) >> 2);
        printf("%d", (wahu & 2) >> 1);
        printf("%d\n", (wahu & 1) >> 0);

        printf("seqnum1: %d\n", packData->seqNum1);
        printf("seqnum2: %d\n", packData->seqNum2);
        printf("ackNum: %d\n", ackNum);
        printf("payload: %s\n", packData->payload);
        printf("\ncount: %d\n", count);
        printf("\nnumBytes = %d\n", numBytes);
        printf("flag: %d\n", packData->flags);
        */

    }

}


void closeConnection(struct ServerData* serverData) {
    int sock = *(serverData->udpSock);
    socklen_t sockLen = sizeof(*(serverData->clientAddr));

    struct PacketData* outPack = 
            (struct PacketData*)malloc(sizeof(struct PacketData));
    memset(outPack, 0, sizeof(struct OutPack));

    struct PacketData* packData = 
        (struct PacketData*)malloc(sizeof(struct PacketData));
    memset(packData, 0, sizeof(struct PacketData));

    outPack->seqNum2 = *(serverData->seqNum);
    outPack->seqNum1 = *(serverData->seqNum) >> 8;
    outPack->flags = FIN;

    if (sendto(sock, outPack, MAX_HEAD_AND_PAY, 0, 
            (struct sockaddr*)serverData->clientAddr, sockLen) == -1) {
        fprintf(stderr, "sendto\n");
    } else {
        serverData->seqNum++;
    }
    int numBytes;
    printf("recv'ing in closeConnection\n");
    fflush(stdout);
    if ((numBytes = recvfrom(sock, packData, MAX_HEAD_AND_PAY, 0,
            (struct sockaddr *)serverData->clientAddr, &sockLen)) == -1) {
        fprintf(stderr, "recvfrom\n");
    }
    // todo: check this is a valid FIN/ACK
    memset(outPack, 0, sizeof(struct PacketData));
    outPack->seqNum2 = *(serverData->seqNum);
    outPack->seqNum1 = *(serverData->seqNum) >> 8;
    outPack->flags = FIN + ACK;
    if (sendto(sock, outPack, MAX_HEAD_AND_PAY, 0, 
            (struct sockaddr*)serverData->clientAddr, sockLen) == -1) {
        fprintf(stderr, "sendto\n");
    } else {
        serverData->seqNum++;
    }
    if (close(sock) == -1) {
        exit(1);
    }
    exit(0);
}

void processGet(struct PacketData* packData, struct ServerData* serverData) {
    printf("in get\n");
    setSocketTimeout(serverData->udpSock);
    FILE* fileToTransmit;
    fileToTransmit = fopen(packData->payload, "r");
    int numBytes;
    socklen_t sockLen = sizeof(*(serverData->clientAddr));

    while (!feof(fileToTransmit)) {

        struct PacketData* outPack = 
                (struct PacketData*)malloc(sizeof(struct PacketData));
        memset(outPack, 0, sizeof(struct OutPack));
        strcpy(outPack->payload, constructPayload(fileToTransmit));
        outPack->seqNum2 = *(serverData->seqNum);
        outPack->seqNum1 = *(serverData->seqNum) >> 8;
        outPack->flags = DAT;
        int sock = *(serverData->udpSock);

        printf("1: %hu\n", *(serverData->seqNum));
        if (sendto(sock, outPack, MAX_HEAD_AND_PAY, 0, 
                (struct sockaddr*)serverData->clientAddr, 
                sockLen) == -1) {
            fprintf(stderr, "sendto\n");
        } else {
            *(serverData->seqNum) = *(serverData->seqNum) + 1;
        }
        while (1) {
            memset(packData, 0, sizeof(struct PacketData));
            numBytes = 0;
            if ((numBytes = recvfrom(sock, packData, MAX_HEAD_AND_PAY, 0,
                    (struct sockaddr *)serverData->clientAddr, &sockLen)) == -1) {
                printf("2: %hu\n", *(serverData->seqNum));
                if (sendto(sock, outPack, MAX_HEAD_AND_PAY, 0, 
                        (struct sockaddr*)serverData->clientAddr, 
                        sockLen) == -1) {
                    fprintf(stderr, "sendto\n");
                }
            } else {
                uint16_t ackNum = ((uint16_t)packData->ackNum1 << 8) | 
                    packData->ackNum2;
                if (ackNum + 1 == *(serverData->seqNum)) {
                    break;
                }
            }
        }

        free(outPack);
    }
}


void setSocketTimeout(int* udpSock) {
    struct timeval timeVal;
    timeVal.tv_sec = 3;
    if (setsockopt(*udpSock, SOL_SOCKET, SO_RCVTIMEO,
                (struct timeval*)&timeVal, sizeof(struct timeval))) {
        perror("setsockopt");
    }

}


char* constructPayload(FILE* fileToTransmit) {
    int index = 0;
    int c;
    char* payload = (char*)malloc(sizeof(char) * MAX_PAY);
    memset(payload, 0, MAX_PAY);
    while (!feof(fileToTransmit) && index < 1466) {
        c = fgetc(fileToTransmit);
        payload[index] = c;
        index++;
    }
    return payload;
}

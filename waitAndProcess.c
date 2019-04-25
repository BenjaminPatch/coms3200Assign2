#include "waitAndProcess.h"

void waitAndProcess(int* udpSock, struct ServerData* serverData) {
    socklen_t sockLen = sizeof(*(serverData->clientAddr));
    char* buffer = (char*)malloc(sizeof(char) * MAX_HEAD_AND_PAY);
    memset(buffer, 0, MAX_HEAD_AND_PAY);
    struct PacketData* packData = 
        (struct PacketData*)malloc(sizeof(struct PacketData));
    memset(packData, 0, sizeof(struct PacketData));

    while (1) {
        int numBytes;
        printf("recv'ing\n");
        fflush(stdout);
        if ((numBytes = recvfrom(*udpSock, buffer, MAX_HEAD_AND_PAY, 0,
                (struct sockaddr *)serverData->clientAddr, &sockLen)) == -1) {
            fprintf(stderr, "recvfrom\n");
        }
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", 
                inet_ntoa(serverData->clientAddr->sin_addr), 
                ntohs(serverData->clientAddr->sin_port));
        buffer[1500] = 0;
        printf("Data: %s\n" , buffer);

        if (sendto(*udpSock, buffer, numBytes, 0, 
                (struct sockaddr*)serverData->clientAddr, sockLen) == -1) {
            fprintf(stderr, "sendto\n");
        } else {
            printf("yeeeeeehaw\n");
        }
        

        //for(size_t i = 0; i < MAX_HEAD_AND_PAY; i++) {
                ////printf("%02x ", buffer[i]);
                //if (buffer[i] == 0) {
                    //printf("0 ");
                //} else { 
                    //printf("%hhu ", buffer[i]);
                //}
                //count++;
        //}
        memcpy(packData, buffer, MAX_HEAD_AND_PAY);
        uint16_t seqNum = ((uint16_t)packData->seqNum1 << 8) | 
                packData->seqNum2;
        uint16_t ackNum = ((uint16_t)packData->ackNum1 << 8) | 
                packData->ackNum2;
        if (packData->flags == GET) {
            processGet(packData, seqNum, ackNum, serverData);
        }
        printf("seqNum: %d\n", seqNum);
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

void processGet(struct PacketData* packData, int seqNum, int ackNum,
        struct ServerData* serverData) {
    printf("in get\n");
    int sequence = 1001;
    int sequence2 = sequence >> 8;
    while (1) {

        struct PacketData* outPack = 
                (struct PacketData*)malloc(sizeof(struct PacketData));
        memset(outPack, 0, sizeof(struct OutPack));
        outPack->seqNum2 = sequence;
        outPack->seqNum1 = sequence2;
        printf("2: %d\n", outPack->seqNum2);
        printf("1: %d\n", outPack->seqNum1);
        int sockLen = sizeof(*(serverData->clientAddr));
        int sock = *(serverData->udpSock);

        if (sendto(sock, outPack, MAX_HEAD_AND_PAY, 0, 
                (struct sockaddr*)serverData->clientAddr, sockLen) == -1) {
            fprintf(stderr, "sendto\n");
        }
        break;
    }
}

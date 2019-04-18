#include "waitAndProcess.h"

void waitAndProcess(int* udpSock, struct sockaddr_in* toClientAddr) {
    struct sockaddr_in clientAddr = *toClientAddr;
    socklen_t sockLen = sizeof(clientAddr);
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
                (struct sockaddr *)&clientAddr, &sockLen)) == -1) {
            fprintf(stderr, "recvfrom\n");
        }
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(clientAddr.sin_addr), 
                ntohs(clientAddr.sin_port));
        buffer[1500] = 0;
        printf("Data: %s\n" , buffer);

        //if (sendto(*udpSock, buffer, numBytes, 0, 
                //(struct sockaddr*)&clientAddr, sockLen) == -1) {
            //fprintf(stderr, "sendto\n");
        //}
        
        int count = 0;

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
            processGet(packData, seqNum, ackNum);
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
        */
        printf("seqnum1: %d\n", packData->seqNum1);
        printf("seqnum2: %d\n", packData->seqNum2);
        printf("ackNum: %d\n", ackNum);
        printf("payload: %s\n", packData->payload);
        printf("\ncount: %d\n", count);
        printf("\nnumBytes = %d\n", numBytes);
        printf("flag: %d\n", packData->flags);

        FILE *f = fopen("wahu.txt", "w");
        fprintf(f, "%s", buffer);
        break;
    }

}

processGet(packData, seqNum, ackNum);

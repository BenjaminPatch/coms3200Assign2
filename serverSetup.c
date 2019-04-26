#include "serverSetup.h"

void serverSetup(int* udpSock, struct sockaddr_in* toServerAddr,
        struct ServerData* serverData) {
    struct sockaddr_in serverAddr = *toServerAddr;
    serverData->serverAddr = toServerAddr;

    if ((*udpSock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        // todo
        fprintf(stderr, "sock\n");
    }
    serverData->udpSock = udpSock;

    memset((char*)&serverAddr, 0, sizeof(serverAddr));
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(0);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(*udpSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr))
            == -1) {
        fprintf(stderr, "bind\n");
    }

    socklen_t size = sizeof(serverAddr);

    if (getsockname(*udpSock, (struct sockaddr *)&serverAddr, 
               &size)  == -1) {
        perror("getsockname");
    } else {
        printf("%d\n", ntohs(serverAddr.sin_port));
        fflush(stdout);
    }
}

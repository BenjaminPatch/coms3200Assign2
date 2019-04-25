#include "main.h"

int main(int argc, char** argv) {
    int udpSock;
    struct ServerData* serverData = 
        (struct ServerData*)malloc(sizeof(struct ServerData));
    memset(serverData, 0, sizeof(struct ServerData));
    struct sockaddr_in serverAddr, clientAddr;
    
    serverData->clientAddr = &clientAddr;

    serverSetup(&udpSock, &serverAddr, serverData);
    waitAndProcess(&udpSock, serverData);

}

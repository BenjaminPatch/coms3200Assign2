#include "main.h"

int main(int argc, char** argv) {
    int udpSock;
    struct sockaddr_in serverAddr, clientAddr;
    serverSetup(&udpSock, &serverAddr);
    waitAndProcess(&udpSock, &clientAddr);

}

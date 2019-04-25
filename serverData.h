#ifndef SERVERDATA_H
#define SERVERDATA_H

struct ServerData {
    int* udpSock;
    struct sockaddr_in* clientAddr;
    struct sockaddr_in* serverAddr;

};

#endif

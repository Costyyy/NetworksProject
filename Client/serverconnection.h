#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
class serverConnection
{
public:
    unsigned short port;
    int sd; //socket descriptor
    struct sockaddr_in server; //struct for the server
    void connectTo(const char[]);
    serverConnection();
};

#endif // SERVERCONNECTION_H

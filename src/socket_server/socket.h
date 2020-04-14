#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <stdio.h>
#include <unistd.h>

#define FLAG_DEFAULT 0

using namespace std;

class Socket
{
    private:
        int sock_fd;
        int port;
        char *ip_address;

    public:
        Socket(int sock_fd, char *ip_address, int port);
        ~Socket();

        int sendd(const char *msg);
        int recvv(char *msg, int lenght);

        int getSock_fd();
        char* get_IPAddress();
        int getPort();

};

#endif
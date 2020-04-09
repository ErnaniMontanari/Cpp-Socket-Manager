#ifndef SERVER_H
#define SERVER_H

#include "socket.h"

class Server : public Socket
{
    private:
        char *name;

    public:
        Server(int sock_fd, char* ip_address, int port, char *name);

        char* getName();

};

#endif
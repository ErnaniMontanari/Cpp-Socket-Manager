#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"

class Client : public Socket
{
    private:
        char *name;

    public:
        Client(int sock_fd, char* ip_address, int port, char *name);

        char* getName();

};

#endif
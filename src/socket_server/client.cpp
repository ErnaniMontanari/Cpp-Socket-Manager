#include "client.h"

Client::Client(int sock_fd, char* ip_address, int port, char *name) : Socket(sock_fd, ip_address, port)
{
    this->name = name;
}

char* Client::getName()
{
    return name;
}
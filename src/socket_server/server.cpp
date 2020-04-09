#include "server.h"

Server::Server(int sock_fd, char* ip_address, int port, char *name) : Socket(sock_fd, ip_address, port)
{
    this->name = name;
}

char* Server::getName()
{
    return name;
}
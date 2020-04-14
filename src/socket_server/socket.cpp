#include "socket.h"

Socket::Socket(int sock_fd, char *ip_address, int pt)
{
    this->sock_fd = sock_fd;
    this->ip_address = ip_address;
    this->port = pt;
}

Socket::~Socket()
{
    close(sock_fd);
}

int Socket::sendd(const char *msg)
{
    return send(sock_fd, msg, strlen(msg), FLAG_DEFAULT);
}

int Socket::recvv(char *msg, int lenght)
{
    return recv(sock_fd, msg, lenght, FLAG_DEFAULT);
}

int Socket::getSock_fd()
{
    return this->sock_fd;
}
char* Socket::get_IPAddress()
{
    return this->ip_address;
}
int Socket::getPort()
{
    return this->port;
}

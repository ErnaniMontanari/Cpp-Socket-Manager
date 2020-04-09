#include "socket.h"

Socket::Socket(int sock_fd, char *ip_address, int port)
{
    this->sock_fd = sock_fd;
    this->ip_address = ip_address;
    this->port = port;
}

Socket::~Socket()
{
    close(sock_fd);
}

int Socket::sendd(const char *msg)
{
    return send(sock_fd, msg, strlen(msg), FLAG_DEFAULT);
}

int Socket::recvv(char *msg)
{
    return recv(sock_fd, msg, strlen(msg), FLAG_DEFAULT);
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

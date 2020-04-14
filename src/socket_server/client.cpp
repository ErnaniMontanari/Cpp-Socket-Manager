#include "client.h"

Client::Client(int fd, char* ip, int pt, char *nm) : Socket(fd, ip, pt)
{
    this->name = nm;
}

char* Client::getName()
{cout << name << endl;
    return this->name;
}
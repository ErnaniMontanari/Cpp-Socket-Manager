#ifndef SOCKMANAGER_H
#define SOCKMANAGER_H

#include <unistd.h> 
#include <stdio.h> 
#include <iostream>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#include "server.h"

#define SOCK_FD_ERROR "Creating socket file descriptor falied"
#define BINDING_ERROR "Socket bind falied"
#define RECV_ERROR "Socket recv falied"
#define LISTENING_ERROR "Socket listen falied"
#define LISTENER_ERROR "Socket can't listener"
#define SOCK_CREATED "Socket is created, can't create"
#define LISTEN_BACKLOG 50
#define SOCK_BUFF_NAME 1024

#define CREATE_SERVER 1         //usa quando for criar um servidor // usa quando for servidor
#define CONNECT_TO_SERVER 2     //usando quando for se conectar a um servidor // usa quando for cliente


class SockManager
{
    public:
        //define as variaveis
        SockManager(int domain, int type, int protocol, int port, int task);

        //encerra o socket
        ~SockManager();

        //inicia o socket e comeca a receber conexoes
        int create(Socket *sock);

        int startListener(int maxclients, Client* clientlist[]);

        int waitClient(Client* client);

        // para de receber connexoes ao socket
        int stop();
        bool isListen();

    private:
        int domain;
        int type;
        int protocol;
        int port;
        int task;

        bool listener;
        bool sock_created;

        int socketFD; // file descriptor

        struct sockaddr_in address;

        int createServer();
        int connectToServer(Socket *sock);
};

#endif
#include "sockmanager.h"


SockManager::SockManager(int domain, int type, int protocol, int port, int task)
{
    this->domain = domain;
    this->type = type;
    this->protocol = protocol;
    this->port = port;
    this->task = task;
    listener = false;
    sock_created = false;
}

SockManager::~SockManager()
{
    close(socketFD);
}

int SockManager::create(Socket *sock)
{
    //verefica se ja foi criado com sucesso
    if(sock_created)
    {
        perror(SOCK_CREATED);
        return -1;
    }

    //cria o socket
    socketFD = socket(domain, type, protocol); 
    if(!(socketFD > 0))
    {
        perror(SOCK_FD_ERROR);
        return socketFD;
    }

    //define os valores da estrutura sockaddr_in
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(port);
    
    //cria um servidor que pode receber conexoes
    //ou
    //conecta a um servidor existente
    if(task == CREATE_SERVER)
    {
        int var = createServer();
        if(var < 0)
        {
            return var;
        }
    }
    else if(task == CONNECT_TO_SERVER)
    {
        int var = connectToServer(sock);
        if(var < 0)
        {
            return var;
        }
    }

    sock_created = true;
    return 0;
}

int SockManager::startListener(int maxclients, Client* clientlist[])
{
    if(task == CONNECT_TO_SERVER)
    {
        perror(LISTENER_ERROR);
        return -1;
    }

    int sockclient;
    listener = true;
    while (listener)
    {
        sockclient = accept(socketFD, (struct sockaddr *) &address, (socklen_t*)&address);

        for(int i = 0; i < maxclients; i++)
        {
            if(clientlist[i] == 0x00000)
            {
                clientlist[i] = new Client(sockclient, "sem ip", 54321, "sem nome");
                if(i == (maxclients -1))
                {
                    listener = false;
                }
                break;
            }
        }
    }
    cout << "cabou o listener" << endl;
    return 0;
}

int SockManager::waitClient(Client* client)
{
    int sockclient = accept(socketFD, (struct sockaddr *) &address, (socklen_t*)&address);
    if(sockclient < 0)
    {
        cout << "ERRO AO ACEITAR CLIENTE" << endl;
        return sockclient;
    }

    client = new Client(sockclient, "sem ip", 54321, "sem nome");
    return 0;
}

int SockManager::stop()
{
    //TODO: stop socket, but not close.
    listener = false;
    return 0;
}

bool SockManager::isListen()
{
    return listener;
}

int SockManager::createServer()
{
    //define um para o enderesso do socket
    int binding = bind(socketFD, (struct sockaddr *) &address, sizeof(address));
    if(binding < 0) 
    {
        perror(BINDING_ERROR);
        return binding;
    }

    //define o socket para receber as conexoes pela funcao accept
    int listening = listen(socketFD, LISTEN_BACKLOG);
    if(listening < 0)
    {
        perror(LISTENING_ERROR);
        return listening;
    }

    return 0;
}

int SockManager::connectToServer(Socket *sock)
{
    //conecta-se ao servidor
    int connected = connect(socketFD, (const sockaddr *) &address, sizeof(address));
    if(connected < 0)
    {
        return connected;
    }

    //pega o nome do primeiro request
    char sv_name[SOCK_BUFF_NAME];
    int recv_name = recv(socketFD, sv_name, SOCK_BUFF_NAME, 0);
    if(recv_name < 0)
    {
        perror(RECV_ERROR);
        return recv_name;
    }

    //define o ponteiro como uma instancia do servidor conectado
    sock = new Server(socketFD, inet_ntoa(address.sin_addr), (int) ntohs(address.sin_port), sv_name);

    return 0;
}
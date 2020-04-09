#include "sockmanager.h"


SockManager::SockManager(int domain, int type, int protocol, unsigned int ip, int port, int task, char *myname)
{
    this->domain = domain;
    this->type = type;
    this->protocol = protocol;
    this->ip = ip;
    this->port = port;
    this->task = task;
    this->myname = myname;
    listener = false;
    sock_created = false;

    //define os valores da estrutura sockaddr_in
    address.sin_family = AF_INET; 
    address.sin_port = htons(port);
    if(ip > 0)
    {
        address.sin_addr.s_addr = ip; 
    }
    else
    {
        address.sin_addr.s_addr = INADDR_ANY; 
    }
}

SockManager::~SockManager()
{
    close(socketFD);
}

int SockManager::create(Server *sock) //if create a server passe parametter nullptr
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

    char nameclient[SOCK_BUFF_NAME]; //client name returned from the client
    int sockclient; //client file descriptor

    listener = true;
    while (listener)
    {
        sockclient = acceptClient(nameclient);
        if(sockclient < 0)
        {
            continue;
        }

        for(int i = 0; i < maxclients; i++)
        {
            if(clientlist[i] == 0x00000)
            {
                clientlist[i] = new Client(sockclient, inet_ntoa(address.sin_addr), (int) ntohs(address.sin_port), nameclient);
                if(i == (maxclients -1))
                {
                    listener = false;
                }
                break;
            }
        }
    }

    return 0;
}

int SockManager::waitClient(Client* client)
{
    if(task == CONNECT_TO_SERVER)
    {
        perror(WAITCLIENT_ERROR);
        return -1;
    }

    char nameclient[SOCK_BUFF_NAME];
    int sockclient = acceptClient(nameclient);
    if(sockclient < 0)
    {
        return sockclient;
    }

    client = new Client(sockclient, inet_ntoa(address.sin_addr), (int) ntohs(address.sin_port), nameclient);
    return 0;
}

int SockManager::acceptClient(char* clientname)
{
    int sockclient = accept(socketFD, (struct sockaddr *) &address, (socklen_t*)&address);
    if(sockclient < 0)
    {
        cout << "ERRO AO ACEITAR CLIENTE" << endl;
        return sockclient;
    }

    //envia o nome do server
    int name = send(sockclient, this->myname, SOCK_BUFF_NAME, 0);
    if(name < 0)
    {
        perror(SEND_ERROR);
        return name;
    }

    //recebe o nome do client
    name = recv(sockclient, clientname, SOCK_BUFF_NAME, 0);
    if(name < 0)
    {
        perror(RECV_ERROR);
        return name;
    }

    return sockclient;
}

int SockManager::connectToServer(Server *sock)
{
    //conecta-se ao servidor
    int connected = connect(socketFD, (const sockaddr *) &address, sizeof(address));
    if(connected < 0)
    {
        return connected;
    }

    //recebe o nome do server
    char sv_name[SOCK_BUFF_NAME];
    int recv_name = recv(socketFD, sv_name, SOCK_BUFF_NAME, 0);
    if(recv_name < 0)
    {
        perror(RECV_ERROR);
        return recv_name;
    }

    //envia o nome do client
    recv_name = send(socketFD, this->myname, SOCK_BUFF_NAME, 0);
    if(recv_name < 0)
    {
        perror(RECV_ERROR);
        return recv_name;
    }

    //define o ponteiro como uma instancia do servidor conectado
    sock = new Server(socketFD, inet_ntoa(address.sin_addr), (int) ntohs(address.sin_port), sv_name);

    return 0;
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
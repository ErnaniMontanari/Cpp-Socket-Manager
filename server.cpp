#include <iostream>
#include <string.h>
#include <thread>
#include "src/socket_server/sockmanager.h"

using namespace std;

Client *client;

int run();

int main() {
	cout << "Server Start..." << endl;

	SockManager socketmanager{AF_INET, SOCK_STREAM, 0, INADDR_ANY, 54321, CREATE_SERVER, "Server Montanari"};

	int status = socketmanager.create(nullptr); //when create a server, use nullptr

	if(status == 0)
	{
		cout << "Server create successful" << endl;
	}
	else
	{
		cout << "Server don't create successful" << endl;
	}
	
	if(socketmanager.waitClient(&client) < 0) //wait client connection
	{
		return -1;
	}

	cout << "Client Information" << endl;
	cout << "Client name: " << client->getName() << endl;
	cout << "Client ip: " << client->get_IPAddress() << endl;
	cout << "Client port: " << client->getPort() << endl;
	cout << "Client file descripor num: " << client->getSock_fd() << endl;

	thread runnable(run);

	char msg[1024];
	while (true)
	{
		cin.getline(msg, 1024);
		if(!strcmp(msg, "exit"))
		{
			cout << "Close server" << endl;
			break;
		}

		if(client->sendd(msg) < 0)
		{
			cout << "Error to send" << endl;
		}
		cout << "Server: " << msg << endl;

	}

}

int run()
{
	char ms[1024];
	while (true)
	{
		memset(ms, 0, 1024);
		if(client->recvv(ms, 1024) < 0)
		{
			cout << "Error to recv" << endl;
		}
		cout << "Client: " << ms << endl;
	}
	
	return 0;
}
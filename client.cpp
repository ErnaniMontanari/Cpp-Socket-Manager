#include <iostream>
#include <string.h>
#include <thread>
#include "src/socket_server/sockmanager.h"

using namespace std;

Server *sv;

int run();

int main() {
	cout << "Client Start..." << endl;

	SockManager socketmanager{AF_INET, SOCK_STREAM, 0, INADDR_ANY, 54321, CONNECT_TO_SERVER, "Cliente Ernani"};

	int status = socketmanager.create(&sv);

	if(status == 0)
	{
		cout << "Connection successful" << endl;
	}
	else
	{
		cout << "Connection falied" << endl;
	}
	

	cout << "Server Information" << endl;
	cout << "Server name: " << sv->getName() << endl;
	cout << "Server ip: " << sv->get_IPAddress() << endl;
	cout << "Server port: " << sv->getPort() << endl;
	cout << "Server file descripor num: " << sv->getSock_fd() << endl;

	thread runnable(run);

	char msg[1024];
	while (true)
	{
		cin.getline(msg, 1024);
		if(!strcmp(msg, "exit"))
		{
			cout << "Close client" << endl;
			break;
		}

		if(sv->sendd(msg) < 0)
		{
			cout << "Error to send" << endl;
		}
		cout << "Client: " << msg << endl;

	}
	
	return 0;
}

int run()
{
	char ms[1024];
	while (true)
	{
		memset(ms, 0, 1024);
		if(sv->recvv(ms, 1024) < 0)
		{
			cout << "Error to recv" << endl;
		}
		cout << "Server: " << ms << endl;

		if(!strcmp(ms, "exit_client"))
		{
			cout << "Close client from server." << endl;
			break;
		}
	}
	
	return 0;
}
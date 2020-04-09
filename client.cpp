#include <iostream>
#include "src/socket_server/sockmanager.h"

using namespace std;

int main() {
	cout << "Client Start..." << endl;

	Server *sv;
	char msg[1024];
	SockManager socketmanager{AF_INET, SOCK_STREAM, 0, INADDR_ANY, 54321, CONNECT_TO_SERVER, "Cliente Ernani"};

	int status = socketmanager.create(sv);

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

	string ms1;
	getline(cin, ms1);
	sv->recvv(msg);
	sv->sendd("msg");

	
 //TODO make




}
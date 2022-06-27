// Sever Code
#include "TCPServer.h"

using namespace GTPNet;


int main()
{	
	TCPServer tcp_server;


	if (tcp_server.Initialize(IPEndpoint("127.0.0.1", 4790)))
		while (true)
			tcp_server.Frame();

	Network::Shutdown();

	return 0;
}
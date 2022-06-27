// Client Code
#include <GTPNet/IncludeMe.h>
#include "TCPClient.h"
#include <iostream>

using namespace GTPNet;


int main()
{

	TCPClient tcp_client;


	if (tcp_client.Connect(IPEndpoint("127.0.0.1", 4790)))
		while (tcp_client.IsConnected())
			tcp_client.Frame();

	Network::Shutdown();

	return 0;
}
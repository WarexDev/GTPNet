#include "Network.h"
#include <iostream>

bool GTPNet::Network::Initialize()
{
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0)
	{
		std::cerr << "Context : GTPNet::Network::Initialize() -> Failed To Start up the winsock API." << std::endl;
		return false;
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		std::cerr << "Context : GTPNet::Network::Initialize() -> Could not find a usable version of the winsock API DLL." << std::endl;
		return false;
	}

	return true;
}

void GTPNet::Network::Shutdown()
{
	WSACleanup();
}

#pragma once
#include <GTPNet/IncludeMe.h>
#include "Client.h"

using namespace GTPNet;

class TCPClient : public Client
{
public:
	TCPClient();


	bool Connect(IPEndpoint _Ep);
	bool IsConnected();
	bool Frame() override;

private:
	bool isConnected = false;
	TCPSocket socket;

};

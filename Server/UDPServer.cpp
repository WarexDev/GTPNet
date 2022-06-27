#include "UDPServer.h"

namespace GTPNet
{
	UDPServer::UDPServer()
	{

	}

	UDPServer::~UDPServer()
	{

	}

	bool UDPServer::Initialize(IPEndpoint _Ep)
	{
		if (Network::Initialize())
		{
			socket = UDPSocket();

			endPoint = &_Ep;

			if (endPoint != nullptr && (socket.Create(endPoint->GetIpVersion()) != PResult::P_GenericError))
			{
				if (socket.Bind(*endPoint) != PResult::P_GenericError)
				{
					LOGGER->Log("Failed to bind the UDPServer's socket to the current EndPoint.");
					return false;
				}
			}
			else
			{
				LOGGER->Log("Failed to create the socket on UDPServer Initialization.");
				return false;
			}
		}

		return true;
	}

	void UDPServer::Frame()
	{

	}
}


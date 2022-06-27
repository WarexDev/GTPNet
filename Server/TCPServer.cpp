#include "TCPServer.h"

namespace GTPNet
{
	TCPServer::TCPServer()
	{
	}

	TCPServer::~TCPServer()
	{
	}

	bool TCPServer::Initialize(IPEndpoint _Ep)
	{
		if (Network::Initialize())
		{
			LOGGER->Log("Winsock API as been successfully initialized");

			listeningSocket = TCPSocket();

			endPoint = &_Ep;

			if (listeningSocket.Create(endPoint->GetIpVersion()) == PResult::P_Success)
			{
				LOGGER->Log("Socket successfully created");

				if (listeningSocket.Listen(*endPoint) == PResult::P_Success)
				{
					LOGGER->Log("Socket successfully listening.");
					return true;
				}
				else
					LOGGER->Log("Failed to listen.");

				listeningSocket.Close();
			}
			else
				LOGGER->Log("Socket failed to create");
		}

		return false;
	}

	void TCPServer::Frame()
	{
		TCPSocket newConn;
		if (listeningSocket.Accept(newConn) == PResult::P_Success)
		{
			LOGGER->Log("New Connection Accepted");

			newConn.Close();
		}
		else
		{
			
		}
	}
}



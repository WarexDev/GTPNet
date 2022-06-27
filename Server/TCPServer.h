#pragma once
#include "Server.h"

namespace GTPNet
{

	class TCPServer : public Server
	{
	public:
		TCPServer();
		~TCPServer();

		bool Initialize(IPEndpoint _Ep);
		void Frame() override;

	private:
		TCPSocket listeningSocket;
		
	};
}

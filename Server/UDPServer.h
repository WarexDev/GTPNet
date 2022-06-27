#pragma once
#include "Server.h"

namespace GTPNet
{
	class UDPServer : public Server
	{
	public:
		UDPServer();
		~UDPServer();

		bool Initialize(IPEndpoint _Ep);
		void Frame() override;

	private:
		UDPSocket socket;
	};
}

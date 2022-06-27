#pragma once
#include <GTPNet/IncludeMe.h>

namespace GTPNet
{
	class Server
	{
	public:
		Server();

		virtual void Frame() = 0;

	protected:
		IPEndpoint* endPoint;
	};
}


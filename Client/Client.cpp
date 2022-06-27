#include "Client.h"
#include <typeinfo>

namespace GTPNet
{
	Client::Client() : endPoint(nullptr)
	{

	}

	Client::~Client()
	{

	}

	template<typename T>
	bool Client::AddPacketData(T _data)
	{

	}

}

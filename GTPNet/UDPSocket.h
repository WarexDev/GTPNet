#pragma once
#include "SocketBase.h"
#include <string>

namespace GTPNet
{
	class UDPSocket : public SocketBase
	{

	public:
		UDPSocket();
		virtual ~UDPSocket();


		PResult Create(IPVersion _ipVersion = IPVersion::IPv4);
		PResult Send(const void* DataToSend, int _nbBytesToSend, int& _bytesRec, const IPEndpoint& _ep);
		PResult SendAll(const void* DataToSend, int _nbBytesToSend, const IPEndpoint& _ep);
		PResult Send(Packet& _Packet, const IPEndpoint& _ep);
		PResult Recv(void* _buffer, int _nbBytesReceived, int& _bytesRec, const IPEndpoint& _ep);
		PResult RecvAll(void* _buffer, int _nbBytesToReceived, const IPEndpoint& _ep);
		PResult Recv(Packet& _Packet, const IPEndpoint& _ep);
	};
}

#pragma once
#include "SocketBase.h"
#include <string>

namespace GTPNet
{
	class TCPSocket : public SocketBase
	{

	public:
		TCPSocket();
		virtual ~TCPSocket();

		PResult Create(IPVersion _ipVersion = IPVersion::IPv4);
		PResult Listen(IPEndpoint _endpoint, int _backLog = 5); 
		PResult Accept(TCPSocket& _outSocket); 
		PResult Connect(IPEndpoint _endpoint); 
		PResult Send(const void* DataToSend, int _nbBytesToSend, int& _bytesRec);
		PResult SendAll(const void* DataToSend, int _nbBytesToSend);
		PResult Send(Packet& _Packet);
		PResult Recv(void* _buffer, int _nbBytesReceived, int& _bytesRec);
		PResult RecvAll(void* _buffer, int _nbBytesReceived);
		PResult Recv(Packet& _Packet);
	};
}


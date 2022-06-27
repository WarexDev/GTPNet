#include "TCPSocket.h"
#include <assert.h>
#include <iostream>
#include "Constants.h"

namespace GTPNet
{

	TCPSocket::TCPSocket()
	{

	}

	TCPSocket::~TCPSocket()
	{
	}

	/// <summary>Socket Creation and Setting SocketOptions | Should get WSAError if the creation failed</summary>
	/// 
	/// <returns>PResult</returns> 
	PResult TCPSocket::Create(IPVersion _ipVersion)
	{
		if (SocketBase::Create(_ipVersion, IPPROTO::IPPROTO_TCP) != PResult::P_Success)
			return PResult::P_GenericError;

		if (SetBlocking(false) != PResult::P_Success)
			return PResult::P_GenericError;

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != PResult::P_Success)
			return PResult::P_GenericError;

		return PResult::P_Success;
	}



#pragma region TCP_Functions
	PResult TCPSocket::Listen(IPEndpoint _endpoint, int _backLog)
	{

		if (ipVersion == IPVersion::IPv6)
			if (SetSocketOption(SocketOption::IPv6_Only, FALSE) == PResult::P_GenericError)
				return PResult::P_GenericError;

		if (Bind(_endpoint) != PResult::P_Success)
			return PResult::P_GenericError;

		int result = listen(handle, _backLog);

		if (result != 0)
			return PrintWSALastErrorCode("[Socket::Listen] - listen()");

		return PResult::P_Success;
	}

	PResult TCPSocket::Accept(TCPSocket& _outSocket)
	{
		sockaddr_in addr_v4 = {};
		sockaddr_in6 addr_v6 = {};
		bool isIpv4 = (ipVersion == IPVersion::IPv4);
		int len = isIpv4 ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
		sockaddr* acceptanceAddr = isIpv4 ? (sockaddr*)&addr_v4 : (sockaddr*)&addr_v6;
		int addrFamily = isIpv4 ? AF_INET : AF_INET6;

		SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)&addr_v4, &len);

		if (acceptedConnectionHandle == INVALID_SOCKET)
		{
			if(IsInBlocking()) PrintWSALastErrorCode("[TcpSocket::Accept - accept()]");
			return PResult::P_GenericError;
		}
		
		// Only debug purpose to know the EP
		IPEndpoint newConnEP(acceptanceAddr);
		newConnEP.Print();

		_outSocket.SetHandle(socket(addrFamily, SOCK_STREAM, IPPROTO::IPPROTO_TCP));

		if (_outSocket.GetHandle() == INVALID_SOCKET)
			return PrintWSALastErrorCode("[TcpSocket::Accept - new sock creation()]");

		return PResult::P_Success;
	}


	PResult TCPSocket::Connect(IPEndpoint _endpoint)
	{
		int result = 0;

		if (ipVersion == IPVersion::IPv4)
		{
			sockaddr_in addr = _endpoint.GetSockAddrIPv4();

			result = connect(handle, (sockaddr*)&(addr), sizeof(sockaddr_in));
		}
		else
		{
			sockaddr_in6 addr = _endpoint.GetSockAddrIPv6();

			result = connect(handle, (sockaddr*)&(addr), sizeof(sockaddr_in6));
		}

		if (result != 0)
			return PrintWSALastErrorCode("[Socket::Connect - connect()]");

		return PResult::P_Success;
	}
#pragma endregion

	PResult TCPSocket::Send(const void* _DataToSend, int _nbBytesToSend, int& _bytesSend)
	{
		_bytesSend = send(handle, (const char*)_DataToSend, _nbBytesToSend, NULL);

		if (_bytesSend == SOCKET_ERROR)
			return PrintWSALastErrorCode("[Socket::Send - send()]");

		return PResult::P_Success;
	}

	PResult TCPSocket::SendAll(const void* DataToSend, int _nbBytesToSend)
	{
		int totalBytesSended = 0;

		while (totalBytesSended < _nbBytesToSend)
		{
			int nbBytesRemaining = _nbBytesToSend - totalBytesSended;
			int bytesSended = 0;
			char* bufferOffset = (char*)(DataToSend)+totalBytesSended;

			if (Send(bufferOffset, nbBytesRemaining, bytesSended) == PResult::P_GenericError)
				return PResult::P_GenericError;

			totalBytesSended += bytesSended;
		}

		return PResult::P_Success;
	}

	PResult TCPSocket::Send(Packet& _Packet)
	{
		uint16_t encodedSize = htons(static_cast<u_short>(_Packet.buffer.size()));

		if (SendAll(&encodedSize, sizeof(uint16_t)) != PResult::P_Success)
			return PResult::P_GenericError;

		if (SendAll(_Packet.buffer.data(), static_cast<int>(_Packet.buffer.size())) != PResult::P_Success)
			return PResult::P_GenericError;

		return PResult::P_Success;
	}

	PResult TCPSocket::RecvAll(void* _buffer, int _nbBytesToReceived)
	{
		int totalBytesReceived = 0;

		while (totalBytesReceived < _nbBytesToReceived)
		{
			int nbBytesRemaining = _nbBytesToReceived - totalBytesReceived;
			int bytesReceived = 0;

			char* bufferOffset = (char*)(_buffer)+totalBytesReceived;

			if (Recv(bufferOffset, nbBytesRemaining, bytesReceived) == PResult::P_GenericError)
				return PResult::P_GenericError;

			totalBytesReceived += bytesReceived;
		}

		return PResult::P_Success;
	}

	PResult TCPSocket::Recv(Packet& _Packet)
	{
		_Packet.Clear();

		uint16_t encodedSize = 0;

		if (RecvAll(&encodedSize, sizeof(uint16_t)) != PResult::P_Success)
			return PResult::P_GenericError;

		uint16_t bufferSize = ntohs(encodedSize);

		if (bufferSize > g_MaxPacketSize)
			return PResult::P_GenericError;

		_Packet.buffer.resize(bufferSize);

		if (RecvAll(&_Packet.buffer[0], bufferSize) != PResult::P_Success)
			return PResult::P_GenericError;

		return PResult::P_Success;

	}

	PResult TCPSocket::Recv(void* _buffer, int _nbBytesReceived, int& _bytesRec)
	{

		_bytesRec = recv(handle, (char*)_buffer, _nbBytesReceived, NULL);

		if (_bytesRec == 0) 
			return PResult::P_GenericError;

		if (_bytesRec == SOCKET_ERROR)
			return PrintWSALastErrorCode("[Socket::Recv -> recv() : recvfrom()]");

		return PResult::P_Success;
	}

}



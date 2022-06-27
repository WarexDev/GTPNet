#include "UDPSocket.h"
#include "Constants.h"

namespace GTPNet
{
	UDPSocket::UDPSocket()
	{
	}

	UDPSocket::~UDPSocket()
	{
	}

	PResult UDPSocket::Create(IPVersion _ipVersion)
	{
		if (SocketBase::Create(_ipVersion, IPPROTO::IPPROTO_UDP) != PResult::P_Success)
			return PResult::P_GenericError;

		// fill option and blocking state here


		return PResult::P_Success;
	}

	PResult UDPSocket::Send(const void*_DataToSend, int _nbBytesToSend, int& _bytesRec, const IPEndpoint& _ep)
	{
		_bytesRec = sendto(handle, (const char*)_DataToSend, _nbBytesToSend, NULL, _ep.GetUsingSockAddr(), sizeof(struct sockaddr*));
			
		if(_bytesRec == SOCKET_ERROR)
			return PrintWSALastErrorCode("UDPSocket::Send() -> sendto()");

		return PResult::P_Success;
	}

	PResult UDPSocket::SendAll(const void* DataToSend, int _nbBytesToSend, const IPEndpoint& _ep)
	{
		int totalBytesSended = 0;

		while (totalBytesSended < _nbBytesToSend)
		{
			int nbByteRemaining = _nbBytesToSend - totalBytesSended;
			int bytesSended = 0;
			char* bufferOffset = (char*)(DataToSend)+totalBytesSended;

			if (Send(bufferOffset, nbByteRemaining, bytesSended, _ep) == PResult::P_GenericError)
				return PResult::P_GenericError;

			totalBytesSended += bytesSended;
		}

		return PResult::P_Success;
	}
	PResult UDPSocket::Send(Packet& _Packet, const IPEndpoint& _ep)
	{
		uint16_t encodedSize = htons(static_cast<u_short>(_Packet.buffer.size()));

		if (SendAll(&encodedSize, sizeof(uint16_t), _ep) != PResult::P_Success)
			return PResult::P_GenericError;

		if (SendAll(_Packet.buffer.data(), static_cast<int>(_Packet.buffer.size()), _ep) != PResult::P_Success)
			return PResult::P_GenericError;

		return PResult::P_Success;
	}
	PResult UDPSocket::Recv(void* _buffer, int _nbBytesReceived, int& _bytesRec, const IPEndpoint& _ep)
	{

		_bytesRec = recvfrom(handle, (char*)_buffer, _nbBytesReceived, NULL, _ep.GetUsingSockAddr(), NULL);

		if (_bytesRec == 0)
			return PResult::P_GenericError;

		if (_bytesRec == SOCKET_ERROR)
			return PrintWSALastErrorCode("[UDPSocket::Recv -> recv() : recvfrom()]");

		return PResult::P_Success;
	}
	PResult UDPSocket::RecvAll(void* _buffer, int _nbBytesToReceived, const IPEndpoint& _ep)
	{
		int totalBytesReceived = 0;

		while (totalBytesReceived < _nbBytesToReceived)
		{
			int nbBytesRemaining = _nbBytesToReceived - totalBytesReceived;
			int bytesReceived = 0;

			char* bufferOffset = (char*)(_buffer)+totalBytesReceived;

			if (Recv(bufferOffset, nbBytesRemaining, bytesReceived, _ep) == PResult::P_GenericError)
				return PResult::P_GenericError;

			totalBytesReceived += bytesReceived;
		}

		return PResult::P_Success;
	}
	PResult UDPSocket::Recv(Packet& _Packet, const IPEndpoint& _ep)
	{
		_Packet.Clear();

		uint16_t encodedSize = 0;

		// get buffer size from the packet first
		if (RecvAll(&encodedSize, sizeof(uint16_t), _ep) != PResult::P_Success)
			return PResult::P_GenericError;

		uint16_t bufferSize = ntohs(encodedSize);

		if (bufferSize > g_MaxPacketSize)
			return PResult::P_GenericError;

		_Packet.buffer.resize(bufferSize);

		if (RecvAll(&_Packet.buffer[0], bufferSize, _ep) != PResult::P_Success)
			return PResult::P_GenericError;

		return PResult::P_Success;
	}
}
#include "SocketBase.h"
#include <assert.h>
#include <iostream>
#include "Constants.h"


namespace GTPNet
{

	SocketBase::SocketBase()
	{
		handle = INVALID_SOCKET;
		ipVersion = IPVersion::Unknown;
		protocoleType = IPPROTO::IPPROTO_UDP;

		isBlocking = false;
	}

	SocketBase::~SocketBase()
	{
	}

	/// <summary>Socket Default Constructor, Insure using | IPv4 or IPv6 | TCP or UDP</summary>
		/// 
		/// <param name=" ProtocolType :"> Protocol that socket is using (default = TCP)</param>  
		/// <param name=" IpVersion :"> Ip format (default = IPv4)</param>  
		/// <param name=" Handle :"> Socket value which is init at socket creation (default = INVALID_SOCKET)</param>  
		/// 
		/// <returns>Socket</returns> 
	PResult SocketBase::Create(IPVersion _ipVersion, IPPROTO _protocole)
	{
		// insure using ipv4 or ipv6
		assert(_ipVersion == IPVersion::IPv4 || _ipVersion == IPVersion::IPv6);

		assert(_protocole == IPPROTO::IPPROTO_TCP || _protocole == IPPROTO::IPPROTO_UDP);

		ipVersion = _ipVersion;
		protocoleType = _protocole;

		int addrFamily = (_ipVersion == IPVersion::IPv4) ? AF_INET : AF_INET6;
		int type = (_protocole == IPPROTO::IPPROTO_TCP) ? SOCK_STREAM : SOCK_DGRAM;

		// attempting to create socket (AF_INET =  Ipv4 / AF_INET6 = Ipv6)
		handle = socket(addrFamily, type, _protocole);

		return (handle == INVALID_SOCKET) ? PrintWSALastErrorCode("SocketBase -> Create") : PResult::P_Success;

	}

	PResult SocketBase::Bind(IPEndpoint _endpoint)
	{
		assert(ipVersion == _endpoint.GetIpVersion());

		if (ipVersion == IPVersion::IPv4)
		{
			// for IPv4
			sockaddr_in addr = _endpoint.GetSockAddrIPv4();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));

			if (result != 0)
				return PrintWSALastErrorCode("[Socket::Bind - bind() -> ipVersion = IPv4]");
		}
		else
		{
			// for IPv6
			sockaddr_in6 addr = _endpoint.GetSockAddrIPv6();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));

			if (result != 0)
				return PrintWSALastErrorCode("[Socket::Bind - bind() -> ipVersion = IPv6]");
		}

		return PResult::P_Success;
	}

	PResult SocketBase::Close()
	{
		if (handle == INVALID_SOCKET)
			return PResult::P_GenericError;

		int result = closesocket(handle);

		if (result != 0)
			return PrintWSALastErrorCode("[Socket::Close - closesocket()]");

		handle = INVALID_SOCKET;

		return PResult::P_Success;
	}

	PResult SocketBase::PrintWSALastErrorCode(std::string _context)
	{

		std::cout << _context << " >> " << "Winsock error code : " << " >> " << WSAGetLastError() << std::endl;
		return PResult::P_GenericError;
	}

	bool SocketBase::AvailableToUse() const
	{
		return (handle != INVALID_SOCKET);
	}

	bool SocketBase::IsInBlocking() const
	{
		return isBlocking;
	}

	PResult SocketBase::SetBlocking(bool _isBlocking)
	{
		unsigned long nonBlocking = 1;
		unsigned long blocking = 0;

		int result = ioctlsocket(handle, FIONBIO, _isBlocking ? &blocking : &nonBlocking);

		if (result == SOCKET_ERROR)
			return PrintWSALastErrorCode("[Socket::SetBlocking -> ioctlsocket()]");

		isBlocking = _isBlocking;

		return PResult::P_Success;
	}

	PResult SocketBase::SetSocketOption(SocketOption _option, BOOL _value)
	{
		int result = 0;

		switch (_option)
		{
		case SocketOption::TCP_NoDelay:
			result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&_value, sizeof(_value));
			break;

		case SocketOption::IPv6_Only:
			result = setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&_value, sizeof(_value));
			break;

		default:
			return PResult::P_GenericError;
			break;
		}

		if (result != 0)
			return PrintWSALastErrorCode("[Socket::SetSocketOption -> setsockopt()]");

		return PResult::P_Success;
	}

	void SocketBase::SetHandle(SocketHandle _handle)
	{
		handle = _handle;
	}

	SocketHandle SocketBase::GetHandle() const
	{
		return handle;
	}

	IPVersion SocketBase::GetIPVersion() const
	{
		return ipVersion;
	}

	IPPROTO SocketBase::GetProtocol() const
	{
		return protocoleType;
	}
}
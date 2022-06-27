#include "IPEndpoint.h"
#include "Constants.h"
#include <assert.h>

namespace GTPNet
{
	IPEndpoint::IPEndpoint(const char* _ip, unsigned short _port) : port(_port)
	{
		//store IPV4 address
		in_addr addr;

		int resultv4 = inet_pton(AF_INET, _ip, &addr);

		if (resultv4 == 1)
		{
			if (addr.S_un.S_addr != INADDR_NONE)
			{
				ip_string = _ip;
				hostName = _ip;

				ip_bytes.resize(sizeof(ULONG));
				memcpy(&ip_bytes[0], &addr.S_un.S_addr, sizeof(ULONG));

				ipVersion = IPVersion::IPv4;

				return;
			}
		}

		// attempt to resolve hostname to ipv4 address
		addrinfo hints = {};
		hints.ai_family = AF_INET;
		addrinfo* hostinfo = nullptr;
		resultv4 = getaddrinfo(_ip, NULL, &hints, &hostinfo);

		if (resultv4 == 0)
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);

			// way to go to the network presentation format
			//host_addr->sin_addr.S_un.S_addr

			// For IPv4 address, pStringBuf should be large enough to hold at least 16 characters.
			ip_string.resize(16);
			inet_ntop(AF_INET, &host_addr->sin_addr, &ip_string[0], 16);



			hostName = _ip;

			ULONG ip_long = host_addr->sin_addr.S_un.S_addr;
			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &ip_long, sizeof(ULONG));

			ipVersion = IPVersion::IPv4;

			freeaddrinfo(hostinfo);
			return;
		}

		//store IPV6 address
		in_addr6 addr6;

		int resultv6 = inet_pton(AF_INET6, _ip, &addr6);

		if (resultv6 == 1)
		{
			ip_string = _ip;
			hostName = _ip;

			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &addr6.u, 16);

			ipVersion = IPVersion::IPv6;

			return;
		}

		// attempt to resolve hostname to ipv6 address
		addrinfo hintsv6 = {};
		hintsv6.ai_family = AF_INET6;
		addrinfo* hostinfov6 = nullptr;
		resultv6 = getaddrinfo(_ip, NULL, &hintsv6, &hostinfov6);

		if (resultv6 == 0)
		{
			sockaddr_in6* host_addr = reinterpret_cast<sockaddr_in6*>(hostinfov6->ai_addr);

			// For IPv6 address, pStringBuf should be large enough to hold at least 46 characters.
			ip_string.resize(46);
			inet_ntop(AF_INET6, &host_addr->sin6_addr, &ip_string[0], 16);

			hostName = _ip;

			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &host_addr->sin6_addr, 16);

			ipVersion = IPVersion::IPv6;

			freeaddrinfo(hostinfov6);
			return;
		}
	}

	IPEndpoint::IPEndpoint(sockaddr* _addr)
	{
		if (_addr->sa_family != AF_INET && _addr->sa_family != AF_INET6)
		{
			LOGGER->Log("IPEndpoint : Wrong sockaddr family format");
			return;
		}

		if (_addr->sa_family == AF_INET)
		{
			sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(_addr);
			ipVersion = IPVersion::IPv4;
			// from network byte order to host byte order (from big Endian to Little Endian) -> ntohs 
			port = ntohs(addrv4->sin_port);
			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &addrv4->sin_addr, sizeof(ULONG));
			ip_string.resize(16);
			inet_ntop(AF_INET, &addrv4->sin_addr, &ip_string[0], 16);

			hostName = ip_string;
		}
		else
		{
			sockaddr_in6* addrv6 = reinterpret_cast<sockaddr_in6*>(_addr);
			ipVersion = IPVersion::IPv6;
			// from network byte order to host byte order (from big Endian to Little Endian) -> ntohs 
			port = ntohs(addrv6->sin6_port);
			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &addrv6->sin6_addr, 16);
			ip_string.resize(46);
			inet_ntop(AF_INET6, &addrv6->sin6_addr, &ip_string[0], 46);

			hostName = ip_string;
		}
	}

	IPVersion IPEndpoint::GetIpVersion() const
	{
		return ipVersion;
	}

	std::string IPEndpoint::GetHostName() const
	{
		return hostName;
	}

	std::string IPEndpoint::GetIpString() const
	{
		return ip_string;
	}

	std::vector<uint8_t> IPEndpoint::GetIpBytes() const
	{
		return ip_bytes;
	}

	unsigned short IPEndpoint::GetPort() const
	{
		return port;
	}

	sockaddr_in IPEndpoint::GetSockAddrIPv4() const
	{
		assert(ipVersion == IPVersion::IPv4);
		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr, &ip_bytes[0], sizeof(ULONG));
		// convert port to network byte order (from little endiannes to big endiannes) htonS -> ushort, htonL -> ulong
		addr.sin_port = htons(port);
		return addr;
	}

	sockaddr_in6 IPEndpoint::GetSockAddrIPv6() const
	{
		assert(ipVersion == IPVersion::IPv6);
		sockaddr_in6 addr = {};
		addr.sin6_family = AF_INET6;
		memcpy(&addr.sin6_addr, &ip_bytes[0], 16);
		// convert port to network byte order (from little endiannes to big endiannes) htonS -> ushort, htonL -> ulong
		addr.sin6_port = htons(port);
		return addr;
	}

	sockaddr* IPEndpoint::GetUsingSockAddr() const
	{
		struct sockaddr_in addr_v4 = GetSockAddrIPv4();
		struct sockaddr_in6 addr_v6 = GetSockAddrIPv6();

		return (ipVersion == IPVersion::IPv4) ? reinterpret_cast<struct sockaddr*>(&addr_v4)
		: reinterpret_cast<struct sockaddr*>(&addr_v6);
	}

	void IPEndpoint::Print() const
	{
		switch (ipVersion)
		{
		case IPVersion::IPv4:
			std::cout << "IP Version : IPv4" << std::endl;
			break;
		case IPVersion::IPv6:
			std::cout << "IP Version : IPv6" << std::endl;
			break;
		default:
			std::cout << "IP Version : Unknown" << std::endl;
			break;
		}

		std::cout << "Hostname: " << hostName << std::endl;
		std::cout << "IP: " << ip_string << std::endl;
		std::cout << "Port: " << port << std::endl;
		std::cout << "Ip_Bytes ->" << std::endl;
		for (auto& digit : ip_bytes)
		{
			std::cout << (int)digit << std::endl;
		}
	}

}

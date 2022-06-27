#pragma once
#include "IPVersion.h"
#include <WS2tcpip.H>
#include <string>
#include <vector>
#include <iostream>
#include "NetLog.h"

namespace GTPNet
{
	class IPEndpoint
	{
	public:
		IPEndpoint(const char* _ip, unsigned short _port);
		IPEndpoint(sockaddr* _addr);

		IPVersion GetIpVersion() const;
		std::string GetHostName() const;
		std::string GetIpString() const;
		std::vector<uint8_t> GetIpBytes() const;
		unsigned short GetPort() const;
		sockaddr_in GetSockAddrIPv4() const;
		sockaddr_in6 GetSockAddrIPv6() const;
		sockaddr* GetUsingSockAddr() const;

		void Print() const;

	private:
		IPVersion ipVersion = IPVersion::Unknown;
		std::string hostName = "";
		std::string ip_string = "";
		std::vector<uint8_t> ip_bytes;
		unsigned short port = 0;

	};
}

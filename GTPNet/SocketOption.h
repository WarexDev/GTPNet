#pragma once

namespace GTPNet
{
	enum class SocketOption
	{
		TCP_NoDelay, // When True, Disable the nagle's algorithm -> https://fr.wikipedia.org/wiki/Algorithme_de_Nagle
		IPv6_Only, // TRUE : IPv6 only can connect, FALSE : Ipv6 and IPv4 can both connect
	};
}

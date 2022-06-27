#pragma once

#include "SocketHandle.h"
#include "PResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include "Packet.h"
#include <string>

namespace GTPNet
{
	class SocketBase
	{
	public:
		PResult Bind(IPEndpoint _ep); 

		SocketHandle GetHandle() const;
		IPVersion GetIPVersion() const;
		IPPROTO GetProtocol() const;

		PResult SetBlocking(bool _isBlocking);
		PResult Close();

	protected:
		SocketBase();
		virtual ~SocketBase();

		PResult Create(IPVersion _ipVersion, IPPROTO _protocole);
		PResult SetSocketOption(SocketOption _option, BOOL _value);
		void SetHandle(SocketHandle _handle);

		static PResult PrintWSALastErrorCode(std::string _context);

		bool AvailableToUse() const;
		bool IsInBlocking() const;

	protected:
		IPVersion ipVersion = IPVersion::Unknown;
		SocketHandle handle = INVALID_SOCKET;
		IPPROTO protocoleType;

	private:
		bool isBlocking;

	};
}

#pragma once
#include <WinSock2.h>
#include <vector>
#include <string>
#include "PacketException.h"
#include "PacketType.h"

namespace GTPNet
{
	class Packet
	{
	public:
		Packet(PacketType _packetType = PacketType::PT_Undefined);

		void Clear();
		void Append(const void* data, uint32_t size);
		void AssignPacketType(const PacketType _packetType);

		PacketType GetPacketType();

		Packet& operator<<(uint32_t _data);
		Packet& operator>>(uint32_t& _data);

		Packet& operator<<(std::string _data);
		Packet& operator>>(std::string& _data);

		std::vector<char> buffer;
		uint32_t extractionOffset;

	};
}
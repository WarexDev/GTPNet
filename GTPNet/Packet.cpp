#include "Packet.h"
#include "Constants.h"

namespace GTPNet
{

	Packet::Packet(PacketType _packetType /*= PacketType::PT_Undefined*/)
	{
		Clear();
		AssignPacketType(_packetType);
	}

	void Packet::Clear()
	{
		buffer.resize(sizeof(PacketType));
		AssignPacketType(PacketType::PT_Undefined);
		extractionOffset = sizeof(PacketType);
	}

	void Packet::Append(const void * data, uint32_t size)
	{
		if ((buffer.size() + size) > g_MaxPacketSize)
			throw PacketException("[Packet::Append(const void *, uint32_t)] -> Packet size exceeded the packet size limit");

		buffer.insert(buffer.end(), (char*)data, (char*)data + sizeof(uint32_t));
	}

	void Packet::AssignPacketType(const PacketType _packetType)
	{
		PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
		*packetTypePtr = static_cast<PacketType>(htons(_packetType));
	}

	PacketType Packet::GetPacketType()
	{
		PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
		return static_cast<PacketType>(ntohs(*packetTypePtr));
	}

	Packet& Packet::operator<<(uint32_t _data)
	{
		_data = htonl(_data);
		Append(&_data, sizeof(uint32_t));
		return *this;
	}


	Packet& Packet::operator>>(uint32_t& _data)
	{
		if (extractionOffset + sizeof(uint32_t) > buffer.size())
			throw PacketException("[Packet::operator>>(uint32_t&)] -> ExtractionOffset exceeded the buffer size");

		_data = *reinterpret_cast<uint32_t*>(&buffer[extractionOffset]);
		_data = ntohl(_data);
		extractionOffset += sizeof(uint32_t);
		return *this;
	}

	Packet& Packet::operator<<(std::string _data)
	{
		*this << (uint32_t)_data.size();
		Append(_data.data(), static_cast<uint32_t>(_data.size()));
		return *this;
	}

	Packet& Packet::operator>>(std::string& _data)
	{
		_data.clear();

		uint32_t stringSize = 0;
		*this >> stringSize;

		if ((extractionOffset + stringSize) > buffer.size())
			throw PacketException("[Packet::operator>>(std::string&)] -> ExtractionOffset exceeded the buffer size");

		_data.resize(stringSize);
		_data.assign(&buffer[extractionOffset], stringSize);

		extractionOffset += stringSize;
		return *this;
	}
}
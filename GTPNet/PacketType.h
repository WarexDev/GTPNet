#pragma once
#include <stdint.h>

namespace GTPNet
{
	enum PacketType : uint16_t
	{
		// Debug/Test purpose
		PT_Undefined,
		PT_ChatMessage,
		PT_IntegerArray
	};
}
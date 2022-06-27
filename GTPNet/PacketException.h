#pragma once
#include <string>

namespace GTPNet
{
	class PacketException
	{

	public:
		PacketException(std::string _exception)
			: exception(_exception)
		{}

		const char* What() { return exception.c_str(); }

		std::string ToString() { return exception; }

	private:
		std::string exception;
	};
}
#pragma once
#include <GTPNet/IncludeMe.h>

namespace GTPNet
{
	class Client
	{
	public:
		Client();
		virtual ~Client();

		virtual bool Frame() = 0;

		/// <summary>
		/// Data is add to the packet only if it's the correct Datatype
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="_data"></param>
		/// <returns></returns>
		template<typename T>
		bool AddPacketData(T _data);

	protected:
		class IPEndpoint* endPoint;
		class Packet packet;
	};
}

#include "TCPClient.h"
#include <iostream>

TCPClient::TCPClient()
{
}

bool TCPClient::Connect(IPEndpoint _Ep)
{
	isConnected = false;

	if (Network::Initialize())
	{
		std::cout << "Winsock API as been successfully initialized" << std::endl;

		endPoint = &_Ep;

		if (socket.Create(endPoint->GetIpVersion()) == PResult::P_Success)
		{
			std::cout << "Socket successfully created" << std::endl;

			if (socket.SetBlocking(true) != PResult::P_Success)
				return false;

			if (socket.Connect(_Ep) == PResult::P_Success)
			{
				std::cout << "Successfully connected to server" << std::endl;
				isConnected = true;
				return true;
			}
			else
			{
				std::cout << "Failed to connect to server" << std::endl;
			}

			socket.Close();
		}
		else
		{
			std::cerr << "Socket failed to create" << std::endl;
		}
	}

	return false;
}

bool TCPClient::IsConnected()
{
	return isConnected;
}

bool TCPClient::Frame()
{

	std::string a("Test");
	Packet stringPacket(PacketType::PT_ChatMessage);
	stringPacket << a;

	Packet integerPacket(PacketType::PT_IntegerArray);
	uint32_t arraySize = 6;
	integerPacket << arraySize;
	uint32_t packetArray[6];

	for (size_t i = 0; i < arraySize; i++)
	{
		packetArray[i] = static_cast<uint32_t>(rand() % 100);
		integerPacket << packetArray[i];
	}


	std::cout << "Attempting to send chunk of data ..." << std::endl;

	if (socket.Send((rand() % 2 == 0) ? stringPacket : integerPacket) != PResult::P_Success)
	{
		isConnected = false;
		return false;
	}

	Sleep(500);

	return true;
}

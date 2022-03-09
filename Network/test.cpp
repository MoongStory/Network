#include <iostream>
#include <atlstr.h>

#include "Network.h"

int main()
{
	MOONG::NETWORK::Network network;

	const std::string URL = "https://www.google.com";
	const std::string PORT = "80";

	const int PORT_0 = 54321;
	const int PORT_1 = 80;

	if (network.InternetConnected())
	{
		std::cout << "InternetConnected()\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "InternetConnected()\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	if (network.InternetConnected(URL))
	{
		std::cout << "InternetConnected[" << URL.c_str() << "]\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "InternetConnected[" << URL.c_str() << "]\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	std::vector<MOONG::NETWORK::ADDR_INFO> addr_info;
	if (network.getHostByName(URL, PORT_1, addr_info) == MOONG::NETWORK::RETURN::SUCCESS)
	{
		std::cout << "URL[" << URL.c_str() << "], PORT[" << PORT_1 << "] Information" << std::endl;
		for(size_t i = 0; i < addr_info.size(); i++)
		{
			std::cout << "\tFlags[0x" << std::hex << addr_info[i].getFlags() << "]" << std::endl;
			std::cout << "\tFamily[" << addr_info[i].getFamily().c_str() << "]" << std::endl;
			std::cout << "\tAddress[" << addr_info[i].getIPAddress().c_str() << "]" << std::endl;
			std::cout << "\tSocket type[" << addr_info[i].getSocketType().c_str() << "]" << std::endl;
			std::cout << "\tProtocol[" << addr_info[i].getProtocol().c_str() << "]" << std::endl;
			std::cout << "\tLength of this sockaddr[" << std::dec << addr_info[i].getLengthOfThisSockaddr() << "]" << std::endl;
			std::cout << "\tCanonical name[" << std::dec << addr_info[i].getCanonicalName().c_str() << "]" << std::endl;

			std::cout << std::endl;

			if (network.Ping(addr_info[i].getIPAddress()) == MOONG::NETWORK::RETURN::SUCCESS)
			{
				std::cout << "Ping[" << addr_info[i].getIPAddress().c_str() << "]\n\tSUCCESS" << std::endl;
			}
			else
			{
				std::cout << "Ping[" << addr_info[i].getIPAddress().c_str() << "]\n\tFAIL" << std::endl;
			}

			std::cout << std::endl;

			if (network.Ping(addr_info[i].getIPAddress(), PORT_0) == MOONG::NETWORK::RETURN::SUCCESS)
			{
				std::cout << "Ping[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_0 << "]\n\tSUCCESS" << std::endl;
			}
			else
			{
				std::cout << "Ping[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_0 << "]\n\tFAIL" << std::endl;
			}

			std::cout << std::endl;

			if (network.Ping(addr_info[i].getIPAddress(), PORT_1) == MOONG::NETWORK::RETURN::SUCCESS)
			{
				std::cout << "Ping[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_1 << "]\n\tSUCCESS" << std::endl;
			}
			else
			{
				std::cout << "Ping[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_1 << "]\n\tFAIL" << std::endl;
			}
		}
	}

	return EXIT_SUCCESS;
}
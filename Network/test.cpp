#include <iostream>
#include <atlstr.h>

#include "Network.h"

int main()
{
	const std::string URL = "https://www.google.com";
	const std::string WRONG_URL = "https://www.something_wrong_url.com";
	const std::string PORT = "80";

	const int PORT_0 = 54321;
	const int PORT_1 = 80;

	std::cout << "Port from URL\n\t[" << MOONG::NETWORK::Network::getPortFromURL(URL) << "]" << std::endl;

	std::cout << std::endl;

	if (MOONG::NETWORK::Network::InternetConnected())
	{
		std::cout << "InternetConnected()\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "InternetConnected()\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	if (MOONG::NETWORK::Network::InternetConnected(URL))
	{
		std::cout << "InternetConnected\n\taddress[" << URL.c_str() << "]\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "InternetConnected\n\taddress[" << URL.c_str() << "]\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	if (MOONG::NETWORK::Network::Ping(URL))
	{
		std::cout << "Ping\n\taddress[" << URL.c_str() << "]\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "Ping\n\taddress[" << URL.c_str() << "]\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	if (MOONG::NETWORK::Network::Ping(URL, PORT_0))
	{
		std::cout << "Ping\n\taddress[" << URL.c_str() << "], Port[" << PORT_0 << "]\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "Ping\n\taddress[" << URL.c_str() << "], Port[" << PORT_0 << "]\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	if (MOONG::NETWORK::Network::Ping(URL, PORT_1))
	{
		std::cout << "Ping\n\taddress[" << URL.c_str() << "], Port[" << PORT_1 << "]\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "Ping\n\taddress[" << URL.c_str() << "], Port[" << PORT_1 << "]\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	if (MOONG::NETWORK::Network::Ping(WRONG_URL))
	{
		std::cout << "Ping\n\taddress[" << WRONG_URL.c_str() << "]\n\tSUCCESS" << std::endl;
	}
	else
	{
		std::cout << "Ping\n\taddress[" << WRONG_URL.c_str() << "]\n\tFAIL" << std::endl;
	}

	std::cout << std::endl;

	std::vector<MOONG::NETWORK::ADDR_INFO> addr_info;
	if (MOONG::NETWORK::Network::getAddrInfoFromURL(URL, PORT_1, addr_info) == MOONG::NETWORK::RETURN::SUCCESS)
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
			std::cout << "\tCanonical name[" << addr_info[i].getCanonicalName().c_str() << "]" << std::endl;

			std::cout << std::endl;

			if (MOONG::NETWORK::Network::Ping(addr_info[i].getIPAddress()))
			{
				std::cout << "Ping\n\taddress[" << addr_info[i].getIPAddress().c_str() << "]\n\tSUCCESS" << std::endl;
			}
			else
			{
				std::cout << "Ping\n\taddress[" << addr_info[i].getIPAddress().c_str() << "]\n\tFAIL" << std::endl;
			}

			std::cout << std::endl;

			if (MOONG::NETWORK::Network::Ping(addr_info[i].getIPAddress(), PORT_0))
			{
				std::cout << "Ping\n\taddress[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_0 << "]\n\tSUCCESS" << std::endl;
			}
			else
			{
				std::cout << "Ping\n\taddress[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_0 << "]\n\tFAIL" << std::endl;
			}

			std::cout << std::endl;

			if (MOONG::NETWORK::Network::Ping(addr_info[i].getIPAddress(), PORT_1))
			{
				std::cout << "Ping\n\taddress[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_1 << "]\n\tSUCCESS" << std::endl;
			}
			else
			{
				std::cout << "Ping\n\taddress[" << addr_info[i].getIPAddress().c_str() << "], Port[" << PORT_1 << "]\n\tFAIL" << std::endl;
			}
		}
	}

	return EXIT_SUCCESS;
}
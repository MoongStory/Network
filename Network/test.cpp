#include <iostream>
#include <atlstr.h>

#include "Network.h"

// https://github.com/MoongStory/Url
#include "../../Url/Url/Url.h"

int main()
{
	try
	{
		const std::string URL = "https://www.google.com";
		const std::string WRONG_URL = "https://www.something_wrong_url.com";

		const int TEST_PORT = 54321;

		std::cout << "Port from URL\n\t[" << MOONG::Url::get_port(URL) << "]" << std::endl;

		std::cout << std::endl;

		if (MOONG::Network::check_connect_internet())
		{
			std::cout << "CheckConnectInternet()\n\tSUCCESS" << std::endl;
		}
		else
		{
			std::cout << "CheckConnectInternet()\n\tFAIL" << std::endl;
		}

		std::cout << std::endl;

		if (MOONG::Network::check_connect_internet(URL))
		{
			std::cout << "CheckConnectInternet\n\taddress[" << URL.c_str() << "]\n\tSUCCESS" << std::endl;
		}
		else
		{
			std::cout << "CheckConnectInternet\n\taddress[" << URL.c_str() << "]\n\tFAIL" << std::endl;
		}

		std::cout << std::endl;

		if (MOONG::Network::check_connect_tcp(URL))
		{
			std::cout << "CheckConnectTCP\n\taddress[" << URL.c_str() << "]\n\tSUCCESS" << std::endl;
		}
		else
		{
			std::cout << "CheckConnectTCP\n\taddress[" << URL.c_str() << "]\n\tFAIL" << std::endl;
		}

		std::cout << std::endl;

		if (MOONG::Network::check_connect_tcp(URL, TEST_PORT))
		{
			std::cout << "CheckConnectTCP\n\taddress[" << URL.c_str() << "], Port[" << TEST_PORT << "]\n\tSUCCESS" << std::endl;
		}
		else
		{
			std::cout << "CheckConnectTCP\n\taddress[" << URL.c_str() << "], Port[" << TEST_PORT << "]\n\tFAIL" << std::endl;
		}

		std::cout << std::endl;

		if (MOONG::Network::check_connect_tcp(URL, MOONG::Url::get_port(URL)))
		{
			std::cout << "CheckConnectTCP\n\taddress[" << URL.c_str() << "], Port[" << MOONG::Url::get_port(URL) << "]\n\tSUCCESS" << std::endl;
		}
		else
		{
			std::cout << "CheckConnectTCP\n\taddress[" << URL.c_str() << "], Port[" << MOONG::Url::get_port(URL) << "]\n\tFAIL" << std::endl;
		}

		std::cout << std::endl;

		if (MOONG::Network::check_connect_tcp(WRONG_URL))
		{
			std::cout << "CheckConnectTCP\n\taddress[" << WRONG_URL.c_str() << "]\n\tSUCCESS" << std::endl;
		}
		else
		{
			std::cout << "CheckConnectTCP\n\taddress[" << WRONG_URL.c_str() << "]\n\tFAIL" << std::endl;
		}

		std::cout << std::endl;

		std::vector<MOONG::NETWORK::ADDR_INFO> addr_info;
		if (MOONG::Network::get_addr_Info_from_url(URL, MOONG::Url::get_port(URL), addr_info) == MOONG::NETWORK::RETURN::SUCCESS)
		{
			std::cout << "URL[" << URL.c_str() << "], PORT[" << MOONG::Url::get_port(URL) << "] Information" << std::endl;
			for (size_t i = 0; i < addr_info.size(); i++)
			{
				std::cout << "\tFlags[0x" << std::hex << addr_info[i].get_flags() << "]" << std::endl;
				std::cout << "\tFamily[" << addr_info[i].get_family().c_str() << "]" << std::endl;
				std::cout << "\tAddress[" << addr_info[i].get_ip_address().c_str() << "]" << std::endl;
				std::cout << "\tSocket type[" << addr_info[i].get_socket_type().c_str() << "]" << std::endl;
				std::cout << "\tProtocol[" << addr_info[i].get_protocol().c_str() << "]" << std::endl;
				std::cout << "\tLength of this sockaddr[" << std::dec << addr_info[i].get_length_of_this_sock_addr() << "]" << std::endl;
				std::cout << "\tCanonical name[" << addr_info[i].get_canonical_name().c_str() << "]" << std::endl;

				std::cout << std::endl;

				if (MOONG::Network::check_connect_tcp(addr_info[i].get_ip_address()))
				{
					std::cout << "CheckConnectTCP\n\taddress[" << addr_info[i].get_ip_address().c_str() << "]\n\tSUCCESS" << std::endl;
				}
				else
				{
					std::cout << "CheckConnectTCP\n\taddress[" << addr_info[i].get_ip_address().c_str() << "]\n\tFAIL" << std::endl;
				}

				std::cout << std::endl;

				if (MOONG::Network::check_connect_tcp(addr_info[i].get_ip_address(), TEST_PORT))
				{
					std::cout << "CheckConnectTCP\n\taddress[" << addr_info[i].get_ip_address().c_str() << "], Port[" << TEST_PORT << "]\n\tSUCCESS" << std::endl;
				}
				else
				{
					std::cout << "CheckConnectTCP\n\taddress[" << addr_info[i].get_ip_address().c_str() << "], Port[" << TEST_PORT << "]\n\tFAIL" << std::endl;
				}

				std::cout << std::endl;

				if (MOONG::Network::check_connect_tcp(addr_info[i].get_ip_address(), MOONG::Url::get_port(URL)))
				{
					std::cout << "CheckConnectTCP\n\taddress[" << addr_info[i].get_ip_address().c_str() << "], Port[" << MOONG::Url::get_port(URL) << "]\n\tSUCCESS" << std::endl;
				}
				else
				{
					std::cout << "CheckConnectTCP\n\taddress[" << addr_info[i].get_ip_address().c_str() << "], Port[" << MOONG::Url::get_port(URL) << "]\n\tFAIL" << std::endl;
				}
			}
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
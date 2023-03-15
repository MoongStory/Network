#include <iostream>
#include <atlstr.h>

#include "Network.h"

#include "../../Url/Url/Url.h"

int main()
{
	const std::string URL = "https://www.google.com";
	const std::string WRONG_URL = "https://www.something_wrong_url.com";

	const int TEST_PORT = 54321;

	std::cout << "get_port(" << URL.c_str() << ")[" << MOONG::Url::get_port(URL) << "]" << std::endl;

	std::cout << std::endl;

	std::cout << "check_connect_internet()[" << (MOONG::Network::check_connect_internet() == true ? "성공" : "실패") << "]" << std::endl;

	std::cout << std::endl;

	std::cout << "check_connect_internet(" << URL.c_str() << ")[" << (MOONG::Network::check_connect_internet(URL) == true ? "성공" : "실패") << "]" << std::endl;

	std::cout << std::endl;

	std::cout << "check_connect_tcp(" << URL.c_str() << ")[" << (MOONG::Network::check_connect_tcp(URL) == true ? "성공" : "실패") << "]" << std::endl;

	std::cout << std::endl;

	std::cout << "check_connect_tcp(" << URL.c_str() << ", " << TEST_PORT << ")[" << (MOONG::Network::check_connect_tcp(URL, TEST_PORT) == true ? "성공" : "실패") << "]" << std::endl;

	std::cout << std::endl;

	std::cout << "check_connect_tcp(" << URL.c_str() << ", " << MOONG::Url::get_port(URL) << ")[" << (MOONG::Network::check_connect_tcp(URL, MOONG::Url::get_port(URL)) == true ? "성공" : "실패") << "]" << std::endl;

	std::cout << std::endl;

	std::cout << "check_connect_tcp(" << WRONG_URL.c_str() << ")[" << (MOONG::Network::check_connect_tcp(WRONG_URL) == true ? "성공" : "실패") << "]" << std::endl;

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

			std::cout << "check_connect_tcp(" << addr_info[i].get_ip_address().c_str() << ")[" << (MOONG::Network::check_connect_tcp(addr_info[i].get_ip_address()) == true ? "성공" : "실패") << "]" << std::endl;

			std::cout << std::endl;

			std::cout << "check_connect_tcp(" << addr_info[i].get_ip_address().c_str() << ", " << TEST_PORT << ")[" << (MOONG::Network::check_connect_tcp(addr_info[i].get_ip_address(), TEST_PORT) == true ? "성공" : "실패") << "]" << std::endl;

			std::cout << std::endl;

			std::cout << "check_connect_tcp(" << addr_info[i].get_ip_address().c_str() << ", " <<  MOONG::Url::get_port(URL) << ")[" << (MOONG::Network::check_connect_tcp(addr_info[i].get_ip_address(), MOONG::Url::get_port(URL)) == true ? "성공" : "실패") << "]" << std::endl;
		}
	}

	std::cout << std::endl;
	
	const std::string IP = "255.255.255.255";

	std::cout << "is_ip_v4_format(" << IP.c_str() << ")[" << (MOONG::Network::is_ip_v4_format(IP) == true ? "성공" : "실패") << "]" << std::endl;

	return EXIT_SUCCESS;
}
#include <iostream>
#include <atlstr.h>

#include "Network.h"

#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

int main()
{
	MOONG::NETWORK::Network network;

	const std::string URL = "https://www.google.com";

	const int PORT_0 = 54321;
	const int PORT_1 = 80;

	if (network.InternetConnected())
	{
		std::cout << "success, InternetConnected()" << std::endl;
	}
	else
	{
		std::cout << "fail, InternetConnected()" << std::endl;
	}

	std::cout << std::endl;

	if (network.InternetConnected(URL))
	{
		std::cout << "success, InternetConnected[" << URL.c_str() << "]" << std::endl;
	}
	else
	{
		std::cout << "fail, InternetConnected[" << URL.c_str() << "]" << std::endl;
	}

	std::cout << std::endl;

	char IP[64] = { 0 };

	struct hostent *remote_host = NULL;
	if (network.getHostByName(URL.c_str(), &remote_host) == MOONG::NETWORK::RETURN::SUCCESS)
	{
		char **pAlias = NULL;
		int i = 0;
		struct in_addr addr = {0};

		printf("Function returned:\n");
		printf("\tOfficial name: %s\n", remote_host->h_name);
		for (pAlias = remote_host->h_aliases; *pAlias != 0; pAlias++) {
			printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
		}
		printf("\tAddress type: ");
		switch (remote_host->h_addrtype) {
		case AF_INET:
			printf("AF_INET\n");
			break;
		case AF_NETBIOS:
			printf("AF_NETBIOS\n");
			break;
		default:
			printf(" %d\n", remote_host->h_addrtype);
			break;
		}
		printf("\tAddress length: %d\n", remote_host->h_length);

		i = 0;
		if (remote_host->h_addrtype == AF_INET)
		{
			while (remote_host->h_addr_list[i] != 0) {
				inet_ntop(remote_host->h_addrtype, remote_host->h_addr_list[i++], IP, sizeof(IP));
				printf("\tIP Address #%d: %s\n", i, IP);
			}
		}
		else if (remote_host->h_addrtype == AF_NETBIOS)
		{
			printf("NETBIOS address was returned\n");
		}
	}

	std::cout << std::endl;

	if (network.Ping(IP) == MOONG::NETWORK::RETURN::SUCCESS)
	{
		std::cout << "success, Ping[" << IP << "]" << std::endl;
	}
	else
	{
		std::cout << "fail, Ping[" << IP << "]" << std::endl;
	}

	std::cout << std::endl;

	if (network.Ping(IP, PORT_0) == MOONG::NETWORK::RETURN::SUCCESS)
	{
		std::cout << "success, Ping[" << IP << "], Port[" << PORT_0 << "]" << std::endl;
	}
	else
	{
		std::cout << "fail, Ping[" << IP << "], Port[" << PORT_0 << "]" << std::endl;
	}

	std::cout << std::endl;

	if (network.Ping(IP, PORT_1) == MOONG::NETWORK::RETURN::SUCCESS)
	{
		std::cout << "success, Ping[" << IP << "], Port[" << PORT_1 << "]" << std::endl;
	}
	else
	{
		std::cout << "fail, Ping[" << IP << "], Port[" << PORT_1 << "]" << std::endl;
	}

	return EXIT_SUCCESS;
}
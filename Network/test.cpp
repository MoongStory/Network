#include <iostream>
#include <atlstr.h>

#include "Network.h"

int main()
{
	MOONG::NETWORK::Network network;

	const CStringA URL = "https://www.google.com";

	const CStringA IP = "142.251.42.208";

	if (network.InternetConnected())
	{
		std::cout << "success, InternetConnected()" << std::endl;
	}
	else
	{
		std::cout << "fail, InternetConnected()" << std::endl;
	}

	if (network.InternetConnected(URL))
	{
		std::cout << "success, InternetConnected(" << URL << ")" << std::endl;
	}
	else
	{
		std::cout << "fail, InternetConnected(" << URL << ")" << std::endl;
	}

	if (network.Ping(IP) ==  MOONG::NETWORK::RETURN::SUCCESS)
	{
		std::cout << "success, Ping(" << IP << ")" << std::endl;
	}
	else
	{
		std::cout << "fail, Ping(" << IP << ")" << std::endl;
	}

	return EXIT_SUCCESS;
}
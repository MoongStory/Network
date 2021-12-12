//TODO: ping 체크
//TODO: url 체크
//TODO: PC 인터넷 상태 체크

//TODO: 회사에 코드 있음

#include <iostream>

#include "Network.h"

int main()
{
	MOONG::NETWORK::Network network;

	const std::string URL = "https://www.google.com";

	const std::string IP = "142.251.42.208";

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

	if (network.Ping(IP))
	{
		std::cout << "success, Ping(" << IP << ")" << std::endl;
	}
	else
	{
		std::cout << "fail, Ping(" << IP << ")" << std::endl;
	}

	return EXIT_SUCCESS;
}
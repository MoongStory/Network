//TODO: ping üũ
//TODO: url üũ
//TODO: PC ���ͳ� ���� üũ

//TODO: ȸ�翡 �ڵ� ����

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
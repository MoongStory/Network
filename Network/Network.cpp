#include "Network.h"

#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

#include <stdlib.h>

BOOL MOONG::NETWORK::Network::InternetConnected()
{
	DWORD dwFlag = 0;
	TCHAR szName[256] = {0};
	
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0);
}

BOOL MOONG::NETWORK::Network::InternetConnected(std::string param_url)
{
#ifdef _UNICODE
	const size_t new_size_w = strlen(param_url.c_str()) + 1;
	size_t converted_chars = 0;
	wchar_t* url = new wchar_t[new_size_w];
	mbstowcs_s(&converted_chars, url, new_size_w, param_url.c_str(), _TRUNCATE);
#elif _MBCS
	const size_t new_size = (strlen(param_url.c_str()) + 1) * 2;
	char* url = new char[new_size];
	strcpy_s(url, new_size, param_url.c_str());
#endif

	return InternetCheckConnection(url, FLAG_ICC_FORCE_CONNECTION, NULL);
}

BOOL MOONG::NETWORK::Network::Ping(std::string ip)
{
	std::string command = "ping -n 1 " + ip;
	
 	FILE* file = _popen(command.data(), "r");

	char output[300] = {0};
 	fread(output, 1, sizeof(output), file);

	//printf("%s\n", output);

	fclose(file);

	std::string response_msg = ip + "¿« ¿¿¥‰";

	if(strstr(output, response_msg.data()))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
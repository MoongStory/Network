#include "Network.h"

#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

#include <Winsock.h>
#pragma comment (lib, "Ws2_32.lib")

BOOL MOONG::NETWORK::Network::InternetConnected() const
{
	DWORD dwFlag = 0;
	TCHAR szName[256] = { 0 };

#if _MSC_VER > 1200
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0);
#else
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0) ? true : false;
#endif
}

BOOL MOONG::NETWORK::Network::InternetConnected(const std::string param_url) const
{
	return InternetCheckConnectionA(param_url.c_str(), FLAG_ICC_FORCE_CONNECTION, NULL) ? true : false;
}

int MOONG::NETWORK::Network::Ping(const std::string IP, const unsigned int port/* = 80*/, const unsigned int param_timeout/* = 1*/)
{
    WSADATA wsaData;
    int err = 0;

    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        //printf("WSAStartup failed with error: %d\n", err);

		return MOONG::NETWORK::RETURN::FAILURE::WSASTARTUP_FAILED;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		//printf("Could not find a usable version of Winsock.dll\n");

		WSACleanup();

		return MOONG::NETWORK::RETURN::FAILURE::COULD_NOT_FIND_A_USABLE_VERSION_OF_WINSOCK_DLL;
	}
	else
	{
		//printf("The Winsock 2.2 dll was found okay\n");
	}

	/* The Winsock DLL is acceptable. Proceed to use it. */
	/* Add network programming using Winsock here */
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	unsigned int addr = inet_addr(IP.c_str());

	if (addr == INADDR_NONE)
	{
		return MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM;
	}

	struct sockaddr_in address;
	address.sin_addr.s_addr = addr;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;

	// set the socket in non-blocking
	unsigned long iMode = 1;
	int return_value = ioctlsocket(sock, FIONBIO, &iMode);
	if (return_value != NO_ERROR)
	{
		//printf("ioctlsocket set non-block failed with error[%d]\n", return_value);
	}

	if(connect(sock, (struct sockaddr *)&address, sizeof(address)) == false)
	{
		return MOONG::NETWORK::RETURN::FAILURE::SOCKET_CONNECT;
	}

	// set the socket in blocking
	iMode = 0;
	return_value = ioctlsocket(sock, FIONBIO, &iMode);
	if (return_value != NO_ERROR)
	{
		//printf("ioctlsocket set block failed with error[%d]\n", return_value);
	}

	fd_set Write, Err;
	FD_ZERO(&Write);
	FD_ZERO(&Err);
	FD_SET(sock, &Write);
	FD_SET(sock, &Err);
	
	TIMEVAL timeout;
	timeout.tv_sec = param_timeout;
	timeout.tv_usec = 0;

	// check if the socket is ready
	select(0, NULL, &Write, &Err, &timeout);
	if(FD_ISSET(sock, &Write))
	{
		/* then call WSACleanup when done using the Winsock dll */
		WSACleanup();

		return MOONG::NETWORK::RETURN::SUCCESS;
	}
	else
	{
		/* then call WSACleanup when done using the Winsock dll */
		WSACleanup();

		return MOONG::NETWORK::RETURN::FAILURE::PING; // 통신 실패.
	}
}

int MOONG::NETWORK::Network::getHostByName(const std::string host_name, struct hostent **remote_host)
{
	WSADATA wsaData;
	int iResult = 0;

	DWORD dwError = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		//printf("WSAStartup failed[d]\n", iResult);
		return MOONG::NETWORK::RETURN::FAILURE::WSASTARTUP_FAILED;
	}

	std::string host_name_for_func = host_name;
	//printf("host_name_for_func before[%s]\n", host_name_for_func.c_str());
	size_t position = host_name_for_func.find("://");
	if(position != std::string::npos)
	{
		host_name_for_func = host_name_for_func.substr(position + strlen("://"));
	}

	position = host_name_for_func.find(":");
	if(position != std::string::npos)
	{
		host_name_for_func = host_name_for_func.substr(0, position);
	}

	position = host_name_for_func.find("/");
	if(position != std::string::npos)
	{
		host_name_for_func = host_name_for_func.substr(0, position);
	}
	//printf("host_name_for_func after[%s]\n", host_name_for_func.c_str());

	*remote_host = gethostbyname(host_name_for_func.c_str());

	if (*remote_host == NULL)
	{
		dwError = WSAGetLastError();

		if (dwError != 0)
		{
			if (dwError == WSAHOST_NOT_FOUND)
			{
				return WSAHOST_NOT_FOUND;
			}
			else if (dwError == WSANO_DATA)
			{
				return WSANO_DATA;
			}
			else
			{
				printf("Function failed with error[%ld]\n", dwError);
				return dwError;
			}
		}
	}

	return MOONG::NETWORK::RETURN::SUCCESS;
}
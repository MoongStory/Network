#include "Network.h"

#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

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

#if _MSC_VER > 1200
	struct sockaddr_in address;

	inet_pton(AF_INET, IP.c_str(), &address.sin_addr.s_addr);

	if (address.sin_addr.s_addr == INADDR_NONE)
	{
		return MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM;
	}
#else
	unsigned int addr = inet_addr(IP.c_str());

	if (addr == INADDR_NONE)
	{
		return MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM;
	}

	struct sockaddr_in address;
	address.sin_addr.s_addr = addr;
#endif
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

int MOONG::NETWORK::Network::getAddrInfoFromURL(const std::string url, const std::string port, std::vector<ADDR_INFO> &param_addr_info)
{
	// https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
    //-----------------------------------------
    // Declare and initialize variables
	ADDR_INFO addr_info;

    WSADATA wsaData = {0};
    int iResult = 0;
    INT iRetval = 0;

    DWORD dwRetval = 0;

    int i = 1;
    
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints = {0};

    struct sockaddr_in  *sockaddr_ipv4;
//    struct sockaddr_in6 *sockaddr_ipv6;
    LPSOCKADDR sockaddr_ip = NULL;

    wchar_t ipstringbuffer[46] = {0};
    DWORD ipbufferlength = 46;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
	{
        printf("WSAStartup failed[%d]\n", iResult);

        return 1;
    }

    //--------------------------------
    // Setup the hints address info structure
    // which is passed to the getaddrinfo() function
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

	std::string url_for_func = url;
	//printf("url_for_func before[%s]\n", url_for_func.c_str());
	size_t position = url_for_func.find("://");
	if(position != std::string::npos)
	{
		url_for_func = url_for_func.substr(position + strlen("://"));
	}

	position = url_for_func.find(":");
	if(position != std::string::npos)
	{
		url_for_func = url_for_func.substr(0, position);
	}

	position = url_for_func.find("/");
	if(position != std::string::npos)
	{
		url_for_func = url_for_func.substr(0, position);
	}
	//printf("url_for_func after[%s]\n", url_for_func.c_str());
    
	//--------------------------------
	// Call getaddrinfo(). If the call succeeds,
	// the result variable will hold a linked list
	// of addrinfo structures containing response
	// information
    dwRetval = getaddrinfo(url_for_func.c_str(), port.c_str(), &hints, &result);
    if ( dwRetval != 0 )
	{
        printf("getaddrinfo failed with error[%d]\n", dwRetval);

        WSACleanup();

        return 1;
    }
    
	char IP[64] = {0};

    // Retrieve each address and print out the hex bytes
    for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		addr_info.Clear();
		ZeroMemory(IP, sizeof(IP));

		addr_info.setFlags(ptr->ai_flags);
        switch (ptr->ai_family) {
            case AF_UNSPEC:
				addr_info.setFamily("Unspecified");
                break;
            case AF_INET:
				addr_info.setFamily("AF_INET (IPv4)");
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
#if _MSC_VER > 1200
				inet_ntop(ptr->ai_family, &(sockaddr_ipv4->sin_addr), IP, sizeof(IP));
				addr_info.setIPAddress(IP);
#else
				addr_info.setIPAddress(inet_ntoa(sockaddr_ipv4->sin_addr));
#endif
                break;
            case AF_INET6:
				addr_info.setFamily("AF_INET6 (IPv6)");
                // the InetNtop function is available on Windows Vista and later
                // sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
                // printf("\tIPv6 address %s\n",
                //    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
                
                // We use WSAAddressToString since it is supported on Windows XP and later
                sockaddr_ip = (LPSOCKADDR) ptr->ai_addr;
                // The buffer length is changed by each call to WSAAddresstoString
                // So we need to set it for each iteration through the loop for safety
                ipbufferlength = 46;
                iRetval = WSAAddressToStringW(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, ipstringbuffer, &ipbufferlength );
                if (iRetval)
				{
					char str_temp[64] = {0};
#if _MSC_VER > 1200
					_itoa_s(WSAGetLastError(), str_temp, sizeof(str_temp), 10);
#else
					_itoa(WSAGetLastError(), str_temp, 10);
#endif

					std::string temp = "WSAAddressToString failed with ";
					temp += str_temp;

					addr_info.setIPAddress(temp);
				}
                else    
				{
					if(ipstringbuffer != NULL)
					{
						const size_t new_size = (wcslen(ipstringbuffer) + 1) * 2;
						char* nstring = new char[new_size];

#if _MSC_VER > 1200
						size_t convertedChars = 0;
						wcstombs_s(&convertedChars, nstring, new_size, ipstringbuffer, _TRUNCATE);
#else
						wcstombs(nstring, ipstringbuffer, new_size);
#endif

						addr_info.setIPAddress(nstring);

						delete[] nstring;
					}
				}
                break;
            case AF_NETBIOS:
				addr_info.setFamily("AF_NETBIOS (NetBIOS)");
                break;
			default:
				char str_temp[256] = {0};
#if _MSC_VER > 1200
				_itoa_s(ptr->ai_family, str_temp, sizeof(str_temp), 10);
#else
				_itoa(ptr->ai_family, str_temp, 10);
#endif

				std::string temp = "Other ";
				temp += str_temp;

				addr_info.setFamily(temp);
                break;
        }

        switch (ptr->ai_socktype) {
            case 0:
				addr_info.setSocketType("Unspecified");
                break;
            case SOCK_STREAM:
				addr_info.setSocketType("SOCK_STREAM (stream)");
                break;
            case SOCK_DGRAM:
				addr_info.setSocketType("SOCK_DGRAM (datagram)");
                break;
            case SOCK_RAW:
				addr_info.setSocketType("SOCK_RAW (raw)");
                break;
            case SOCK_RDM:
				addr_info.setSocketType("SOCK_RDM (reliable message datagram)");
                break;
            case SOCK_SEQPACKET:
				addr_info.setSocketType("SOCK_SEQPACKET (pseudo-stream packet)");
                break;
            default:
				char str_temp[256] = {0};
#if _MSC_VER > 1200
				_itoa_s(ptr->ai_socktype, str_temp, sizeof(str_temp), 10);
#else
				_itoa(ptr->ai_socktype, str_temp, 10);
#endif

				std::string temp = "Other ";
				temp += str_temp;
				
				addr_info.setSocketType(temp);
                break;
        }

        switch (ptr->ai_protocol) {
            case 0:
				addr_info.setProtocol("Unspecified");
                break;
            case IPPROTO_TCP:
				addr_info.setProtocol("IPPROTO_TCP (TCP)");
                break;
            case IPPROTO_UDP:
				addr_info.setProtocol("IPPROTO_UDP (UDP)");
                break;
            default:
				char str_temp[256] = {0};
#if _MSC_VER > 1200
				_itoa_s(ptr->ai_protocol, str_temp, sizeof(str_temp), 10);
#else
				_itoa(ptr->ai_protocol, str_temp, 10);
#endif

				std::string temp = "Other ";
				temp += str_temp;

				addr_info.setProtocol(temp);
                break;
        }
		addr_info.setLengthOfThisSockaddr(ptr->ai_addrlen);
		if(ptr->ai_canonname != NULL)
		{
			addr_info.setCanonicalName(ptr->ai_canonname);
		}

		param_addr_info.push_back(addr_info);
    }

    freeaddrinfo(result);

    WSACleanup();

    return MOONG::NETWORK::RETURN::SUCCESS;
}

int MOONG::NETWORK::Network::getAddrInfoFromURL(const std::string url, const unsigned int port, std::vector<ADDR_INFO> &param_addr_info)
{
	char str_temp[64] = {0};
#if _MSC_VER > 1200
	_itoa_s(port, str_temp, sizeof(str_temp), 10);
#else
	_itoa(port, str_temp, 10);
#endif

	return this->getAddrInfoFromURL(url, str_temp, param_addr_info);
}

int MOONG::NETWORK::Network::getPortFromURL(const std::string url)
{
	std::string port = url;

	size_t position = port.find("://");
	if(position != std::string::npos)
	{
		port = port.substr(position + strlen("://"));
	}

	position = port.find("/");
	if(position != std::string::npos)
	{
		port = port.substr(0, position);
	}

	position = port.find(":");
	if(position != std::string::npos)
	{
		port = port.substr(position + 1);
	}
	else
	{
		port = "-1";
	}

	return atoi(port.c_str());
}
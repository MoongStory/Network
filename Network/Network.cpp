#include "Network.h"

#if _MSC_VER >= 1700
	#include <regex>
#endif

#include "../../ConvertDataType/ConvertDataType/ConvertDataType.h"

#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

#include <sstream>

int MOONG::NETWORK::ADDR_INFO::flags_ = 0;
std::string MOONG::NETWORK::ADDR_INFO::family_ = "";
std::string MOONG::NETWORK::ADDR_INFO::ip_address_ = "";
std::string MOONG::NETWORK::ADDR_INFO::socket_type_ = "";
std::string MOONG::NETWORK::ADDR_INFO::protocol_ = "";
size_t MOONG::NETWORK::ADDR_INFO::length_of_this_sockaddr_ = 0;
std::string MOONG::NETWORK::ADDR_INFO::canonical_name_ = "";

bool MOONG::Network::check_connect_internet()
{
	DWORD dwFlag = 0;
	TCHAR szName[256] = { 0 };

#if _MSC_VER > 1200
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0) ? true : false;
#else
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0) ? true : false;
#endif
}

bool MOONG::Network::check_connect_internet(const std::string param_url)
{
	return InternetCheckConnectionA(param_url.c_str(), FLAG_ICC_FORCE_CONNECTION, NULL) ? true : false;
}

bool MOONG::Network::check_connect_tcp(const std::string address, const unsigned int port/* = 80*/, const unsigned int timeout/* = 1*/)
{
	if (MOONG::Network::is_ip_v4_format(address))
	{
		if (MOONG::Network::check_connect_tcp_(address, port, timeout) == MOONG::NETWORK::RETURN::SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		try
		{
			std::vector<MOONG::NETWORK::ADDR_INFO> addr_info;
			if (MOONG::Network::get_addr_Info_from_url(address, port, addr_info) == MOONG::NETWORK::RETURN::SUCCESS)
			{
				for (size_t i = 0; i < addr_info.size(); i++)
				{
					//printf("IP Address[%s]\n", addr_info[i].getIPAddress().c_str());
					if (MOONG::Network::check_connect_tcp_(addr_info[i].get_ip_address(), port, timeout) == MOONG::NETWORK::RETURN::SUCCESS)
					{
						return true;
					}
				}
			}
		}
		catch (const std::exception& exception)
		{
			throw exception;
		}
	}

	return false;
}

int MOONG::Network::get_addr_Info_from_url(const std::string url, const std::string port, std::vector<MOONG::NETWORK::ADDR_INFO> &param_addr_info)
{
	// https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
    //-----------------------------------------
    // Declare and initialize variables
	MOONG::NETWORK::ADDR_INFO addr_info;

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
        //printf("WSAStartup failed[%d]\n", iResult);

		return MOONG::NETWORK::RETURN::FAILURE::WSASTARTUP_FAILED;
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
        //printf("getaddrinfo failed with error[%d]\n", dwRetval);

        WSACleanup();

		return MOONG::NETWORK::RETURN::FAILURE::GETADDRINFO_FAILED;
    }
    
	char IP[64] = {0};

    // Retrieve each address and print out the hex bytes
    for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		addr_info.clear();
		ZeroMemory(IP, sizeof(IP));

		addr_info.set_flags(ptr->ai_flags);
        switch (ptr->ai_family) {
            case AF_UNSPEC:
				addr_info.set_family("Unspecified");
                break;
            case AF_INET:
				addr_info.set_family("AF_INET (IPv4)");
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
#if _MSC_VER > 1200
				inet_ntop(ptr->ai_family, &(sockaddr_ipv4->sin_addr), IP, sizeof(IP));
				addr_info.set_ip_address(IP);
#else
				addr_info.set_ip_address(inet_ntoa(sockaddr_ipv4->sin_addr));
#endif
                break;
            case AF_INET6:
				addr_info.set_family("AF_INET6 (IPv6)");
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

					addr_info.set_ip_address(temp);
				}
                else    
				{
					if(ipstringbuffer != NULL)
					{
						try
						{
							addr_info.set_ip_address(MOONG::ConvertDataType::wstring_to_string(ipstringbuffer));
						}
						catch (const std::exception& exception)
						{
							throw exception;
						}
					}
				}
                break;
            case AF_NETBIOS:
				addr_info.set_family("AF_NETBIOS (NetBIOS)");
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

				addr_info.set_family(temp);
                break;
        }

        switch (ptr->ai_socktype) {
            case 0:
				addr_info.set_socket_type("Unspecified");
                break;
            case SOCK_STREAM:
				addr_info.set_socket_type("SOCK_STREAM (stream)");
                break;
            case SOCK_DGRAM:
				addr_info.set_socket_type("SOCK_DGRAM (datagram)");
                break;
            case SOCK_RAW:
				addr_info.set_socket_type("SOCK_RAW (raw)");
                break;
            case SOCK_RDM:
				addr_info.set_socket_type("SOCK_RDM (reliable message datagram)");
                break;
            case SOCK_SEQPACKET:
				addr_info.set_socket_type("SOCK_SEQPACKET (pseudo-stream packet)");
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
				
				addr_info.set_socket_type(temp);

                break;
        }

        switch (ptr->ai_protocol) {
            case 0:
				addr_info.set_protocol("Unspecified");
                break;
            case IPPROTO_TCP:
				addr_info.set_protocol("IPPROTO_TCP (TCP)");
                break;
            case IPPROTO_UDP:
				addr_info.set_protocol("IPPROTO_UDP (UDP)");
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

				addr_info.set_protocol(temp);
                break;
        }
		addr_info.set_length_of_this_sock_addr(ptr->ai_addrlen);
		if(ptr->ai_canonname != NULL)
		{
			addr_info.set_canonical_name(ptr->ai_canonname);
		}

		param_addr_info.push_back(addr_info);
    }

    freeaddrinfo(result);

    WSACleanup();

    return MOONG::NETWORK::RETURN::SUCCESS;
}

int MOONG::Network::get_addr_Info_from_url(const std::string url, const unsigned int port, std::vector<MOONG::NETWORK::ADDR_INFO> &param_addr_info)
{
	char str_temp[64] = {0};
#if _MSC_VER > 1200
	_itoa_s(port, str_temp, sizeof(str_temp), 10);
#else
	_itoa(port, str_temp, 10);
#endif

	try
	{
		return MOONG::Network::get_addr_Info_from_url(url, str_temp, param_addr_info);
	}
	catch (const std::exception& exception)
	{
		throw exception;
	}
}





bool MOONG::Network::is_ip_v4_format(const std::string IP)
{
#if _MSC_VER >= 1700
	std::regex ip_v4_format("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

	if (std::regex_match(IP, ip_v4_format))
	{
		return true;;
	}
	else
	{
		return false;
	}
#else
	std::vector<std::string> separated_IP;
	const std::string delimit = ".";

	char* token = NULL;
#if _MSC_VER > 1200
	char* next_token = NULL;

	token = strtok_s((char*)(IP.c_str()), delimit.c_str(), &next_token);

	while (token != NULL)
	{
		// Get next token:
		if (token != NULL)
		{
			separated_IP.push_back(token);

			token = strtok_s(NULL, delimit.c_str(), &next_token);
		}
	}
#else
	token = strtok((char*)(IP.c_str()), delimit.c_str());

	while (token != NULL)
	{
		separated_IP.push_back(token);

		// Get next token:
		token = strtok(NULL, delimit.c_str()); // C4996
	}
#endif

	if (separated_IP.size() == 4)
	{
		int temp_IP = 0;

		for (size_t i = 0; i < separated_IP.size(); i++)
		{
			if(separated_IP[i].length() > 3)
			{
				return false;
			}

			temp_IP = atoi(separated_IP[i].c_str());

			if (temp_IP == 0)
			{
				for (size_t j = 0; separated_IP[i].length(); j++)
				{
					if (separated_IP[i].at(j) < 0 || separated_IP[i].at(j) > 9)
					{
						return false;
					}
				}
			}

			if (temp_IP < 0 || temp_IP > 255)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
#endif
}

int MOONG::Network::check_connect_tcp_(const std::string IP, const unsigned int port/* = 80*/, const unsigned int param_timeout/* = 1*/)
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
	if (sock == INVALID_SOCKET)
	{
		WSACleanup();

		return MOONG::NETWORK::RETURN::FAILURE::SOCKET_FUNCTION_CALL;
	}

#if _MSC_VER > 1200
	struct sockaddr_in socket_address;

	inet_pton(AF_INET, IP.c_str(), &socket_address.sin_addr.s_addr);

	if (socket_address.sin_addr.s_addr == INADDR_NONE)
	{
		WSACleanup();

		return MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM;
	}
#else
	unsigned int addr = inet_addr(IP.c_str());

	if (addr == INADDR_NONE)
	{
		WSACleanup();

		return MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM;
	}

	struct sockaddr_in socket_address;
	socket_address.sin_addr.s_addr = addr;
#endif
	socket_address.sin_port = htons(port);
	socket_address.sin_family = AF_INET;

	// set the socket in non-blocking
	unsigned long iMode = 1;
	int return_value = ioctlsocket(sock, FIONBIO, &iMode);
	if (return_value != NO_ERROR)
	{
		//printf("ioctlsocket set non-block failed with error[%d]\n", return_value);
	}

	if (connect(sock, (struct sockaddr*)&socket_address, sizeof(socket_address)) == false)
	{
		WSACleanup();

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
	if (FD_ISSET(sock, &Write))
	{
		/* then call WSACleanup when done using the Winsock dll */
		WSACleanup();

		closesocket(sock);

		return MOONG::NETWORK::RETURN::SUCCESS;
	}
	else
	{
		/* then call WSACleanup when done using the Winsock dll */
		WSACleanup();

		closesocket(sock);

		return MOONG::NETWORK::RETURN::FAILURE::CONNECT_CHECK; // 통신 실패.
	}
}
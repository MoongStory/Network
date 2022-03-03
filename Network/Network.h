// https://github.com/MoongStory/Network

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include <windows.h>

namespace MOONG
{
	namespace NETWORK
	{
		namespace RETURN
		{
			const int SUCCESS = 0;

			namespace FAILURE
			{
				const int PING = 1;
				const int WSASTARTUP_FAILED = 2;
				const int COULD_NOT_FIND_A_USABLE_VERSION_OF_WINSOCK_DLL = 3;
				const int INVALID_IP_FORM = 4;
				const int SOCKET_CONNECT = 5;
				const int GET_HOST_BY_NAME = 6;
			}
		}

		class Network
		{
		public:
			BOOL InternetConnected() const;
			BOOL InternetConnected(const std::string param_url) const;

			// return
			//		MOONG::NETWORK::RETURN::SUCCESS
			//		MOONG::NETWORK::RETURN::FAILURE::PING
			//		MOONG::NETWORK::RETURN::FAILURE::WSASTARTUP_FAILED
			//		MOONG::NETWORK::RETURN::FAILURE::COULD_NOT_FIND_A_USABLE_VERSION_OF_WINSOCK_DLL
			//		MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM
			//		MOONG::NETWORK::RETURN::FAILURE::SOCKET_CONNECT
			int Ping(const std::string IP, const unsigned int port = 80, const unsigned int param_timeout = 1);

			// return
			//		MOONG::NETWORK::RETURN::FAILURE::WSASTARTUP_FAILED
			//		WSAHOST_NOT_FOUND
			//		WSANO_DATA
			//		WSAGetLastError()
			// param
			//		host_name
			//			URL.
			//		remote_host
			//			¹ÝÈ¯.
			int getHostByName(const std::string host_name, struct hostent **remote_host);

		protected:
		private:
		};
	}
}

#endif _NETWORK_H_
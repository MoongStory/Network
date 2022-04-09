// https://github.com/MoongStory/Network

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

#include <iostream>
#include <vector>
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
				const int SOCKET_FUNCTION_CALL = 7;
			}
		}

		class ADDR_INFO
		{
		public:
			ADDR_INFO()
			{
				this->flags_ = 0;
				this->family_.erase();
				this->ip_address_.erase();
				this->socket_type_.erase();
				this->protocol_.erase();
				this->length_of_this_sockaddr_ = 0;
				this->canonical_name_.erase();
			}

			void Clear()
			{
				this->flags_ = 0;
				this->family_.erase();
				this->ip_address_.erase();
				this->socket_type_.erase();
				this->protocol_.erase();
				this->length_of_this_sockaddr_ = 0;
				this->canonical_name_.erase();
			}

			int getFlags()
			{
				return this->flags_;
			}
			void setFlags(const int flags)
			{
				this->flags_ = flags;
			}

			std::string getFamily()
			{
				return this->family_;
			}
			void setFamily(std::string family)
			{
				this->family_ = family;
			}

			std::string getIPAddress()
			{
				return this->ip_address_;
			}
			void setIPAddress(std::string ip_address)
			{
				this->ip_address_ = ip_address;
			}

			std::string getSocketType()
			{
				return this->socket_type_;
			}
			void setSocketType(std::string socket_type)
			{
				this->socket_type_ = socket_type;
			}

			std::string getProtocol()
			{
				return this->protocol_;
			}
			void setProtocol(std::string protocol)
			{
				this->protocol_ = protocol;
			}

			size_t getLengthOfThisSockaddr()
			{
				return this->length_of_this_sockaddr_;
			}
			void setLengthOfThisSockaddr(const size_t length_of_this_sockaddr)
			{
				this->length_of_this_sockaddr_ = length_of_this_sockaddr;
			}

			std::string getCanonicalName()
			{
				return this->canonical_name_;
			}
			void setCanonicalName(std::string canonical_name)
			{
				this->canonical_name_ = canonical_name;
			}
		private:
			int flags_;
			std::string family_;
			std::string ip_address_;
			std::string socket_type_;
			std::string protocol_;
			size_t length_of_this_sockaddr_;
			std::string canonical_name_;
		};

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
			int Ping(const std::string address, const unsigned int port = 80, const unsigned int param_timeout = 1);

			// return
			//		MOONG::NETWORK::RETURN::SUCCESS
			// param
			//		host_name
			//			URL.
			//		port
			//		param_addr_info
			//			반환.
			int getAddrInfoFromURL(const std::string url, const std::string port, std::vector<ADDR_INFO> &param_addr_info);
			int getAddrInfoFromURL(const std::string url, const unsigned int port, std::vector<ADDR_INFO> &param_addr_info);

			// return
			//		포트 번호.
			//		-1 : URL에 포트가 포함되어 있지 않은 경우.
			// param
			//		url
			int getPortFromURL(const std::string url);

		protected:
		private:
		};
	}
}

#endif _NETWORK_H_
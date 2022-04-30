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
				static const int PING = 1;
				static const int WSASTARTUP_FAILED = 2;
				static const int COULD_NOT_FIND_A_USABLE_VERSION_OF_WINSOCK_DLL = 3;
				static const int INVALID_IP_FORM = 4;
				static const int SOCKET_CONNECT = 5;
				static const int GET_HOST_BY_NAME = 6;
				static const int SOCKET_FUNCTION_CALL = 7;
			}
		}
		
		class ADDR_INFO
		{
		public:
		protected:
		private:
			static int flags_;
			static std::string family_;
			static std::string ip_address_;
			static std::string socket_type_;
			static std::string protocol_;
			static size_t length_of_this_sockaddr_;
			static std::string canonical_name_;
			
		public:
			void Clear()
			{
				MOONG::NETWORK::ADDR_INFO::flags_ = 0;
				MOONG::NETWORK::ADDR_INFO::family_.erase();
				MOONG::NETWORK::ADDR_INFO::ip_address_.erase();
				MOONG::NETWORK::ADDR_INFO::socket_type_.erase();
				MOONG::NETWORK::ADDR_INFO::protocol_.erase();
				MOONG::NETWORK::ADDR_INFO::length_of_this_sockaddr_ = 0;
				MOONG::NETWORK::ADDR_INFO::canonical_name_.erase();
			}
			
			int getFlags()
			{
				return MOONG::NETWORK::ADDR_INFO::flags_;
			}
			void setFlags(const int flags)
			{
				MOONG::NETWORK::ADDR_INFO::flags_ = flags;
			}
			
			std::string getFamily()
			{
				return MOONG::NETWORK::ADDR_INFO::family_;
			}
			void setFamily(std::string family)
			{
				MOONG::NETWORK::ADDR_INFO::family_ = family;
			}
			
			std::string getIPAddress()
			{
				return MOONG::NETWORK::ADDR_INFO::ip_address_;
			}
			void setIPAddress(std::string ip_address)
			{
				MOONG::NETWORK::ADDR_INFO::ip_address_ = ip_address;
			}
			
			std::string getSocketType()
			{
				return MOONG::NETWORK::ADDR_INFO::socket_type_;
			}
			void setSocketType(std::string socket_type)
			{
				MOONG::NETWORK::ADDR_INFO::socket_type_ = socket_type;
			}
			
			std::string getProtocol()
			{
				return MOONG::NETWORK::ADDR_INFO::protocol_;
			}
			void setProtocol(std::string protocol)
			{
				MOONG::NETWORK::ADDR_INFO::protocol_ = protocol;
			}
			
			size_t getLengthOfThisSockaddr()
			{
				return MOONG::NETWORK::ADDR_INFO::length_of_this_sockaddr_;
			}
			void setLengthOfThisSockaddr(const size_t length_of_this_sockaddr)
			{
				MOONG::NETWORK::ADDR_INFO::length_of_this_sockaddr_ = length_of_this_sockaddr;
			}
			
			std::string getCanonicalName()
			{
				return MOONG::NETWORK::ADDR_INFO::canonical_name_;
			}
			void setCanonicalName(std::string canonical_name)
			{
				MOONG::NETWORK::ADDR_INFO::canonical_name_ = canonical_name;
			}
		protected:
		private:
		};
	}
	
	class Network
	{
	public:
		static bool InternetConnected();
		static bool InternetConnected(const std::string param_url);
		
		static bool Ping(const std::string address, const unsigned int port = 80, const unsigned int timeout = 1);
		
		// return
		//		MOONG::NETWORK::RETURN::SUCCESS
		// param
		//		host_name
		//			URL.
		//		port
		//		param_addr_info
		//			반환.
		static int getAddrInfoFromURL(const std::string url, const std::string port, std::vector<MOONG::NETWORK::ADDR_INFO> &param_addr_info);
		static int getAddrInfoFromURL(const std::string url, const unsigned int port, std::vector<MOONG::NETWORK::ADDR_INFO> &param_addr_info);
		
		// return
		//		포트 번호.
		//		-1 : URL에 포트가 포함되어 있지 않은 경우.
		// param
		//		url
		static int getPortFromURL(const std::string url);
		
	protected:
	private:
		static bool Is_IPv4_Format_(const std::string IP);
		
		// return
		//		MOONG::NETWORK::RETURN::SUCCESS
		//		MOONG::NETWORK::RETURN::FAILURE::PING
		//		MOONG::NETWORK::RETURN::FAILURE::WSASTARTUP_FAILED
		//		MOONG::NETWORK::RETURN::FAILURE::COULD_NOT_FIND_A_USABLE_VERSION_OF_WINSOCK_DLL
		//		MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM
		//		MOONG::NETWORK::RETURN::FAILURE::SOCKET_CONNECT
		//		MOONG::NETWORK::RETURN::FAILURE::SOCKET_FUNCTION_CALL
		static int Ping_(const std::string IP, const unsigned int port = 80, const unsigned int param_timeout = 1);
	};
}

#endif _NETWORK_H_
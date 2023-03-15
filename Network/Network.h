/*******************************************************************************
MIT License

Copyright (c) 2023 Moong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

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
				static const int CONNECT_CHECK = 1;
				static const int WSASTARTUP_FAILED = 2;
				static const int COULD_NOT_FIND_A_USABLE_VERSION_OF_WINSOCK_DLL = 3;
				static const int INVALID_IP_FORM = 4;
				static const int SOCKET_CONNECT = 5;
				static const int SOCKET_FUNCTION_CALL = 6;
				static const int GETADDRINFO_FAILED = 7;
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
			void clear()
			{
				MOONG::NETWORK::ADDR_INFO::flags_ = 0;
				MOONG::NETWORK::ADDR_INFO::family_.erase();
				MOONG::NETWORK::ADDR_INFO::ip_address_.erase();
				MOONG::NETWORK::ADDR_INFO::socket_type_.erase();
				MOONG::NETWORK::ADDR_INFO::protocol_.erase();
				MOONG::NETWORK::ADDR_INFO::length_of_this_sockaddr_ = 0;
				MOONG::NETWORK::ADDR_INFO::canonical_name_.erase();
			}

			int get_flags()
			{
				return MOONG::NETWORK::ADDR_INFO::flags_;
			}
			void set_flags(const int flags)
			{
				MOONG::NETWORK::ADDR_INFO::flags_ = flags;
			}

			std::string get_family()
			{
				return MOONG::NETWORK::ADDR_INFO::family_;
			}
			void set_family(std::string family)
			{
				MOONG::NETWORK::ADDR_INFO::family_ = family;
			}

			std::string get_ip_address()
			{
				return MOONG::NETWORK::ADDR_INFO::ip_address_;
			}
			void set_ip_address(std::string ip_address)
			{
				MOONG::NETWORK::ADDR_INFO::ip_address_ = ip_address;
			}

			std::string get_socket_type()
			{
				return MOONG::NETWORK::ADDR_INFO::socket_type_;
			}
			void set_socket_type(std::string socket_type)
			{
				MOONG::NETWORK::ADDR_INFO::socket_type_ = socket_type;
			}

			std::string get_protocol()
			{
				return MOONG::NETWORK::ADDR_INFO::protocol_;
			}
			void set_protocol(std::string protocol)
			{
				MOONG::NETWORK::ADDR_INFO::protocol_ = protocol;
			}

			size_t get_length_of_this_sock_addr()
			{
				return MOONG::NETWORK::ADDR_INFO::length_of_this_sockaddr_;
			}
			void set_length_of_this_sock_addr(const size_t length_of_this_sockaddr)
			{
				MOONG::NETWORK::ADDR_INFO::length_of_this_sockaddr_ = length_of_this_sockaddr;
			}

			std::string get_canonical_name()
			{
				return MOONG::NETWORK::ADDR_INFO::canonical_name_;
			}
			void set_canonical_name(std::string canonical_name)
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
		static bool check_connect_internet();
		static bool check_connect_internet(const std::string param_url);

		static bool check_connect_tcp(const std::string address, const unsigned int port = 80, const unsigned int timeout = 1);

		// return
		//		MOONG::NETWORK::RETURN::SUCCESS
		// param
		//		host_name
		//			URL.
		//		port
		//		param_addr_info
		//			¹ÝÈ¯.
		static int get_addr_Info_from_url(const std::string url, const std::string port, std::vector<MOONG::NETWORK::ADDR_INFO>& param_addr_info);
		static int get_addr_Info_from_url(const std::string url, const unsigned int port, std::vector<MOONG::NETWORK::ADDR_INFO>& param_addr_info);

		static bool is_ip_v4_format(const std::string IP);
	protected:
	private:

		// return
		//		MOONG::NETWORK::RETURN::SUCCESS
		//		MOONG::NETWORK::RETURN::FAILURE::CONNECT_CHECK
		//		MOONG::NETWORK::RETURN::FAILURE::WSASTARTUP_FAILED
		//		MOONG::NETWORK::RETURN::FAILURE::COULD_NOT_FIND_A_USABLE_VERSION_OF_WINSOCK_DLL
		//		MOONG::NETWORK::RETURN::FAILURE::INVALID_IP_FORM
		//		MOONG::NETWORK::RETURN::FAILURE::SOCKET_CONNECT
		//		MOONG::NETWORK::RETURN::FAILURE::SOCKET_FUNCTION_CALL
		static int check_connect_tcp_(const std::string IP, const unsigned int port = 80, const unsigned int param_timeout = 1);
	};
}

#endif _NETWORK_H_
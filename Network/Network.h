// https://github.com/MoongStory/Network

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _NETWORK_H_
#define _NETWORK_H_

#if _MSC_VER > 1200
#include <atlstr.h>
#else
#include <iostream>
#endif

namespace MOONG
{
	namespace NETWORK
	{
		namespace RETURN
		{
			const int SUCCESS = 0;

			namespace FAILURE
			{
				const int CREATE_PIPE = 1;
				const int CREATE_PROCESS = 2;
				const int PING = 3;
			}
		}

		class Network
		{
		public:
#if _MSC_VER > 1200
			BOOL InternetConnected() const;
			BOOL InternetConnected(const CStringA param_url) const;
			// return
			// RETURN::SUCCESS
			// RETURN::FAILURE::CREATE_PIPE
			// RETURN::FAILURE::CREATE_PROCESS
			// RETURN::FAILURE::PING
			int Ping(const CStringA IP) const;
#else
			bool InternetConnected() const;
			bool InternetConnected(const std::string param_url) const;
			// return
			// RETURN::SUCCESS
			// RETURN::FAILURE::CREATE_PIPE
			// RETURN::FAILURE::CREATE_PROCESS
			// RETURN::FAILURE::PING
			int Ping(const std::string IP) const;
#endif
		protected:
		private:
		};
	}
}

#endif _NETWORK_H_
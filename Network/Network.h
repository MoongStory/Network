// https://github.com/MoongStory/Network

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include <Windows.h>

namespace MOONG
{
	namespace NETWORK
	{
		class Network
		{
		public:
			BOOL InternetConnected();
			// TODO: std::string을 CStringA로 변경.
			BOOL InternetConnected(std::string param_url);
			BOOL Ping(std::string ip);
		protected:
		private:
		};
	}
}

#endif _NETWORK_H_
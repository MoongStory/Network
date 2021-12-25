// https://github.com/MoongStory/Network

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <atlstr.h>

namespace MOONG
{
	namespace NETWORK
	{
		class Network
		{
		public:
			BOOL InternetConnected() const;
			BOOL InternetConnected(CStringA param_url) const;
			BOOL Ping(CStringA IP) const;
		protected:
		private:
		};
	}
}

#endif _NETWORK_H_
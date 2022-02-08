#include "Network.h"

#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

#include <stdlib.h>

BOOL MOONG::NETWORK::Network::InternetConnected() const
{
	DWORD dwFlag = 0;
	TCHAR szName[256] = {0};
	
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0);
}

BOOL MOONG::NETWORK::Network::InternetConnected(CStringA param_url) const
{
#ifdef _UNICODE
	const size_t new_size_w = strlen(param_url.GetString()) + 1;
	size_t converted_chars = 0;
	wchar_t* url = new wchar_t[new_size_w];
	mbstowcs_s(&converted_chars, url, new_size_w, param_url.GetString(), _TRUNCATE);
#elif _MBCS
	const size_t new_size = (strlen(param_url.c_str()) + 1) * 2;
	char* url = new char[new_size];
	strcpy_s(url, new_size, param_url.c_str());
#endif

	return InternetCheckConnection(url, FLAG_ICC_FORCE_CONNECTION, NULL);
}

BOOL MOONG::NETWORK::Network::Ping(const CStringA IP) const
{
	CStringA command = "ping -n 1 " + IP;
	
 	FILE* file = _popen(command.GetString(), "r");

	char output[300] = {0};
 	fread(output, 1, sizeof(output), file);

	//printf("%s\n", output);

	fclose(file);

// Case 0 (성공)
// Ping 172.20.25.130 32바이트 데이터 사용:
// 172.20.25.130의 응답: 바이트=32 시간<1ms TTL=63

// 172.20.25.130에 대한 Ping 통계:
//     패킷: 보냄 = 1, 받음 = 1, 손실 = 0 (0% 손실),
// 왕복 시간(밀리초):
//     최소 = 0ms, 최대 = 0ms, 평균 = 0ms



// Case 1 (실패)
// Ping 172.20.25.130 32바이트 데이터 사용:
// 172.20.25.30의 응답: 대상 호스트에 연결할 수 없습니다.

// 172.20.25.130에 대한 Ping 통계:
//     패킷: 보냄 = 1, 받음 = 1, 손실 = 0 (0% 손실),



// Case 2 (실패)
// Ping 172.20.25.130 32바이트 데이터 사용:
// PING: 전송하지 못했습니다. 일반 오류입니다.

// 172.20.25.130에 대한 Ping 통계:
//     패킷: 보냄 = 1, 받음 = 0, 손실 = 1 (100% 손실),

	CStringA response_msg = IP + "의 응답: 바이트=";

	if(strstr(output, response_msg.GetString()))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
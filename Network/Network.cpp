#include "Network.h"

#include <windows.h>
#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

#include <stdlib.h>

#include <stdexcept>

#if _MSC_VER > 1200
BOOL MOONG::NETWORK::Network::InternetConnected() const
#else
bool MOONG::NETWORK::Network::InternetConnected() const
#endif
{
	DWORD dwFlag = 0;
	TCHAR szName[256] = { 0 };

#if _MSC_VER > 1200
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0);
#else
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0) ? true : false;
#endif
}

#if _MSC_VER > 1200
BOOL MOONG::NETWORK::Network::InternetConnected(const CStringA param_url) const
#else
bool MOONG::NETWORK::Network::InternetConnected(const std::string param_url) const
#endif
{
#if _MSC_VER > 1200
	return InternetCheckConnectionA(param_url.GetString(), FLAG_ICC_FORCE_CONNECTION, NULL);
#else
	return InternetCheckConnectionA(param_url.c_str(), FLAG_ICC_FORCE_CONNECTION, NULL) ? true : false;
#endif
}

#if _MSC_VER > 1200
int MOONG::NETWORK::Network::Ping(const CStringA IP) const
#else
int MOONG::NETWORK::Network::Ping(const std::string IP) const
#endif
{
#if _MSC_VER > 1200
	CStringA command = "ping -n 1 " + IP;
#else
	std::string command = "ping -n 1 " + IP;
#endif

	std::string strResult;
	
#if _MSC_VER > 1200
	ExecCommand(command.GetString(), strResult);
#else
	ExecCommand(command, strResult);
#endif

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

#if _MSC_VER > 1200
	CStringA response_msg = IP + "의 응답: 바이트=";
#else
	std::string response_msg = IP + "의 응답: 바이트=";
#endif

#if _MSC_VER > 1200
	if (strstr(strResult.c_str(), response_msg.GetString()))
#else
	if (strstr(strResult.c_str(), response_msg.c_str()))
#endif
	{
		return MOONG::NETWORK::RETURN::SUCCESS;
	}
	else
	{
		return MOONG::NETWORK::RETURN::FAILURE::PING;
	}
}

int MOONG::NETWORK::Network::ExecCommand(const std::string command, std::string& output) const
{
	HANDLE hPipeRead, hPipeWrite;

	SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
	saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe to get results from child's stdout.
	if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
	{
		return MOONG::NETWORK::RETURN::FAILURE::CREATE_PIPE;
	}

	STARTUPINFOA si = { sizeof(STARTUPINFOA) };
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = hPipeWrite;
	si.hStdError = hPipeWrite;
	si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
	// Requires STARTF_USESHOWWINDOW in dwFlags.

	PROCESS_INFORMATION pi = { 0 };

	BOOL fSuccess = CreateProcessA(NULL, (char*)(command.c_str()), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	if (!fSuccess)
	{
		CloseHandle(hPipeWrite);
		CloseHandle(hPipeRead);

		return MOONG::NETWORK::RETURN::FAILURE::CREATE_PROCESS;
	}

	bool bProcessEnded = false;
	for (; !bProcessEnded;)
	{
		// Give some timeslice (50 ms), so we won't waste 100% CPU.
		bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

		// Even if process exited - we continue reading, if
		// there is some data available over pipe.
		for (;;)
		{
			char buf[1024] = { 0 };
			DWORD dwRead = 0;
			DWORD dwAvail = 0;

			if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
			{
				break;
			}

			if (!dwAvail) // No data available, return
			{
				break;
			}

			if (!::ReadFile(hPipeRead, buf, min(sizeof(buf) - 1, dwAvail), &dwRead, NULL) || !dwRead)
			{
				// Error, the child process might ended
				break;
			}

			buf[dwRead] = 0;
			output += buf;
		}
	}

	CloseHandle(hPipeWrite);
	CloseHandle(hPipeRead);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return EXIT_SUCCESS;
}
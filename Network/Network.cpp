#include "Network.h"

#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

#include <stdlib.h>

#include <stdexcept>

BOOL MOONG::NETWORK::Network::InternetConnected() const
{
	DWORD dwFlag = 0;
	TCHAR szName[256] = { 0 };

#if _MSC_VER > 1200
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0);
#else
	return ::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0) ? true : false;
#endif
}

BOOL MOONG::NETWORK::Network::InternetConnected(const std::string param_url) const
{
	return InternetCheckConnectionA(param_url.c_str(), FLAG_ICC_FORCE_CONNECTION, NULL) ? true : false;
}

int MOONG::NETWORK::Network::Ping(const std::string IP) const
{
	std::string command = "ping -n 1 " + IP;

	std::string strResult;

	int return_value = 0;
	
	return_value = ExecCommand(command, strResult);
	if(return_value != EXIT_SUCCESS)
	{
		return return_value;
	}

	// Case 0 (����)
	// Ping 172.20.25.130 32����Ʈ ������ ���:
	// 172.20.25.130�� ����: ����Ʈ=32 �ð�<1ms TTL=63

	// 172.20.25.130�� ���� Ping ���:
	//     ��Ŷ: ���� = 1, ���� = 1, �ս� = 0 (0% �ս�),
	// �պ� �ð�(�и���):
	//     �ּ� = 0ms, �ִ� = 0ms, ��� = 0ms



	// Case 1 (����)
	// Ping 172.20.25.130 32����Ʈ ������ ���:
	// 172.20.25.30�� ����: ��� ȣ��Ʈ�� ������ �� �����ϴ�.

	// 172.20.25.130�� ���� Ping ���:
	//     ��Ŷ: ���� = 1, ���� = 1, �ս� = 0 (0% �ս�),



	// Case 2 (����)
	// Ping 172.20.25.130 32����Ʈ ������ ���:
	// PING: �������� ���߽��ϴ�. �Ϲ� �����Դϴ�.

	// 172.20.25.130�� ���� Ping ���:
	//     ��Ŷ: ���� = 1, ���� = 0, �ս� = 1 (100% �ս�),

	std::string response_msg = IP + "�� ����: ����Ʈ=";

	if (strResult.find(response_msg) != std::string::npos)
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
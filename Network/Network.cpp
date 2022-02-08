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

	CStringA response_msg = IP + "�� ����: ����Ʈ=";

	if(strstr(output, response_msg.GetString()))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
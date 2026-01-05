#include "Base.h"
#include "WtsUtils.h"
#include <ws2tcpip.h>

NS_BEGIN

//#################################################################################################
void WtsInit(void)
{
	HANDLE hEvent = OpenEventW(SYNCHRONIZE, FALSE, L"Global\\TermSrvReadyEvent");
	if(hEvent)
	{
		WaitForSingleObject(hEvent, INFINITE);
		CloseHandle(hEvent);
	}
}

//#################################################################################################
bool WtsGetCurrentSessionId(uint32_t &nSessionId)
{
	bool bResult = false;

	nSessionId = 0;

	PWSTR pBuffer;
	DWORD dwSize;
	if(WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTSSessionId, &pBuffer, &dwSize))
	{
		Assert(dwSize == sizeof(uint32_t));
		nSessionId = *(uint32_t*)pBuffer;
		WTSFreeMemory(pBuffer);
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool WtsGetDomainName(CStrW &strDomain, const uint32_t nSessionId)
{
	bool bResult = false;

	strDomain.Empty();

	PWSTR sz;
	DWORD dwSize;
	if(WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, nSessionId, WTSDomainName, &sz, &dwSize))
	{
		strDomain = sz;
		WTSFreeMemory(sz);
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool WtsGetUserName(CStrW &strUser, const uint32_t nSessionId)
{
	bool bResult = false;

	strUser.Empty();

	PWSTR sz;
	DWORD dwSize;
	if(WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, nSessionId, WTSUserName, &sz, &dwSize))
	{
		strUser = sz;
		WTSFreeMemory(sz);
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool WtsGetLogonTime(FILETIME &ftLogon, const uint32_t nSessionId)
{
	bool bResult = false;

	ftLogon = FILETIME_NULL;

	PWSTR pBuffer;
	DWORD dwSize;
	if(WTSQuerySessionInformationW(nullptr, nSessionId, WTSSessionInfo, &pBuffer, &dwSize))
	{
		PWTSINFO pwtsi = (PWTSINFO)pBuffer;
		ftLogon.dwHighDateTime = pwtsi->LogonTime.HighPart;
		ftLogon.dwLowDateTime = pwtsi->LogonTime.LowPart;
		WTSFreeMemory(pBuffer);
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool WtsIsRemoteLogon(bool &bRemote, const uint32_t nSessionId)
{
	bool bResult = false;

	bRemote = false;

	PWSTR pBuffer;
	DWORD dwSize;
	if(WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, nSessionId, WTSIsRemoteSession, &pBuffer, &dwSize))
	{
		bRemote = std::any_of((PCBYTE)pBuffer, (PCBYTE)pBuffer + dwSize, [](const BYTE n){return (n != 0);});
		WTSFreeMemory(pBuffer);
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool WtsGetRemoteName(CStrW &strRemote, const uint32_t nSessionId)
{
	bool bResult = false;

	strRemote.Empty();

	PWSTR sz;
	DWORD dwSize;
	if(WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, nSessionId, WTSClientName, &sz, &dwSize))
	{
		strRemote = sz;
		WTSFreeMemory(sz);
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool WtsGetRemoteAddress(CStrW &strAddress, const uint32_t nSessionId)
{
	bool bResult = false;

	strAddress.Empty();

	PWSTR pBuffer;
	DWORD dwSize;
	if(WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, nSessionId, WTSClientAddress, &pBuffer, &dwSize))
	{
		PWTS_CLIENT_ADDRESS pWCA = (PWTS_CLIENT_ADDRESS)pBuffer;
		if(pWCA->AddressFamily == AF_INET)
		{
			size_t nLength = 16;
			auto szIP = std::make_unique<char[]>(nLength);
			if(inet_ntop(AF_INET, pWCA->Address + 2, szIP.get(), nLength))
			{
				strAddress = szIP.get();
				bResult = true;
			}
		}
		else if(pWCA->AddressFamily == AF_INET6)
		{
			size_t nLength = 46;
			auto szIP = std::make_unique<char[]>(nLength);
			if(inet_ntop(AF_INET6, pWCA->Address, szIP.get(), nLength))
			{
				strAddress = szIP.get();
				bResult = true;
			}
		}

		WTSFreeMemory(pBuffer);
	}

	return bResult;
}

NS_END

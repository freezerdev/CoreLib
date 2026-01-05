#pragma once

#ifdef __APPLE__
#include "MemBuffer.h"
#include <CoreFoundation/CoreFoundation.h>
#include <sys/sysctl.h>
#endif

NS_BEGIN

// Platform specific utilities

#ifdef _WIN32
CStr GetUsername(const uint32_t nSessionId);
CStr SidToAccountName(const CStr &strSid);
bool IsValidSidW(const CStrW &strSid);

struct ComputerUser final
{
	CStr strUsername;
	CFilePath pathHomeDir;
	CStr strSid;
};
std::vector<ComputerUser> GetComputerUsers(void);

HMODULE SystemLoadLibrary(PCWSTR szModuleName);

bool LoadUserRegistryHive(PCNSTR szKeyName, PCNSTR szHiveFilename);
bool UnloadUserRegistryHive(PCNSTR szKeyName);

HRESULT ComInit(const bool bMultithreaded = true);
HRESULT ComInitEx(const bool bMultithreaded = true, const DWORD dwAuthnLevel = RPC_C_AUTHN_LEVEL_DEFAULT);
void ComFree(void);

bool GetRemoteTZOffset(const CStrW &strMachine, int16_t &nOffset);

#elif __APPLE__

enum EPrivilegeLevel : uint8_t
{
	EPL_Unknown = 0,
	EPL_Denied,
	EPL_Granted
};

EPrivilegeLevel CheckFullDiskAccess(void);

ERRCODE GetIORegistryString(PCSTR szPath, PCSTR szKey, CStr &strValue);
ERRCODE GetIORegistryData(PCSTR szPath, PCSTR szKey, CMemBuffer &bufValue);

bool GetSysctlValue(const int nLevel1, const int nLevel2, CStr &str);
bool GetSysctlValue(PCNSTR szName, CStr &str);

//#################################################################################################
template<typename INTTYPE>
bool GetSysctlValue(const int nLevel1, const int nLevel2, INTTYPE &n)
{
	n = 0;

	int mib[2] = {nLevel1, nLevel2};
	size_t nDataSize = sizeof(n);
	return (sysctl(mib, COUNTOF(mib), &n, &nDataSize, nullptr, 0) == 0);
}

//#################################################################################################
template<typename INTTYPE>
bool GetSysctlValue(PCNSTR szName, INTTYPE &n)
{
	n = 0;

	size_t nDataSize = sizeof(n);
	return (sysctlbyname(szName, &n, &nDataSize, nullptr, 0) == 0);
}
#endif

NS_END

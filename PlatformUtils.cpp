#include "Base.h"
#include "PlatformUtils.h"
#ifdef _WIN32
#include "FilePathUtils.h"
#include "FilePathConverter.h"
#include "FileSystemUtils.h"
#include "PrivilegeManager.h"
#include "RegKey.h"
#include "WtsUtils.h"
#include <lm.h>
#include <sddl.h>
#elif __APPLE__
#include "FileSystemUtils.h"
#include "Object.h"
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#endif

NS_BEGIN

#ifdef _WIN32
//#################################################################################################
CStr GetUsername(const uint32_t nSessionId)
{
	CStr strUsername;

	static CStr strMachineName = GetMachineName();

	CStr strDomain;
	CStr strUser;
	WtsGetDomainName(strDomain, nSessionId);
	WtsGetUserName(strUser, nSessionId);

	if(!strDomain.IsEmpty() && strDomain != _N('.') && strDomain.Compare(strMachineName, true) != 0)
	{
		strUsername = std::move(strDomain);
		strUsername += _N('\\');
	}
	strUsername += strUser;

	return strUsername;
}

//#################################################################################################
CStr SidToAccountName(const CStr &strSid)
{
	Assert(!strSid.IsEmpty());

	CStr strAccountName;

	static CStr strMachineName = GetMachineName();

	PSID pSid = nullptr;
	if(ConvertStringSidToSidW(strSid, &pSid))
	{
		DWORD dwNameSize = 0;
		DWORD dwDomainSize = 0;
		SID_NAME_USE snu;

		LookupAccountSidW(nullptr, pSid, nullptr, &dwNameSize, nullptr, &dwDomainSize, &snu);
		if(dwNameSize && dwDomainSize)
		{
			auto szName = std::make_unique<wchar_t[]>(dwNameSize);
			auto szDomain = std::make_unique<wchar_t[]>(dwDomainSize);

			if(LookupAccountSidW(nullptr, pSid, szName.get(), &dwNameSize, szDomain.get(), &dwDomainSize, &snu))
			{
				strAccountName = szDomain.get();
				if(strAccountName == _N('.') || strAccountName.Compare(strMachineName, true) == 0)
					strAccountName.Empty();
				else if(!strAccountName.IsEmpty())
					strAccountName += L'\\';
				strAccountName += szName.get();
			}
		}

		LocalFree(pSid);
	}

	return strAccountName;
}

//#################################################################################################
bool IsValidSidW(const CStrW &strSid)
{
	bool bValid = false;

	PSID pSid = nullptr;
	if(ConvertStringSidToSidW(strSid, &pSid))
	{
		bValid = (IsValidSid(pSid) != FALSE);
		LocalFree(pSid);
	}

	return bValid;
}

//#################################################################################################
std::vector<ComputerUser> GetComputerUsers(void)
{
	std::vector<ComputerUser> vecUsers;

	// APIs like NetQueryDisplayInformation() will only return local accounts and not domain users that have logged in locally
	// Instead grab the list of profiles from the registry
	CFilePathConverter fpc;
	fpc.Init();

	ComputerUser cu;
	CRegKey keyProfileList;
	if(keyProfileList.Open(HKEY_LOCAL_MACHINE, _N("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList")) == FW_NO_ERROR)
	{
		CRegKey keyProfile;
		CStr str;
		CRegBuf regbuf;
		DWORD dwIndex = 0;
		while(keyProfileList.EnumKey(regbuf, dwIndex) == FW_NO_ERROR)
		{
			cu.strSid = regbuf;
			if(cu.strSid.CompareLeft(_N("S-1-5-21-"), 9, true) == 0 && IsValidSidW(cu.strSid))
			{	// Found a user SID
				if(keyProfile.Open(keyProfileList, cu.strSid) == FW_NO_ERROR)
				{
					keyProfile.GetValue(_N("ProfileImagePath"), str);
					keyProfile.Close();
					cu.pathHomeDir = str;
					fpc.Resolve(cu.pathHomeDir);

					cu.strUsername = SidToAccountName(cu.strSid);
					if(!cu.strUsername.IsEmpty())
						vecUsers.push_back(std::move(cu));
				}
			}
		}

		keyProfileList.Close();
	}

	return vecUsers;
}

//#################################################################################################
HMODULE SystemLoadLibrary(PCWSTR szModuleName)
{	// A process running as SYSTEM should never load a library with a relative path
	static CFilePath pathSystem = GetSystemFolder();
	return LoadLibraryW((pathSystem + CFilePathSegment(szModuleName)).Get());
}

//#################################################################################################
bool LoadUserRegistryHive(PCNSTR szKeyName, PCNSTR szHiveFilename)
{
	bool bLoaded = false;

	if(FileExists(szHiveFilename))
	{	// Spend the least amount of time inside a privileged state
		CPrivilegeManager pm;
		pm.Grant(SE_BACKUP_NAME);
		pm.Grant(SE_RESTORE_NAME);

		if(RegLoadKeyW(HKEY_USERS, szKeyName, szHiveFilename) == ERROR_SUCCESS)
			bLoaded = true;
	}

	return bLoaded;
}

//#################################################################################################
bool UnloadUserRegistryHive(PCNSTR szKeyName)
{
	bool bUnloaded = false;

	// Unload registry hive
	// Spend the least amount of time inside a privileged state
	CPrivilegeManager pm;
	pm.Grant(SE_BACKUP_NAME);
	pm.Grant(SE_RESTORE_NAME);

	if(RegUnLoadKeyW(HKEY_USERS, szKeyName) == ERROR_SUCCESS)
		bUnloaded = true;

	return bUnloaded;
}

//#################################################################################################
HRESULT ComInit(const bool bMultithreaded)
{
	return CoInitializeEx(nullptr, bMultithreaded ? COINIT_MULTITHREADED : COINIT_APARTMENTTHREADED);
}

//#################################################################################################
HRESULT ComInitEx(const bool bMultithreaded, const DWORD dwAuthnLevel)
{
	HRESULT hr = CoInitializeEx(nullptr, bMultithreaded ? COINIT_MULTITHREADED : COINIT_APARTMENTTHREADED);
	if(SUCCEEDED(hr))
		hr = CoInitializeSecurity(nullptr, -1, nullptr, nullptr, dwAuthnLevel, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);

	return hr;
}

//#################################################################################################
void ComFree(void)
{
	CoUninitialize();
}

//#################################################################################################
bool GetRemoteTZOffset(const CStrW &strMachine, int16_t &nOffset)
{
	bool bReturn = false;

	nOffset = 0;

	// NetRemoteTOD performs a network call which may block for several seconds
	PWSTR pBuffer = nullptr;
	if(NetRemoteTOD(strMachine, (PBYTE*)&pBuffer) == NERR_Success)
	{
		PTIME_OF_DAY_INFO pToDI = (PTIME_OF_DAY_INFO)pBuffer;
		if(pToDI->tod_timezone != -1)
		{	// The returned timezone offset is the opposite sign from normal
			nOffset = -(int16_t)pToDI->tod_timezone;
			bReturn = true;
		}

		NetApiBufferFree(pBuffer);
	}

	return bReturn;
}

#elif __APPLE__

//#################################################################################################
EPrivilegeLevel CheckFullDiskAccess(void)
{
	EPrivilegeLevel eLevel = EPL_Unknown;

	// The Transparency Consent and Control database is a protected file, can we access it?
	int nResult = access("/Library/Application Support/com.apple.TCC/TCC.db", R_OK);
	if(nResult == -1 && errno == EPERM)
		eLevel = EPL_Denied;
	else if(nResult == 0)
		eLevel = EPL_Granted;

	return eLevel;
}

//#################################################################################################
ERRCODE GetIORegistryString(PCSTR szPath, PCSTR szKey, CStr &strValue)
{
	ERRCODE nErrorCode = FW_ERROR_NOT_FOUND;

	strValue.Empty();

	CIOObject ioReg;
	if(szPath)
		ioReg = IORegistryEntryFromPath(kIOMasterPortDefault, szPath);
	else
		ioReg = IORegistryGetRootEntry(kIOMasterPortDefault);

	if(ioReg)
	{
		CCFObject cfKey = CFStringCreateWithCString(kCFAllocatorDefault, szKey, kCFStringEncodingUTF8);
		if(cfKey)
		{
			CCFObject cfProp = IORegistryEntryCreateCFProperty(ioReg, (CFStringRef)cfKey.Get(), kCFAllocatorDefault, 0);
			if(cfProp.GetType() == CFStringGetTypeID())
			{
				strValue = cfProp.GetString();
				nErrorCode = FW_NO_ERROR;
			}
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE GetIORegistryData(PCSTR szPath, PCSTR szKey, CMemBuffer &bufValue)
{
	ERRCODE nErrorCode = FW_ERROR_NOT_FOUND;

	bufValue.Empty();

	CIOObject ioReg;
	if(szPath)
		ioReg = IORegistryEntryFromPath(kIOMasterPortDefault, szPath);
	else
		ioReg = IORegistryGetRootEntry(kIOMasterPortDefault);

	if(ioReg)
	{
		CCFObject cfKey = CFStringCreateWithCString(kCFAllocatorDefault, szKey, kCFStringEncodingUTF8);
		if(cfKey)
		{
			CCFObject cfProp = IORegistryEntryCreateCFProperty(ioReg, (CFStringRef)cfKey.Get(), kCFAllocatorDefault, 0);
			if(cfProp)
			{
				if(cfProp.GetType() == CFDataGetTypeID())
				{
					bufValue = cfProp.GetData();
					nErrorCode = FW_NO_ERROR;
				}
			}
		}
	}

	return nErrorCode;
}

//#################################################################################################
bool GetSysctlValue(const int nLevel1, const int nLevel2, CStr &str)
{
	bool bReturn = false;

	str.Empty();

	int mib[2] = {nLevel1, nLevel2};
	size_t nDataSize = 0;
	if(sysctl(mib, COUNTOF(mib), nullptr, &nDataSize, nullptr, 0) != -1)
	{
		auto sz = std::make_unique<char[]>(nDataSize);
		if(sysctl(mib, COUNTOF(mib), sz.get(), &nDataSize, nullptr, 0) == 0)
		{
			str = sz.get();
			bReturn = true;
		}
	}

	return bReturn;
}

//#################################################################################################
bool GetSysctlValue(PCNSTR szName, CStr &str)
{
	bool bReturn = false;

	str.Empty();

	size_t nDataSize = 0;
	if(sysctlbyname(szName, nullptr, &nDataSize, nullptr, 0) != -1)
	{
		auto sz = std::make_unique<char[]>(nDataSize);
		if(sysctlbyname(szName, sz.get(), &nDataSize, nullptr, 0) == 0)
		{
			str = sz.get();
			bReturn = true;
		}
	}

	return bReturn;
}

#endif

NS_END

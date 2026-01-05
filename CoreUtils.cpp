#include "Base.h"
#include "CoreUtils.h"
#include "MemBuffer.h"
#include "PlatformUtils.h"
#ifdef _WIN32
#include "WhitespaceParser.h"
#include "RegKey.h"
#include <lm.h>
#include <intrin.h>
#include <wtsapi32.h>
#elif __APPLE__
#include "Object.h"
#include "ProcessUtils.h"
#include "TextFile.h"
#include <SystemConfiguration/SCDynamicStoreCopySpecific.h>
#include <SystemConfiguration/SCSchemaDefinitions.h>
#include <SystemConfiguration/SCPreferencesPath.h>
#include <crt_externs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <pwd.h>
#elif __linux__
#include "FileSystemUtils.h"
#include "TextFile.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <pwd.h>
#endif

NS_BEGIN

#ifdef _WIN32
//#################################################################################################
HINSTANCE GetApplicationInstance(void)
{
	return GetModuleHandleW(nullptr);
}
#endif

//#################################################################################################
std::vector<CStr> GetCommandLineArguments(void)
{
	std::vector<CStr> vecArgs;

#ifdef _WIN32
	CWhitespaceParser cmdline(GetCommandLineW());
	for(size_t n = 0; n < cmdline.GetCount(); ++n)
		vecArgs.push_back(cmdline.Get(n));
#elif __APPLE__
	const int *pnCount = _NSGetArgc();
	char ***pppszArguments = _NSGetArgv();
	if(pnCount && pppszArguments && *pppszArguments)
	{
		for(int n = 0; n < *pnCount; ++n)
			vecArgs.emplace_back((*pppszArguments)[n]);
	}
#elif __linux__
	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate("/proc/self/cmdline", EFM_ExistingReadOnly, hFile) == FW_NO_ERROR)
	{
		size_t nSize = 4096;
		auto pBuffer = std::make_unique<BYTE[]>(nSize);
		size_t nBytesRead;
		if(FileRead(hFile, pBuffer.get(), nSize, nBytesRead) == FW_NO_ERROR)
		{
			PCSTR szScan = (PCSTR)pBuffer.get();
			size_t nRemaining = nBytesRead;

			while(nRemaining)
			{
				size_t nLen = StringGetLength(szScan, nRemaining);
				vecArgs.emplace_back(szScan, 0, nLen);
				szScan += nLen + 1;
				nRemaining -= (nLen + 1);
			}
		}

		FileClose(hFile);
	}
#endif

	return vecArgs;
}

//#################################################################################################
CStr GetFriendlyMachineName(void)
{
	CStr strName;

#ifdef _WIN32
	strName = GetMachineName();
#elif __APPLE__
	CCFObject cfName = SCDynamicStoreCopyComputerName(nullptr, nullptr);
	strName = cfName.GetString();
#elif __linux__
	char szName[256];
	if(gethostname(szName, 256) != -1)
		strName = szName;
#endif

	return strName;
}

//#################################################################################################
CStr GetMachineName(void)
{
	CStr strName;

#ifdef _WIN32
	DWORD dwLen = 0;
	GetComputerNameExW(ComputerNamePhysicalNetBIOS, nullptr, &dwLen);
	if(dwLen)
	{
		auto szName = std::make_unique<wchar_t[]>((size_t)dwLen + 1);
		if(GetComputerNameExW(ComputerNamePhysicalNetBIOS, szName.get(), &dwLen))
			strName = szName.get();
	}
#elif __APPLE__
	CCFObject cfValue = SCDynamicStoreCopyLocalHostName(nullptr);
	strName = cfValue.GetString();
#elif __linux__
	char szName[256];
	if(gethostname(szName, 256) != -1)
		strName = szName;
#endif

	return strName;
}

//#################################################################################################
CStr GetFQDNName(void)
{
	CStr strName;

#ifdef _WIN32
	DWORD dwLen = 0;
	GetComputerNameExW(ComputerNamePhysicalDnsFullyQualified, nullptr, &dwLen);
	if(dwLen)
	{
		auto szName = std::make_unique<wchar_t[]>((size_t)dwLen + 1);
		if(GetComputerNameExW(ComputerNamePhysicalDnsFullyQualified, szName.get(), &dwLen))
			strName = szName.get();
	}
#elif __APPLE__
	// gethostname() does not always return the correct network name
	// So take the machine name and append ".local" which seems to always work
	strName = GetMachineName() + _N(".local");
#elif __linux__
	addrinfo aiHints = {0};
	addrinfo *paiResults = nullptr;
	aiHints.ai_family = AF_UNSPEC;
	aiHints.ai_socktype = SOCK_STREAM;
	aiHints.ai_flags = AI_CANONNAME;

	CStr strMachineName = GetMachineName();
	getaddrinfo(strMachineName, "http", &aiHints, &paiResults);
	if(paiResults)
	{
		addrinfo *paiScan = paiResults;
		while(paiScan && strName.IsEmpty())
		{
			strName = paiScan->ai_canonname;
			paiScan = paiScan->ai_next;
		}

		freeaddrinfo(paiResults);
	}
	else
		strName = std::move(strMachineName);
#endif

	return strName;
}

//#################################################################################################
CStr GetMachineDomain(void)
{
	CStr strMachineDomain;

#ifdef _WIN32
	PWSTR szDomain = nullptr;
	NETSETUP_JOIN_STATUS nsjs;
	if(NetGetJoinInformation(nullptr, &szDomain, &nsjs) == NERR_Success)
	{
		strMachineDomain = szDomain;
		NetApiBufferFree(szDomain);
	}
#endif

	return strMachineDomain;
}

//#################################################################################################
CStr GetSessionUsername(void)
{
	CStr strName;

#ifdef _WIN32
	strName = GetUsername(WTS_CURRENT_SESSION);
#else
	struct passwd pwd;
	struct passwd *pResult;

	size_t nBufferSize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if(nBufferSize == (size_t)-1)
		nBufferSize = 4096;

	auto buffer = std::make_unique<BYTE[]>(nBufferSize);
	if(getpwuid_r(geteuid(), &pwd, (PSTR)buffer.get(), nBufferSize, &pResult) == 0)
		strName = pResult->pw_name;
#endif

	return strName;
}

//#################################################################################################
EArch GetSystemArchitecture(void)
{
	EArch eArch = EA_Unknown;

#ifdef _WIN32
	using PFNISWOW64PROCESS2 = BOOL (WINAPI*)(HANDLE, PUSHORT, PUSHORT);
	HMODULE hKernelDll = SystemLoadLibrary(_N("kernel32.dll"));
	if(hKernelDll)
	{
		PFNISWOW64PROCESS2 pfnIsWow64Process2 = (PFNISWOW64PROCESS2)GetProcAddress(hKernelDll, "IsWow64Process2");
		if(pfnIsWow64Process2)
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, GetCurrentProcessId());
			if(hProcess)
			{
				USHORT nProcess = 0;
				USHORT nNative = 0;
				if(pfnIsWow64Process2(hProcess, &nProcess, &nNative))
				{
					if(nNative == IMAGE_FILE_MACHINE_I386)
						eArch = EA_x86;
					else if(nNative == IMAGE_FILE_MACHINE_AMD64)
						eArch = EA_x64;
					else if(nNative == IMAGE_FILE_MACHINE_ARM)
						eArch = EA_arm32;
					else if(nNative == IMAGE_FILE_MACHINE_ARM64)
						eArch = EA_arm64;
				}

				CloseHandle(hProcess);
			}
		}

		FreeLibrary(hKernelDll);
	}

	if(eArch == EA_Unknown)
	{
		SYSTEM_INFO si = {0};
		GetNativeSystemInfo(&si);
		if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
			eArch = EA_x86;
		else if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			eArch = EA_x64;
		else if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
			eArch = EA_arm32;
		else if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64)
			eArch = EA_arm64;
	}
#elif __APPLE__
#ifdef __aarch64__
	eArch = EA_arm64;
#else
	eArch = EA_x64;
#endif
#else
	// TODO
#endif

	return eArch;
}

//#################################################################################################
bool IsVirtualMachine(void)
{
	bool bVirtualMachine = false;

	// On x86/x64 processors, this function tests bit 31 of the ECX register from a CPUID call which is enabled by hypervisors; including VMWare, Hyper-V, VirtualBox, and Xen.
	// Note: Physical computers with Microsoft Hyper-V installed will appear as virtual computers, therefore perform additional tests to handle this case.

#ifdef _WIN32

#ifndef _M_ARM64
	int nRegisters[4] = {0};
	__cpuid(nRegisters, 1);		// Check CPUID leaf 1 ECX bit 31
	if(HAS_FLAG(nRegisters[2], 0x80000000))
	{
		__cpuid(nRegisters, 0x40000000);	// Check CPUID leaf 0x40000000 for "Microsoft Hv"
		if(nRegisters[1] == 0x7263694D && nRegisters[2] == 0x666F736F && nRegisters[3] == 0x76482074)
		{
			__cpuid(nRegisters, 0x40000003);	// Check CPUID leaf 0x40000003 EBX bit 1 for the "CreatePartitions" bit to determine host or child
			if(!HAS_FLAG(nRegisters[1], 1))
				bVirtualMachine = true;
		}
		else
			bVirtualMachine = true;		// Bit 31 is enabled and not Microsoft Hyper-V so it has to be a virtual machine
	}
#else
	// The only known way to detect a virtual machine for ARM is to search the BIOS string for known hypervisors
	CRegKey key;
	if(key.Open(HKEY_LOCAL_MACHINE, _N("HARDWARE\\DESCRIPTION\\System")) == FW_NO_ERROR)
	{
		CStr strBios;
		if(key.GetValue(_N("SystemBiosVersion"), strBios) != FW_NO_ERROR)
		{
			if(strBios.Contains(_N("VMware")) || strBios.Contains(_N("VirtualBox")) || strBios.Contains(_N("Microsoft")) || strBios.Contains(_N("KVM")) || strBios.Contains(_N("BOCHS")))
				bVirtualMachine = true;
		}

		key.Close();
	}
#endif

#elif !defined(__aarch64__)
	unsigned int eax = 1;
	unsigned int ebx = 0;
	unsigned int ecx = 0;
	unsigned int edx = 0;
	asm volatile("cpuid": "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "0"(eax), "2"(ecx));		// Check CPUID leaf 1 ECX bit 31
	if(HAS_FLAG(ecx, 0x80000000))
		bVirtualMachine = true;
#else
	// TODO MacOS ARM implementation
#endif

	return bVirtualMachine;
}

//#################################################################################################
CStr GetEnvVariable(PCNSTR szVariable)
{
	CStr strValue;

#ifdef _WIN32
	DWORD dwLen = GetEnvironmentVariableW(szVariable, nullptr, 0);
	if(dwLen)
	{
		auto szValue = std::make_unique<wchar_t[]>((size_t)dwLen + 1);
		if(GetEnvironmentVariableW(szVariable, szValue.get(), dwLen + 1))
			strValue = szValue.get();
	}
#else
	if(szVariable)
		strValue = getenv(szVariable);
#endif

	return strValue;
}

#ifdef _WIN32
//#################################################################################################
void ConsolePrint(PCWSTR szMessage, PCWSTR szCaption, PCWSTR szGuiMessage)
{
	if(AttachConsole(ATTACH_PARENT_PROCESS))
	{	// Reopen STDOUT
		FILE *pOut = nullptr;
		_wfreopen_s(&pOut, L"CONOUT$", L"w", stdout);
		setvbuf(stdout, nullptr, _IONBF, 0);

		std::wcout << std::endl << szMessage << std::endl;

		if(GetConsoleWindow() == GetForegroundWindow())
		{	// Send a benign Enter key to signal we are done with the console
			INPUT i[2] = {0};
			i[0].type = INPUT_KEYBOARD;
			i[0].ki.wVk = VK_RETURN;
			i[1].type = INPUT_KEYBOARD;
			i[1].ki.wVk = VK_RETURN;
			i[1].ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(2, i, sizeof(INPUT));
		}

		FreeConsole();
	}
	else
	{
		DWORD dwSessionId;
		if(ProcessIdToSessionId(GetCurrentProcessId(), &dwSessionId) && dwSessionId)
			MessageBoxExW(nullptr, (szGuiMessage == nullptr) ? szMessage : szGuiMessage, szCaption, MB_OK, 0);
	}
}
#else
//#################################################################################################
void ConsolePrint(PCSTR szMessage)
{
	std::cout << szMessage << std::endl;
}
#endif

//#################################################################################################
CStr8 UuidToString8(const uuid_t &uuid, const bool bAddBraces, const bool bUppercase)
{
	CStr8 strUuid;

#ifdef _WIN32
	PWSTR szUuid = nullptr;
	if(SUCCEEDED(StringFromIID(uuid, &szUuid)))
	{
		strUuid = szUuid;
		CoTaskMemFree(szUuid);
	}

	if(bUppercase)
		strUuid.MakeUpper();
	else
		strUuid.MakeLower();
#else
	char szUuid[37] = {0};
	if(bUppercase)
		uuid_unparse_upper(uuid, szUuid);
	else
		uuid_unparse_lower(uuid, szUuid);
	strUuid = szUuid;
#endif

	if(bAddBraces)
		AddUuidBraces8(strUuid);
	else
		RemoveUuidBraces8(strUuid);

	return strUuid;
}

//#################################################################################################
CStrW UuidToStringW(const uuid_t &uuid, const bool bAddBraces, const bool bUppercase)
{
	CStrW strUuid;

#ifdef _WIN32
	PWSTR szUuid = nullptr;
	if(SUCCEEDED(StringFromIID(uuid, &szUuid)))
	{
		strUuid = szUuid;
		CoTaskMemFree(szUuid);
	}

	if(bUppercase)
		strUuid.MakeUpper();
	else
		strUuid.MakeLower();
#else
	char szUuid[37] = {0};
	if(bUppercase)
		uuid_unparse_upper(uuid, szUuid);
	else
		uuid_unparse_lower(uuid, szUuid);
	strUuid = szUuid;
#endif

	if(bAddBraces)
		AddUuidBracesW(strUuid);
	else
		RemoveUuidBracesW(strUuid);

	return strUuid;
}

//#################################################################################################
void StringToUuid8(const CStr8 &strUuid, uuid_t &uuid)
{
	SetMemoryZero(&uuid, sizeof(uuid));
#ifdef _WIN32
	if(IsValidUuid8(strUuid))
	{
		CStrW str(strUuid);
		AddUuidBracesW(str);
		Verify(SUCCEEDED(IIDFromString((PWSTR)(PCWSTR)str, &uuid)));
	}
#else
	CStr8 str(strUuid);
	RemoveUuidBraces(str);
	uuid_parse(str, uuid);
#endif
}

//#################################################################################################
void StringToUuidW(const CStrW &strUuid, uuid_t &uuid)
{
	SetMemoryZero(&uuid, sizeof(uuid));
#ifdef _WIN32
	if(IsValidUuidW(strUuid))
	{
		CStrW str(strUuid);
		AddUuidBracesW(str);
		Verify(SUCCEEDED(IIDFromString((PWSTR)(PCWSTR)str, &uuid)));
	}
#else
	CStr8 str(strUuid);
	RemoveUuidBraces(str);
	uuid_parse(str, uuid);
#endif
}

//#################################################################################################
void AddUuidBraces8(CStr8 &strUuid)
{
	if(IsValidUuid8(strUuid))
	{
		if(strUuid.GetFirst() != '{')
			strUuid.Prepend('{');

		if(strUuid.GetLast() != '}')
			strUuid.Append('}');
	}
}

//#################################################################################################
void AddUuidBracesW(CStrW &strUuid)
{
	if(IsValidUuidW(strUuid))
	{
		if(strUuid.GetFirst() != L'{')
			strUuid.Prepend(L'{');

		if(strUuid.GetLast() != L'}')
			strUuid.Append(L'}');
	}
}

//#################################################################################################
void RemoveUuidBraces8(CStr8 &strUuid)
{
	if(IsValidUuid8(strUuid))
	{
		if(strUuid.GetFirst() == '{')
			strUuid.Delete(0);

		if(strUuid.GetLast() == '}')
			strUuid.Delete(strUuid.GetLength() - 1);
	}
}

//#################################################################################################
void RemoveUuidBracesW(CStrW &strUuid)
{
	if(IsValidUuidW(strUuid))
	{
		if(strUuid.GetFirst() == L'{')
			strUuid.Delete(0);

		if(strUuid.GetLast() == L'}')
			strUuid.Delete(strUuid.GetLength() - 1);
	}
}

//#################################################################################################
bool IsValidUuid8(const CStr8 &strUuid)
{	// This function checks the string to see if it is a valid uuid in one of the following forms:
	// {F1F8139B-9BAB-11D3-BF49-00C04F613FDE} or F1F8139B-9BAB-11D3-BF49-00C04F613FDE

	bool bValid = false;

	if(strUuid.GetLength() == 36)
	{	// Does it have the hyphens?
		if(strUuid[8] == '-' && strUuid[13] == '-' && strUuid[18] == '-' && strUuid[23] == '-')
		{	// Check if the rest of the characters are hex?
			if(isxdigit(strUuid[0]) && isxdigit(strUuid[1]) && isxdigit(strUuid[2]) && isxdigit(strUuid[3]) && isxdigit(strUuid[4]) && isxdigit(strUuid[5]) && isxdigit(strUuid[6]) && isxdigit(strUuid[7]) &&
				isxdigit(strUuid[9]) && isxdigit(strUuid[10]) && isxdigit(strUuid[11]) && isxdigit(strUuid[12]) &&
				isxdigit(strUuid[14]) && isxdigit(strUuid[15]) && isxdigit(strUuid[16]) && isxdigit(strUuid[17]) &&
				isxdigit(strUuid[19]) && isxdigit(strUuid[20]) && isxdigit(strUuid[21]) && isxdigit(strUuid[22]) &&
				isxdigit(strUuid[24]) && isxdigit(strUuid[25]) && isxdigit(strUuid[26]) && isxdigit(strUuid[27]) && isxdigit(strUuid[28]) && isxdigit(strUuid[29]) && isxdigit(strUuid[30]) && isxdigit(strUuid[31]) && isxdigit(strUuid[32]) && isxdigit(strUuid[33]) && isxdigit(strUuid[34]) && isxdigit(strUuid[35]))
			{
				bValid = true;
			}
		}
	}
	else if(strUuid.GetLength() == 38)
	{	// Does it have the curly braces?
		if(strUuid[0] == '{' && strUuid[37] == '}')
		{	// Does it have the hyphens?
			if(strUuid[9] == '-' && strUuid[14] == '-' && strUuid[19] == '-' && strUuid[24] == '-')
			{	// Check if the rest of the characters are hex?
				if(isxdigit(strUuid[1]) && isxdigit(strUuid[2]) && isxdigit(strUuid[3]) && isxdigit(strUuid[4]) && isxdigit(strUuid[5]) && isxdigit(strUuid[6]) && isxdigit(strUuid[7]) && isxdigit(strUuid[8]) &&
					isxdigit(strUuid[10]) && isxdigit(strUuid[11]) && isxdigit(strUuid[12]) && isxdigit(strUuid[13]) &&
					isxdigit(strUuid[15]) && isxdigit(strUuid[16]) && isxdigit(strUuid[17]) && isxdigit(strUuid[18]) &&
					isxdigit(strUuid[20]) && isxdigit(strUuid[21]) && isxdigit(strUuid[22]) && isxdigit(strUuid[23]) &&
					isxdigit(strUuid[25]) && isxdigit(strUuid[26]) && isxdigit(strUuid[27]) && isxdigit(strUuid[28]) && isxdigit(strUuid[29]) && isxdigit(strUuid[30]) && isxdigit(strUuid[31]) && isxdigit(strUuid[32]) && isxdigit(strUuid[33]) && isxdigit(strUuid[34]) && isxdigit(strUuid[35]) && isxdigit(strUuid[36]))
				{
					bValid = true;
				}
			}
		}
	}

	return bValid;
}

//#################################################################################################
bool IsValidUuidW(const CStrW &strUuid)
{	// This function checks the string to see if it is a valid uuid in one of the following forms:
	// {F1F8139B-9BAB-11D3-BF49-00C04F613FDE} or F1F8139B-9BAB-11D3-BF49-00C04F613FDE

	bool bValid = false;

	if(strUuid.GetLength() == 36)
	{	// Does it have the hyphens?
		if(strUuid[8] == L'-' && strUuid[13] == L'-' && strUuid[18] == L'-' && strUuid[23] == L'-')
		{	// Check if the rest of the characters are hex?
			if(iswxdigit(strUuid[0]) && iswxdigit(strUuid[1]) && iswxdigit(strUuid[2]) && iswxdigit(strUuid[3]) && iswxdigit(strUuid[4]) && iswxdigit(strUuid[5]) && iswxdigit(strUuid[6]) && iswxdigit(strUuid[7]) &&
				iswxdigit(strUuid[9]) && iswxdigit(strUuid[10]) && iswxdigit(strUuid[11]) && iswxdigit(strUuid[12]) &&
				iswxdigit(strUuid[14]) && iswxdigit(strUuid[15]) && iswxdigit(strUuid[16]) && iswxdigit(strUuid[17]) &&
				iswxdigit(strUuid[19]) && iswxdigit(strUuid[20]) && iswxdigit(strUuid[21]) && iswxdigit(strUuid[22]) &&
				iswxdigit(strUuid[24]) && iswxdigit(strUuid[25]) && iswxdigit(strUuid[26]) && iswxdigit(strUuid[27]) && iswxdigit(strUuid[28]) && iswxdigit(strUuid[29]) && iswxdigit(strUuid[30]) && iswxdigit(strUuid[31]) && iswxdigit(strUuid[32]) && iswxdigit(strUuid[33]) && iswxdigit(strUuid[34]) && iswxdigit(strUuid[35]))
			{
				bValid = true;
			}
		}
	}
	else if(strUuid.GetLength() == 38)
	{	// Does it have the curly braces?
		if(strUuid[0] == L'{' && strUuid[37] == L'}')
		{	// Does it have the hyphens?
			if(strUuid[9] == L'-' && strUuid[14] == L'-' && strUuid[19] == L'-' && strUuid[24] == L'-')
			{	// Check if the rest of the characters are hex?
				if(iswxdigit(strUuid[1]) && iswxdigit(strUuid[2]) && iswxdigit(strUuid[3]) && iswxdigit(strUuid[4]) && iswxdigit(strUuid[5]) && iswxdigit(strUuid[6]) && iswxdigit(strUuid[7]) && iswxdigit(strUuid[8]) &&
					iswxdigit(strUuid[10]) && iswxdigit(strUuid[11]) && iswxdigit(strUuid[12]) && iswxdigit(strUuid[13]) &&
					iswxdigit(strUuid[15]) && iswxdigit(strUuid[16]) && iswxdigit(strUuid[17]) && iswxdigit(strUuid[18]) &&
					iswxdigit(strUuid[20]) && iswxdigit(strUuid[21]) && iswxdigit(strUuid[22]) && iswxdigit(strUuid[23]) &&
					iswxdigit(strUuid[25]) && iswxdigit(strUuid[26]) && iswxdigit(strUuid[27]) && iswxdigit(strUuid[28]) && iswxdigit(strUuid[29]) && iswxdigit(strUuid[30]) && iswxdigit(strUuid[31]) && iswxdigit(strUuid[32]) && iswxdigit(strUuid[33]) && iswxdigit(strUuid[34]) && iswxdigit(strUuid[35]) && iswxdigit(strUuid[36]))
				{
					bValid = true;
				}
			}
		}
	}

	return bValid;
}

//#################################################################################################
CStr8 GenerateNewUuid8(const bool bAddBraces, const bool bUppercase)
{
	uuid_t uuid = {0};

#ifdef _WIN32
	Verify(SUCCEEDED(CoCreateGuid(&uuid)));
#else
	uuid_generate(uuid);
#endif

	CStr8 strUuid = UuidToString8(uuid);

	if(bAddBraces)
		AddUuidBraces8(strUuid);
	else
		RemoveUuidBraces8(strUuid);

	if(bUppercase)
		strUuid.MakeUpper();
	else
		strUuid.MakeLower();

	return strUuid;
}

//#################################################################################################
CStrW GenerateNewUuidW(const bool bAddBraces, const bool bUppercase)
{
	uuid_t uuid = {0};

#ifdef _WIN32
	Verify(SUCCEEDED(CoCreateGuid(&uuid)));
#else
	uuid_generate(uuid);
#endif

	CStrW strUuid = UuidToStringW(uuid);

	if(bAddBraces)
		AddUuidBracesW(strUuid);
	else
		RemoveUuidBracesW(strUuid);

	if(bUppercase)
		strUuid.MakeUpper();
	else
		strUuid.MakeLower();

	return strUuid;
}

//#################################################################################################
void GenerateUuid(uuid_t &uuid)
{
#ifdef _WIN32
	Verify(SUCCEEDED(CoCreateGuid(&uuid)));
#else
	uuid_generate(uuid);
#endif
}

//#################################################################################################
CStr8 UUEncode8(const CMemBuffer &bufDecoded)
{
	CStr8 strEncoded;

	if(!bufDecoded.IsEmpty())
	{
		strEncoded.ReserveBuffer(bufDecoded.GetDataSize() * 2 + 1);
		for(size_t n = 0; n < bufDecoded.GetDataSize(); ++n)
			strEncoded += CStr8(CStr8::EPT_Printf, "%02X", bufDecoded.GetAt(n));
	}

	return strEncoded;
}

//#################################################################################################
CStr8 UUEncode8(PCBYTE pDecoded, const size_t nDecodedSize)
{
	CStr8 strEncoded;

	if(nDecodedSize != 0)
	{
		strEncoded.ReserveBuffer(nDecodedSize * 2 + 1);
		for(size_t n = 0; n < nDecodedSize; ++n)
			strEncoded += CStr8(CStr8::EPT_Printf, "%02X", *(pDecoded + n));
	}

	return strEncoded;
}

//#################################################################################################
CStrW UUEncodeW(const CMemBuffer &bufDecoded)
{
	CStrW strEncoded;

	if(!bufDecoded.IsEmpty())
	{
		strEncoded.ReserveBuffer(bufDecoded.GetDataSize() * 2 + 1);
		for(size_t n = 0; n < bufDecoded.GetDataSize(); ++n)
			strEncoded += CStrW(CStrW::EPT_Printf, L"%02X", bufDecoded.GetAt(n));
	}

	return strEncoded;
}

//#################################################################################################
CStrW UUEncodeW(PCBYTE pDecoded, const size_t nDecodedSize)
{
	CStrW strEncoded;

	if(nDecodedSize != 0)
	{
		strEncoded.ReserveBuffer(nDecodedSize * 2 + 1);
		for(size_t n = 0; n < nDecodedSize; ++n)
			strEncoded += CStrW(CStrW::EPT_Printf, L"%02X", *(pDecoded + n));
	}

	return strEncoded;
}

//#################################################################################################
bool UUDecode8(const CStr8 &strEncoded, CMemBuffer &bufDecoded)
{
	bool bSuccess = false;

	size_t nLen = strEncoded.GetLength();
	if(nLen % 2 == 0)
	{
		bufDecoded.SetBufferSize(nLen / 2);
		char sz[3] = {0};
		for(size_t n = 0; n < nLen; n += 2)
		{
			sz[0] = strEncoded.GetAt(n);
			sz[1] = strEncoded.GetAt(n + 1);
			bufDecoded.SetAt(n / 2, StringToInteger8<BYTE>(sz, 16));
		}

		bufDecoded.SetDataSize(nLen / 2);
		bSuccess = true;
	}

	return bSuccess;
}

//#################################################################################################
bool UUDecode8(const CStr8 &strEncoded, PBYTE pDecoded, size_t &nDecodedSize)
{
	bool bSuccess = false;

	size_t nLen = strEncoded.GetLength();
	if(nLen % 2 == 0)
	{
		if(nDecodedSize >= nLen / 2)
		{
			PBYTE pScan = pDecoded;
			char sz[3] = {0};
			for(size_t n = 0; n < nLen; n += 2)
			{
				sz[0] = strEncoded.GetAt(n);
				sz[1] = strEncoded.GetAt(n + 1);
				*pScan++ = StringToInteger8<BYTE>(sz, 16);
			}

			bSuccess = true;
		}

		nDecodedSize = nLen / 2;
	}
	else
		nDecodedSize = 0;

	return bSuccess;
}

//#################################################################################################
bool UUDecodeW(const CStrW &strEncoded, CMemBuffer &bufDecoded)
{
	bool bSuccess = false;

	size_t nLen = strEncoded.GetLength();
	if(nLen % 2 == 0)
	{
		bufDecoded.SetBufferSize(nLen / 2);
		wchar_t sz[3] = {0};
		for(size_t n = 0; n < nLen; n += 2)
		{
			sz[0] = strEncoded.GetAt(n);
			sz[1] = strEncoded.GetAt(n + 1);
			bufDecoded.SetAt(n / 2, StringToIntegerW<BYTE>(sz, 16));
		}

		bufDecoded.SetDataSize(nLen / 2);
		bSuccess = true;
	}

	return bSuccess;
}

//#################################################################################################
bool UUDecodeW(const CStrW &strEncoded, PBYTE pDecoded, size_t &nDecodedSize)
{
	bool bSuccess = false;

	size_t nLen = strEncoded.GetLength();
	if(nLen % 2 == 0)
	{
		if(nDecodedSize >= nLen / 2)
		{
			PBYTE pScan = pDecoded;
			wchar_t sz[3] = {0};
			for(size_t n = 0; n < nLen; n += 2)
			{
				sz[0] = strEncoded.GetAt(n);
				sz[1] = strEncoded.GetAt(n + 1);
				*pScan++ = StringToIntegerW<BYTE>(sz, 16);
			}

			bSuccess = true;
		}

		nDecodedSize = nLen / 2;
	}
	else
		nDecodedSize = 0;

	return bSuccess;
}

//#################################################################################################
CStr GetFriendlyErrorName(const ERRCODE nErrorCode)
{
	CStr strName;

// Macro to simplify declaring error codes
#define DECLARE_ERROR(X) case (X): strName = _N(#X); break

	switch(nErrorCode)
	{
		DECLARE_ERROR(FW_NO_ERROR);
		DECLARE_ERROR(FW_ERROR_ABORTED);
		DECLARE_ERROR(FW_ERROR_ACCESS_DENIED);
		DECLARE_ERROR(FW_ERROR_BUSY);
		DECLARE_ERROR(FW_ERROR_ENCRYPTION_FAILED);
		DECLARE_ERROR(FW_ERROR_EXISTS);
		DECLARE_ERROR(FW_ERROR_HANDLE_EOF);
		DECLARE_ERROR(FW_ERROR_INSUFFICIENT_BUFFER);
		DECLARE_ERROR(FW_ERROR_INTERRUPTED);
		DECLARE_ERROR(FW_ERROR_INVALID_HANDLE);
		DECLARE_ERROR(FW_ERROR_INVALID_PARAMETER);
		DECLARE_ERROR(FW_ERROR_INVALID_DATA);
		DECLARE_ERROR(FW_ERROR_IO_PENDING);
		DECLARE_ERROR(FW_ERROR_MORE_DATA);
		DECLARE_ERROR(FW_ERROR_NOT_ENOUGH_MEMORY);
		DECLARE_ERROR(FW_ERROR_NOT_FOUND);
		DECLARE_ERROR(FW_ERROR_QUOTA);
		DECLARE_ERROR(FW_ERROR_CRC);
		DECLARE_ERROR(FW_ERROR_NOT_SUPPORTED);
		DECLARE_ERROR(FW_ERROR_HASH);
		DECLARE_ERROR(FW_ERROR_TOO_LARGE);
		DECLARE_ERROR(FW_ERROR_LOW_LEVEL);
		DECLARE_ERROR(FW_ERROR_IS_DIR);
		DECLARE_ERROR(FW_ERROR_IS_NOT_DIR);
		DECLARE_ERROR(FW_ERROR_LINK);
		DECLARE_ERROR(FW_ERROR_HANDLES);
		DECLARE_ERROR(FW_ERROR_EMPTY);
		DECLARE_ERROR(FW_ERROR_SIGN_FAIL);
		DECLARE_ERROR(FW_ERROR_NOT_CONNECTED);
		DECLARE_ERROR(FW_ERROR_SHUTTING_DOWN);
		DECLARE_ERROR(FW_ERROR_ALREADY_CONNECTED);
		DECLARE_ERROR(FW_ERROR_AUTHENTICATION_FAILED);
		DECLARE_ERROR(FW_ERROR_NOT_IMPLEMENTED);
		DECLARE_ERROR(FW_ERROR_NETWORK_ISSUE);
		DECLARE_ERROR(FW_ERROR_SERVER_WEBCALL_FAILED);
		DECLARE_ERROR(FW_ERROR_SERVER_AUTH_FAILED);
		DECLARE_ERROR(FW_ERROR_SERVER_EXCEPTION);
		DECLARE_ERROR(FW_ERROR_DISABLED);
		DECLARE_ERROR(FW_ERROR_NOT_EMPTY);
		DECLARE_ERROR(FW_ERROR_UNKNOWN);

	default:
		strName.Format(_N("Unknown {0}"), nErrorCode);
		break;
	}

	return strName;
}

NS_END

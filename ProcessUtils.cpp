#include "Base.h"
#include "ProcessUtils.h"
#include "FilePathUtils.h"
#ifdef _WIN32
#include "PlatformUtils.h"
#include "PrivilegeManager.h"
#include <winternl.h>
#define PSAPI_VERSION (1)	// Required for original Server 2008 support
#include <psapi.h>
#elif __APPLE__
#include <libproc.h>
#include "CoreUtils.h"
#include "Object.h"
#include "WhitespaceParser.h"
#include <CoreFoundation/CoreFoundation.h>
#include <sys/event.h>
#include <sys/sysctl.h>
#include <sys/wait.h>
#include <map>
#elif __linux__
#include "FileSystemUtils.h"
#include "WhitespaceParser.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <map>
#endif

#ifndef _WIN32
extern PSTR *environ;
#endif

NS_BEGIN

#ifndef _WIN32
static constexpr auto PIPE_READ = 0;
static constexpr auto PIPE_WRITE = 1;

PSTR *CreateArgs(const CStr8 &strApp, const CStr8 &strParams);
PSTR *CreateEnvVars(const std::vector<CStr8> *pvecEnvVars);
bool IsStreamReady(const int fd);

#ifdef __APPLE__
std::vector<CFilePath8> GetPathEnvironmentVar(void);
#endif
#endif

//#################################################################################################
uint32_t GetProcessId(void)
{
#ifdef _WIN32
	return GetCurrentProcessId();
#else
	return getpid();
#endif
}

//#################################################################################################
CFilePath GetProcessFilename(const uint32_t nPid)
{
	CFilePath pathFilename;

#ifdef _WIN32
	// QueryFullProcessImageName() may return the wrong path, for example if the filename was renamed it may return the cached old path.
	// GetModuleFileNameEx() returns the correct path; however it cannot get the path for all processes because it requires higher privileges.
	// GetProcessImageFileNameW() is yet another API, but it returns paths using 'device' syntax and needs further resolving.
	// So try GetModuleFileNameEx() first, then fallback to QueryFullProcessImageName(), and lastly GetProcessImageFileNameW().

	CPrivilegeManager pm;
	pm.Grant(SE_DEBUG_NAME);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, nPid);
	pm.RevokeAll();

	if(!hProcess)
		hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, nPid);

	if(hProcess)
	{
		wchar_t szName[1024] = {0};

		GetModuleFileNameExW(hProcess, nullptr, szName, 1024);

		pathFilename = szName;
		if(pathFilename.IsEmpty())
		{
			DWORD dwLen = 1024;
			if(QueryFullProcessImageNameW(hProcess, 0, szName, &dwLen))
				pathFilename = szName;

			if(pathFilename.IsEmpty())
			{
				if(GetProcessImageFileNameW(hProcess, szName, 1024))
				{	// The following code is a lightweight version found in CFilePathConverter
					std::vector<std::pair<CStr, CStr>> vecDevObjsLookup;

					// Init
					vecDevObjsLookup.reserve(6);
					vecDevObjsLookup.emplace_back(_N("\\??\\"), CStr());
					vecDevObjsLookup.emplace_back(_N("\\\\?\\UNC\\"), _N("\\\\"));
					vecDevObjsLookup.emplace_back(_N("\\\\?\\"), CStr());
					vecDevObjsLookup.emplace_back(_N("\\Device\\Mup\\"), _N("\\\\"));
					vecDevObjsLookup.emplace_back(_N("\\SystemRoot"), GetWindowsFolder().Get());

					NCHAR szDrive[3] = {_N('A'), _N(':'), _N('\0')};
					DWORD dwDrives = GetLogicalDrives();
					wchar_t szDevice[MAX_PATH];
					CStr strDrive;
					for(NCHAR n = 0; n < 26 && dwDrives; ++n)
					{
						if(HAS_FLAG(dwDrives, 0x1))
						{
							szDrive[0] = _N('A') + n;
							strDrive = szDrive;

							if(QueryDosDeviceW(strDrive, szDevice, COUNTOF(szDevice)))
								vecDevObjsLookup.emplace_back(szDevice, std::move(strDrive));
						}

						dwDrives >>= 1;
					}

					// Resolve
					CStr strName(szName);
					if(strName.CompareLeft(_N("\\Device\\Mup\\;LanmanRedirector\\;"), 31) == 0)
					{
						size_t nPos = strName.Find(_N('\\'), 31);
						if(nPos != CStr::not_found)
							strName.Delete(12, nPos - 11);
					}

					for(const auto &itr : vecDevObjsLookup)
					{
						if(strName.CompareLeft(itr.first, itr.first.GetLength(), true) == 0)
						{
							strName.Delete(0, itr.first.GetLength());
							strName.Prepend(itr.second);
							pathFilename = strName;
							break;
						}
					}
				}
			}
		}

		CloseHandle(hProcess);
	}

	// Ensure we always have a long filename
	ConvertToLong(pathFilename);
#elif __APPLE__
	// Call the undocumented proc_pidinfo() API, but do not trust the return value which is sometimes wrong
	char szPath[PROC_PIDPATHINFO_MAXSIZE] = {0};
	proc_pidinfo(nPid, PROC_PIDPATHINFO, 0, &szPath, sizeof(szPath));
	pathFilename = szPath;
	Canonicalize(pathFilename);
#elif __linux__
	CStr8 strPath(CStr8::EFT_Format, "/proc/{0}/exe", nPid);
	char szPath[MAX_PATH] = {0};
	if(readlink(strPath, szPath, MAX_PATH) != -1)
		pathFilename = szPath;
#endif

	return pathFilename;
}

//#################################################################################################
CStr GetProcessCommandLine(const uint32_t nPid)
{
	CStr strCmdLine;

#ifdef _WIN32
	using PFNNTQUERYINFORMATIONPROCESS = NTSTATUS(NTAPI*)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

	HMODULE hNtDll = SystemLoadLibrary(_N("ntdll.dll"));
	if(hNtDll)
	{
		PFNNTQUERYINFORMATIONPROCESS pfnNtQueryInformationProcess = (PFNNTQUERYINFORMATIONPROCESS)GetProcAddress(hNtDll, "NtQueryInformationProcess");

		if(pfnNtQueryInformationProcess)
		{
			CPrivilegeManager pm;
			pm.Grant(SE_DEBUG_NAME);
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, nPid);
			pm.RevokeAll();

			if(hProcess)
			{
				HANDLE hHeap = GetProcessHeap();
				PPROCESS_BASIC_INFORMATION ppbi = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(PROCESS_BASIC_INFORMATION));
				if(ppbi)
				{
					DWORD dwRequiredSize;
					NTSTATUS status = pfnNtQueryInformationProcess(hProcess, ProcessBasicInformation, ppbi, sizeof(PROCESS_BASIC_INFORMATION), &dwRequiredSize);
					if(status >= 0 && dwRequiredSize > sizeof(PROCESS_BASIC_INFORMATION))
					{
						HeapFree(hHeap, 0, ppbi);
						ppbi = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwRequiredSize);
						if(ppbi)
							status = pfnNtQueryInformationProcess(hProcess, ProcessBasicInformation, ppbi, dwRequiredSize, &dwRequiredSize);
					}

					if(status >= 0 && ppbi && ppbi->PebBaseAddress)
					{
						PEB peb = {0};
						SIZE_T nBytesRead;
						if(ReadProcessMemory(hProcess, ppbi->PebBaseAddress, &peb, sizeof(peb), &nBytesRead))
						{
							RTL_USER_PROCESS_PARAMETERS rtlupp = {0};
							if(ReadProcessMemory(hProcess, peb.ProcessParameters, &rtlupp, sizeof(RTL_USER_PROCESS_PARAMETERS), &nBytesRead))
							{
								if(rtlupp.CommandLine.Length)
								{
									PWSTR szBuffer = (PWSTR)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, rtlupp.CommandLine.Length);
									if(szBuffer)
									{
										if(ReadProcessMemory(hProcess, rtlupp.CommandLine.Buffer, szBuffer, rtlupp.CommandLine.Length, &nBytesRead))
											strCmdLine.Assign(szBuffer, 0, rtlupp.CommandLine.Length / sizeof(wchar_t));

										HeapFree(hHeap, 0, szBuffer);
									}
								}
							}
						}
					}

					if(ppbi)
						HeapFree(hHeap, 0, ppbi);
				}

				CloseHandle(hProcess);
			}
		}

		FreeLibrary(hNtDll);
	}
#elif __APPLE__
	int mib[3] = {CTL_KERN, KERN_PROCARGS2, (int)nPid};
	size_t nSize = 0;
	if(sysctl(mib, COUNTOF(mib), nullptr, &nSize, nullptr, 0) == 0)
	{
		nSize += 256;	// Add more in case the strings change between calls
		auto szBuffer = std::make_unique<char[]>(nSize);
		if(sysctl(mib, COUNTOF(mib), szBuffer.get(), &nSize, nullptr, 0) == 0 && nSize > sizeof(int))
		{
			int nCount;
			PCSTR pScan = szBuffer.get();
			std::memcpy(&nCount, pScan, sizeof(nCount));
			pScan += sizeof(int);
			++nCount;	// Add one for the leading executable filename

			bool bNull = false;
			std::vector<CStr8> vec;
			CStr8 str;
			int nFound = 0;

			while(nFound < nCount && (size_t)(pScan - szBuffer.get()) < nSize)
			{
				if(*pScan)
				{
					if(bNull == true)
					{
						vec.push_back(std::move(str));
						++nFound;
						bNull = false;
					}

					str += *pScan;
				}
				else
					bNull = true;

				++pScan;
			}

			vec.erase(vec.begin());		// First entry is the executable filename
			for(const auto &itr : vec)
			{
				if(!strCmdLine.IsEmpty())
					strCmdLine += _N(' ');
				strCmdLine += itr;
			}
		}
	}
#elif __linux__
	// Technically the command line can be longer than 8K, but our product cannot handle command lines
	// longer than 1K, so read the first 8K and truncate the rest
	static constexpr auto BUFFER_SIZE = 8192;

	CFilePath pathProc("/proc");
	pathProc += CFilePathSegment(IntegerToString(nPid));
	pathProc += CFilePathSegment("cmdline");

	NHANDLE hFile = INVALID_NHANDLE;
	ERRCODE nErrorCode = FileCreate(pathProc.Get(), EFM_ExistingReadOnly, hFile);
	if(nErrorCode == FW_NO_ERROR)
	{
		auto pBuffer = std::make_unique<BYTE[]>(BUFFER_SIZE);
		std::memset(pBuffer.get(), 0, BUFFER_SIZE);
		size_t nBytesRead;
		if(FileRead(hFile, pBuffer.get(), BUFFER_SIZE, nBytesRead) == FW_NO_ERROR)
		{
			for(size_t n = 0; n < nBytesRead; ++n)
			{	// Linux replaces spaces between parameters with nulls
				if(pBuffer.get()[n] == '\0')
					pBuffer.get()[n] = ' ';
			}
			strCmdLine.Assign((PCNSTR)pBuffer.get(), 0, nBytesRead);
			strCmdLine.Trim();
		}

		FileClose(hFile);
	}
#endif

	return strCmdLine;
}

#ifdef _WIN32
//#################################################################################################
ERRCODE SpawnProcessAsync(const CStrW &strApp, const CStrW &strParams, const bool bHidden, uint32_t *pnPid)
{
	Assert(!strApp.IsEmpty());

	ERRCODE nErrorCode = FW_NO_ERROR;

	SHELLEXECUTEINFOW sei = {0};
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = L"runas";
	sei.lpFile = strApp;
	if(!strParams.IsEmpty())
		sei.lpParameters = strParams;
	sei.nShow = bHidden ? SW_HIDE : SW_SHOW;

	if(ShellExecuteExW(&sei) && sei.hInstApp > (HINSTANCE)32)
	{
		if(pnPid)
			*pnPid = ::GetProcessId(sei.hProcess);
		CloseHandle(sei.hProcess);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	return nErrorCode;
}

//#################################################################################################
ERRCODE SpawnProcess(const CStrW &strApp, const CStrW &strParams, const bool bHidden, ERRCODE *pnErrorCodeChild, CStrW *pstrStdOutput, CStrW *pstrStdError)
{
	Assert(!strApp.IsEmpty());

	ERRCODE nErrorCode = FW_NO_ERROR;

	PROCESS_INFORMATION pi = {0};
	STARTUPINFOW si = {0};
	si.cb = sizeof(si);
	ADD_FLAG(si.dwFlags, STARTF_USESHOWWINDOW);
	si.wShowWindow = bHidden ? SW_HIDE : SW_SHOW;

	HANDLE hReadOutput = nullptr;
	HANDLE hWriteOutput = nullptr;
	HANDLE hReadError = nullptr;
	HANDLE hWriteError = nullptr;

	if(pnErrorCodeChild)
		*pnErrorCodeChild = FW_ERROR_UNKNOWN;

	if(pstrStdOutput)
	{
		pstrStdOutput->Empty();
		ADD_FLAG(si.dwFlags, STARTF_USESTDHANDLES);
		CreatePipe(&hReadOutput, &hWriteOutput, nullptr, 0x00100000);	// Create a 1MB pipe for standard output
		DuplicateHandle(GetCurrentProcess(), hWriteOutput, GetCurrentProcess(), &si.hStdOutput, 0, true, DUPLICATE_SAME_ACCESS);
	}

	if(pstrStdError)
	{
		pstrStdError->Empty();
		ADD_FLAG(si.dwFlags, STARTF_USESTDHANDLES);
		CreatePipe(&hReadError, &hWriteError, nullptr, 0x00100000);		// Create a 1MB pipe for standard error
		DuplicateHandle(GetCurrentProcess(), hWriteError, GetCurrentProcess(), &si.hStdError, 0, true, DUPLICATE_SAME_ACCESS);
	}

	CStrW strCmdLine = L'"' + strApp + L'"';
	if(!strParams.IsEmpty())
	{
		strCmdLine += L' ';
		strCmdLine += strParams;
	}

	// Execute the command
	if(CreateProcessW(nullptr, strCmdLine.GetBuffer(), nullptr, nullptr, TRUE, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		if(pnErrorCodeChild)
		{
			DWORD dwErrorCode;
			GetExitCodeProcess(pi.hProcess, &dwErrorCode);
			*pnErrorCodeChild = dwErrorCode;
		}

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		if(si.hStdOutput)
		{
			CloseHandle(si.hStdOutput);
			si.hStdOutput = nullptr;
		}

		if(hWriteOutput)
		{
			CloseHandle(hWriteOutput);
			hWriteOutput = nullptr;
		}

		if(si.hStdError)
		{
			CloseHandle(si.hStdError);
			si.hStdError = nullptr;
		}

		if(hWriteError)
		{
			CloseHandle(hWriteError);
			hWriteError = nullptr;
		}

		if(pstrStdOutput)
		{
			BYTE buffer[4096];
			DWORD dwBytesRead;
			while(ReadFile(hReadOutput, buffer, sizeof(buffer), &dwBytesRead, nullptr) && dwBytesRead)
				pstrStdOutput->Append((PCSTR)buffer, 0, dwBytesRead);
		}

		if(pstrStdError)
		{
			BYTE buffer[4096];
			DWORD dwBytesRead;
			while(ReadFile(hReadError, buffer, sizeof(buffer), &dwBytesRead, nullptr) && dwBytesRead)
				pstrStdError->Append((PCSTR)buffer, 0, dwBytesRead);
		}
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(GetLastError());

	if(hReadOutput)
		CloseHandle(hReadOutput);

	if(si.hStdOutput)
		CloseHandle(si.hStdOutput);

	if(hWriteOutput)
		CloseHandle(hWriteOutput);

	if(hReadError)
		CloseHandle(hReadError);

	if(si.hStdError)
		CloseHandle(si.hStdError);

	if(hWriteError)
		CloseHandle(hWriteError);

	return nErrorCode;
}

#else

//#################################################################################################
ERRCODE SpawnProcessAsync(const CStr8 &strApp, const CStr8 &strParams, const std::vector<CStr8> *pvecEnvVars, pid_t *pnPid, const bool bNewProcessGroup)
{
	Assert(!strApp.IsEmpty());

	ERRCODE nErrorCode = FW_NO_ERROR;

	if(pnPid)
		*pnPid = 0;

	pid_t nPid = fork();
	if(nPid == 0)
	{	// Child process
		if(bNewProcessGroup)
			setsid();

		// Close any open file descriptors
		int nNumber = (int)sysconf(_SC_OPEN_MAX);
		if(nNumber != -1)
		{
			for(int n = 3; n < nNumber; ++n)
				close(n);
		}

		// Reset all signal handlers
#ifdef __APPLE__
		for(int n = 1; n < __DARWIN_NSIG; ++n)
#else
		for(int n = 1; n < _NSIG; ++n)
#endif
		{
			 if(n != SIGKILL && n != SIGSTOP)
				 signal(n, SIG_DFL);
		}

		// Reset the signal mask
		sigset_t ssDefault;
		sigemptyset(&ssDefault);
		sigprocmask(SIG_SETMASK, &ssDefault, nullptr);

		// Redirect stdin/stdout/stderr to /dev/null
		dup2(open("/dev/null", O_RDONLY), STDIN_FILENO);
		int nWrite = open("/dev/null", O_WRONLY);
		dup2(nWrite, STDOUT_FILENO);
		dup2(nWrite, STDERR_FILENO);

		PSTR *pszArgs = CreateArgs(strApp, strParams);
		PSTR *pszEnvVars = CreateEnvVars(pvecEnvVars);

#ifdef __APPLE__
		int nExecResult = execve(pszArgs[0], pszArgs, pszEnvVars);

		// execve does not check the path for the program, on MacOS we have to manually walk the path looking for the application
		if(!strApp.Contains('/'))
		{
			auto vecPaths = GetPathEnvironmentVar();
			CFilePath8 pathApp;
			for(const auto &itr : vecPaths)
			{
				pathApp = itr + CFilePathSegment8(strApp);
				nExecResult = execve(pathApp.Get(), pszArgs, pszEnvVars);	// No need to check the return from execve and stop the loop, if the spawn succeeds then this process will go away
			}
		}
#else
		int nExecResult = execvpe(pszArgs[0], pszArgs, pszEnvVars);
#endif
		// If we made it here then an error occurred, but this is the child process so just exit
		Assert(false);
		std::exit(nExecResult);
	}
	else if(nPid == -1)
		nErrorCode = ConvertFromNativeErrorCode(errno);
	else if(pnPid)
		*pnPid = nPid;

	return nErrorCode;
}

//#################################################################################################
ERRCODE SpawnProcess(const CStr8 &strApp, const CStr8 &strParams, const std::vector<CStr8> *pvecEnvVars, ERRCODE *pnErrorCodeChild, CStr8 *pstrStdOutput, CStr8 *pstrStdError, const bool bNewProcessGroup)
{
	Assert(!strApp.IsEmpty());

	ERRCODE nErrorCode = FW_NO_ERROR;

	if(pnErrorCodeChild)
		*pnErrorCodeChild = FW_ERROR_UNKNOWN;

	if(pstrStdOutput)
		pstrStdOutput->Empty();

	if(pstrStdError)
		pstrStdError->Empty();

	int pipeStdOutput[2];
	if(pipe(pipeStdOutput) != -1)
	{
		int pipeStdError[2];
		if(pipe(pipeStdError) != -1)
		{
			pid_t nPid = fork();
			if(nPid == 0)
			{	// Child process
				if(bNewProcessGroup)
					setsid();

				// Setup the pipes for stdout and stderr
				dup2(pipeStdOutput[PIPE_WRITE], STDOUT_FILENO);
				dup2(pipeStdError[PIPE_WRITE], STDERR_FILENO);
				close(pipeStdError[PIPE_READ]);
				close(pipeStdError[PIPE_WRITE]);
				close(pipeStdOutput[PIPE_READ]);
				close(pipeStdOutput[PIPE_WRITE]);

				// Close any open file descriptors
				int nNumber = (int)sysconf(_SC_OPEN_MAX);
				if(nNumber != -1)
				{
					for(int n = 3; n < nNumber; ++n)
						close(n);
				}

				// Reset all signal handlers
#ifdef __APPLE__
				for(int n = 1; n < __DARWIN_NSIG; ++n)
#else
				for(int n = 1; n < _NSIG; ++n)
#endif
				{
					if(n != SIGKILL && n != SIGSTOP)
						signal(n, SIG_DFL);
				}

				// Reset the signal mask
				sigset_t ssDefault;
				sigemptyset(&ssDefault);
				sigprocmask(SIG_SETMASK, &ssDefault, nullptr);

				PSTR *pszArgs = CreateArgs(strApp, strParams);
				PSTR *pszEnvVars = CreateEnvVars(pvecEnvVars);

#ifdef __APPLE__
				int nExecResult = execve(pszArgs[0], pszArgs, pszEnvVars);

				// execve does not check the path for the program, on MacOS we have to manually walk the path looking for the application
				if(!strApp.Contains('/'))
				{
					auto vecPaths = GetPathEnvironmentVar();
					CFilePath8 pathApp;
					for(const auto &itr : vecPaths)
					{
						pathApp = itr + CFilePathSegment8(strApp);
						nExecResult = execve(pathApp.Get(), pszArgs, pszEnvVars);	// No need to check the return from execve and stop the loop, if the spawn succeeds then this process will go away
					}
				}
#else
				int nExecResult = execvpe(pszArgs[0], pszArgs, pszEnvVars);
#endif
				// If we made it here then an error occurred, but this is the child process so just exit
				Assert(false);
				std::exit(nExecResult);
			}
			else if(nPid == -1)
				nErrorCode = ConvertFromNativeErrorCode(errno);
			else
			{
				close(pipeStdError[PIPE_WRITE]);
				pipeStdError[PIPE_WRITE] = -1;
				close(pipeStdOutput[PIPE_WRITE]);
				pipeStdOutput[PIPE_WRITE] = -1;

				if(pstrStdOutput || pstrStdError)
				{
					ssize_t nBytesRead;
					BYTE buffer[4096];
					bool bStdOutputOpen = (pstrStdOutput != nullptr);
					bool bStdErrorOpen = (pstrStdError != nullptr);

					while(bStdOutputOpen || bStdErrorOpen)
					{
						if(pstrStdOutput && bStdOutputOpen && IsStreamReady(pipeStdOutput[PIPE_READ]))
						{
							if((nBytesRead = read(pipeStdOutput[PIPE_READ], buffer, sizeof(buffer))) > 0)
								pstrStdOutput->Append((PCSTR)buffer, 0, nBytesRead);
							else
								bStdOutputOpen = false;
						}

						if(pstrStdError && bStdErrorOpen && IsStreamReady(pipeStdError[PIPE_READ]))
						{
							if((nBytesRead = read(pipeStdError[PIPE_READ], buffer, sizeof(buffer))) > 0)
								pstrStdError->Append((PCSTR)buffer, 0, nBytesRead);
							else
								bStdErrorOpen = false;
						}
					}
				}

				// Note: The following will fail if the process ignores/blocks SIGCHLD
				int nStatus = 0;
				while(waitpid(nPid, &nStatus, 0) != -1)
				{
					if(WIFEXITED(nStatus))
					{
						if(pnErrorCodeChild)
							*pnErrorCodeChild = WEXITSTATUS(nStatus);
						break;
					}
					else if(WIFSIGNALED(nStatus))
					{
						if(pnErrorCodeChild)
							*pnErrorCodeChild = WTERMSIG(nStatus);
						break;
					}
				}
			}

			close(pipeStdError[PIPE_READ]);
			if(pipeStdError[PIPE_WRITE] != -1)
				close(pipeStdError[PIPE_WRITE]);
		}
		else
			nErrorCode = ConvertFromNativeErrorCode(errno);

		close(pipeStdOutput[PIPE_READ]);
		if(pipeStdOutput[PIPE_WRITE] != -1)
			close(pipeStdOutput[PIPE_WRITE]);
	}
	else
		nErrorCode = ConvertFromNativeErrorCode(errno);

	return nErrorCode;
}

//#################################################################################################
PSTR *CreateArgs(const CStr8 &strApp, const CStr8 &strParams)
{
	PSTR *pszArgs;

	CWhitespaceParser cmdline(strParams);
	pszArgs = new PSTR[cmdline.GetCount() + 2];
	pszArgs[0] = new char[strApp.GetSize(true)];
	std::memcpy(pszArgs[0], strApp, strApp.GetSize(true));

	for(size_t n = 0; n < cmdline.GetCount(); ++n)
	{
		pszArgs[n + 1] = new char[cmdline.Get(n).GetSize(true)];
		std::memcpy(pszArgs[n + 1], cmdline.Get(n), cmdline.Get(n).GetSize(true));
	}
	pszArgs[cmdline.GetCount() + 1] = nullptr;

	return pszArgs;
}

//#################################################################################################
PSTR *CreateEnvVars(const std::vector<CStr8> *pvecEnvVars)
{
	PSTR *pszEnvVars = nullptr;

	std::map<CStr8, CStr8> mapNewVars;
	CStr8 str;
	CStr8 strVariable;
	CStr8 strValue;
	PSTR *pszEnviron = ::environ;
	for(size_t n = 0; pszEnviron[n] != nullptr; ++n)
	{
		str = pszEnviron[n];
		size_t nPos = str.Find('=');
		if(nPos != CStr8::not_found)
		{
			strVariable = str.GetLeft(nPos).Trim();
			strValue = str.GetMid(nPos + 1).Trim();
			mapNewVars[strVariable] = strValue;
		}
	}

	if(pvecEnvVars)
	{
		for(const auto &itr : *pvecEnvVars)
		{
			size_t nPos = itr.Find('=');
			if(nPos != CStr8::not_found)
			{
				strVariable = itr.GetLeft(nPos).Trim();
				strValue = itr.GetMid(nPos + 1).Trim();
				mapNewVars[strVariable] = strValue;
			}
		}
	}

	size_t nIndex = 0;
	pszEnvVars = new PSTR[mapNewVars.size() + 1];
	for(const auto &itr : mapNewVars)
	{
		str = itr.first + '=' + itr.second;
		pszEnvVars[nIndex] = new char[str.GetSize(true)];
		std::memcpy(pszEnvVars[nIndex], str, str.GetSize(true));
		++nIndex;
	}
	pszEnvVars[mapNewVars.size()] = nullptr;

	return pszEnvVars;
}

//#################################################################################################
bool IsStreamReady(const int fd)
{
	fd_set fds;
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	return (select(fd + 1, &fds, nullptr, nullptr, &tv) > 0 && FD_ISSET(fd, &fds));
}

#ifdef __APPLE__
//#################################################################################################
std::vector<CFilePath8> GetPathEnvironmentVar(void)
{
	std::vector<CFilePath8> vecPaths;

	CStr8 strPathVar = GetEnvVariable("PATH");
	size_t nStart = 0;
	CStr8 strPath = strPathVar.Tokenize(':', nStart);
	while(!strPath.IsEmpty())
	{
		vecPaths.emplace_back(strPath);
		strPath = strPathVar.Tokenize(':', nStart);
	}

	return vecPaths;
}

//#################################################################################################
static void ProcessTerminationCallback(CFFileDescriptorRef fdref, CFOptionFlags callBackTypes, PVOID info)
{	// Unregister the event
	struct kevent kev = {0};
	int nQueue = CFFileDescriptorGetNativeDescriptor(fdref);
	Verify(kevent(nQueue, nullptr, 0, &kev, 1, nullptr) != -1);

	CFRunLoopStop(CFRunLoopGetMain());
}

//#################################################################################################
bool WaitForProcess(const uint32_t nPid, const double dTimeoutSeconds)
{
	bool bResult = false;

	int nQueue = kqueue();
	if(nQueue != -1)
	{	// Register the event
		struct kevent kev = {0};
		EV_SET(&kev, nPid, EVFILT_PROC, EV_ADD | EV_ENABLE, NOTE_EXIT, 0, nullptr);
		if(kevent(nQueue, &kev, 1, nullptr, 0, nullptr) != -1)
		{
			bResult = true;		// Returning true indicates the process exited

			CCFObject cfFileDesc = CFFileDescriptorCreate(kCFAllocatorDefault, nQueue, true, ProcessTerminationCallback, nullptr);
			if(cfFileDesc)
			{
				CFFileDescriptorEnableCallBacks((CFFileDescriptorRef)cfFileDesc.Get(), kCFFileDescriptorReadCallBack);
				CCFObject cfRLS = CFFileDescriptorCreateRunLoopSource(kCFAllocatorDefault, (CFFileDescriptorRef)cfFileDesc.Get(), 0);
				if(cfRLS)
				{
					CCFObject cfRL(CFRunLoopGetMain(), false);
					CFRunLoopAddSource((CFRunLoopRef)cfRL.Get(), (CFRunLoopSourceRef)cfRLS.Get(), kCFRunLoopDefaultMode);
					if(FloatIsGreaterEqual(dTimeoutSeconds, 0.0))
					{
						if(CFRunLoopRunInMode(kCFRunLoopDefaultMode, dTimeoutSeconds, false) == kCFRunLoopRunTimedOut)
							bResult = false;	// Timed out
					}
					else
						CFRunLoopRun();

					CFRunLoopRemoveSource((CFRunLoopRef)cfRL.Get(), (CFRunLoopSourceRef)cfRLS.Get(), kCFRunLoopDefaultMode);
				}

				CFFileDescriptorInvalidate((CFFileDescriptorRef)cfFileDesc.Get());
			}
		}

		close(nQueue);
	}

	return bResult;
}

#endif
#endif

NS_END

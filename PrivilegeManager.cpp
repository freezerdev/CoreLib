#include "Base.h"
#include "PrivilegeManager.h"

NS_BEGIN

//#################################################################################################
CPrivilegeManager::~CPrivilegeManager(void)
{
	RevokeAll();
}

//#################################################################################################
bool CPrivilegeManager::Grant(PCNSTR szPrivilege, const EGrantLevel eLevel)
{
	Assert(szPrivilege);
	Assert(*szPrivilege != g_chNull);

	bool bGranted = false;

	BOOL bOpened = FALSE;
	HANDLE hToken = nullptr;

	if(eLevel == EGL_Thread)
	{
		bOpened = OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, false, &hToken);
		if(!bOpened && GetLastError() == ERROR_NO_TOKEN)
		{	// The thread does not have a token, so copy the process token to the thread
			HANDLE hProcess;
			if(OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE, &hProcess))
			{
				if(DuplicateTokenEx(hProcess, TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, nullptr, SecurityImpersonation, TokenImpersonation, &hToken))
				{
					if(SetThreadToken(nullptr, hToken))
						bOpened = TRUE;
					else
						CloseHandle(hToken);
				}

				CloseHandle(hProcess);
			}
		}
	}
	else if(eLevel == EGL_Process)
		bOpened = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	if(bOpened)
	{
		LUID luid = {0};
		if(LookupPrivilegeValueW(nullptr, szPrivilege, &luid))
		{
			DWORD dwSize;
			TOKEN_PRIVILEGES tp;

			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			GetTokenInformation(hToken, TokenPrivileges, nullptr, 0, &dwSize);
			if(dwSize)
			{
				auto pBuffer = std::make_unique<BYTE[]>(dwSize);
				PTOKEN_PRIVILEGES ptp = (PTOKEN_PRIVILEGES)pBuffer.get();

				if(GetTokenInformation(hToken, TokenPrivileges, ptp, dwSize, &dwSize))
				{
					for(DWORD dw = 0; dw < ptp->PrivilegeCount; ++dw)
					{
						if(std::memcmp(&tp.Privileges[0].Luid, &ptp->Privileges[dw].Luid, sizeof(LUID)) == 0)
						{
							if(HAS_FLAG(ptp->Privileges[dw].Attributes, SE_PRIVILEGE_ENABLED_BY_DEFAULT) || HAS_FLAG(ptp->Privileges[dw].Attributes, SE_PRIVILEGE_ENABLED))
								bGranted = true;
							break;
						}
					}

					if(!bGranted)
					{
						AdjustTokenPrivileges(hToken, false, &tp, 0, nullptr, nullptr);
						if(GetLastError() == NO_ERROR)
						{
							m_setPrivileges.emplace(szPrivilege, eLevel);
							bGranted = true;
						}
					}
				}
			}
		}

		CloseHandle(hToken);
	}

	return bGranted;
}

//#################################################################################################
bool CPrivilegeManager::Revoke(PCNSTR szPrivilege, const EGrantLevel eLevel)
{	// This function is capable of revoking a privilege even if it was not assigned by the above "Grant" function
	Assert(szPrivilege);
	Assert(*szPrivilege != g_chNull);

	bool bRevoked = false;

	BOOL bOpened = FALSE;
	HANDLE hToken = nullptr;

	if(eLevel == EGL_Thread)
		bOpened = OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, false, &hToken);
	else if(eLevel == EGL_Process)
		bOpened = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	if(bOpened)
	{
		LUID luid = {0};
		if(LookupPrivilegeValueW(nullptr, szPrivilege, &luid))
		{
			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = 0;

			AdjustTokenPrivileges(hToken, false, &tp, 0, nullptr, nullptr);
			if(GetLastError() == NO_ERROR)
			{
				m_setPrivileges.erase(std::pair<CStr, EGrantLevel>(szPrivilege, eLevel));
				bRevoked = true;
			}
		}

		CloseHandle(hToken);
	}

	return bRevoked;
}

//#################################################################################################
void CPrivilegeManager::RevokeAll(void)
{
	for(const auto &itr : m_setPrivileges)
	{
		BOOL bOpened = FALSE;
		HANDLE hToken = nullptr;

		if(itr.second == EGL_Thread)
			bOpened = OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, false, &hToken);
		else if(itr.second == EGL_Process)
			bOpened = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

		if(bOpened)
		{
			LUID luid = {0};
			if(LookupPrivilegeValueW(nullptr, itr.first, &luid))
			{
				TOKEN_PRIVILEGES tp;
				tp.PrivilegeCount = 1;
				tp.Privileges[0].Luid = luid;
				tp.Privileges[0].Attributes = 0;

				AdjustTokenPrivileges(hToken, false, &tp, 0, nullptr, nullptr);
				Assert(GetLastError() == NO_ERROR);
			}

			CloseHandle(hToken);
		}
	}

	m_setPrivileges.clear();
}

//#################################################################################################
bool CPrivilegeManager::IsEnabled(PCNSTR szPrivilege, const EGrantLevel eLevel)
{
	Assert(szPrivilege);
	Assert(*szPrivilege != g_chNull);

	bool bEnabled = false;

	BOOL bOpened = FALSE;
	HANDLE hToken = nullptr;

	if(eLevel == EGL_Thread)
		bOpened = OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, false, &hToken);
	else if(eLevel == EGL_Process)
		bOpened = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	if(bOpened)
	{
		LUID luid = {0};
		if(LookupPrivilegeValueW(nullptr, szPrivilege, &luid))
		{
			DWORD dwSize;
			TOKEN_PRIVILEGES tp;

			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			GetTokenInformation(hToken, TokenPrivileges, nullptr, 0, &dwSize);
			if(dwSize)
			{
				auto pBuffer = std::make_unique<BYTE[]>(dwSize);
				PTOKEN_PRIVILEGES ptp = (PTOKEN_PRIVILEGES)pBuffer.get();

				if(GetTokenInformation(hToken, TokenPrivileges, ptp, dwSize, &dwSize))
				{
					for(DWORD dw = 0; dw < ptp->PrivilegeCount; ++dw)
					{
						if(std::memcmp(&tp.Privileges[0].Luid, &ptp->Privileges[dw].Luid, sizeof(LUID)) == 0)
						{
							if(HAS_FLAG(ptp->Privileges[dw].Attributes, SE_PRIVILEGE_ENABLED_BY_DEFAULT) || HAS_FLAG(ptp->Privileges[dw].Attributes, SE_PRIVILEGE_ENABLED))
								bEnabled = true;
							break;
						}
					}
				}
			}
		}

		CloseHandle(hToken);
	}

	return bEnabled;
}

NS_END

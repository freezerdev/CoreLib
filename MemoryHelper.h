#pragma once

#include <winternl.h>

NS_BEGIN

using PFNZWQUERYINFORMATIONPROCESS = NTSTATUS(NTAPI*)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

//#################################################################################################
class CMemoryHelper final
{
public:
	CMemoryHelper(void);
	~CMemoryHelper(void);

	uint64_t GetProcessPrivateWorkingSet(const HANDLE hProcess) const;

private:
	HMODULE m_hNtDll;
	PFNZWQUERYINFORMATIONPROCESS m_pfnZwQueryInformationProcess;
};

NS_END

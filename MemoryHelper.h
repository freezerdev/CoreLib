#pragma once

#include <winternl.h>

NS_BEGIN

using PFNZWQUERYINFORMATIONPROCESS = NTSTATUS(NTAPI*)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

//#################################################################################################
class CMemoryHelper final
{
public:
	CMemoryHelper(void);
	// Copy and move constructors
	CMemoryHelper(const CMemoryHelper &src) = delete;
	CMemoryHelper(CMemoryHelper &&src) = delete;
	~CMemoryHelper(void);

	// Copy and move assignment operators
	CMemoryHelper &operator=(const CMemoryHelper &src) = delete;
	CMemoryHelper &operator=(CMemoryHelper &&src) = delete;

	uint64_t GetProcessPrivateWorkingSet(const HANDLE hProcess) const;

private:
	HMODULE m_hNtDll;
	PFNZWQUERYINFORMATIONPROCESS m_pfnZwQueryInformationProcess;
};

NS_END

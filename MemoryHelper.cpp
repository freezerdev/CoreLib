#include "Base.h"
#include "MemoryHelper.h"
#include "PlatformUtils.h"

NS_BEGIN

//#################################################################################################
CMemoryHelper::CMemoryHelper(void)
	: m_hNtDll(nullptr),
	m_pfnZwQueryInformationProcess(nullptr)
{
	m_hNtDll = SystemLoadLibrary(_N("ntdll.dll"));
	if(m_hNtDll)
		m_pfnZwQueryInformationProcess = (PFNZWQUERYINFORMATIONPROCESS)GetProcAddress(m_hNtDll, "ZwQueryInformationProcess");
}

//#################################################################################################
CMemoryHelper::~CMemoryHelper(void)
{
	if(m_hNtDll)
		FreeLibrary(m_hNtDll);
}

//#################################################################################################
uint64_t CMemoryHelper::GetProcessPrivateWorkingSet(const HANDLE hProcess) const
{
	Assert(hProcess);

	// The following structures are in lieu of including <ntddk.h> which is a part of the DDK
	struct VM_COUNTERS_EX final
	{
		SIZE_T PeakVirtualSize;
		SIZE_T VirtualSize;
		ULONG PageFaultCount;
		SIZE_T PeakWorkingSetSize;
		SIZE_T WorkingSetSize;
		SIZE_T QuotaPeakPagedPoolUsage;
		SIZE_T QuotaPagedPoolUsage;
		SIZE_T QuotaPeakNonPagedPoolUsage;
		SIZE_T QuotaNonPagedPoolUsage;
		SIZE_T PagefileUsage;
		SIZE_T PeakPagefileUsage;
		SIZE_T PrivateUsage;
	};

	struct VM_COUNTERS_EX2 final
	{
		VM_COUNTERS_EX CountersEx;
		SIZE_T PrivateWorkingSetSize;
		ULONGLONG SharedCommitUsage;
	};
	// End ntddk.h substitute

	static constexpr auto PROCESS_VM_COUNTERS = 3;

	uint64_t nPrivateWS = 0;

	if(m_pfnZwQueryInformationProcess)
	{	// GetProcessMemoryInfo() returns the same info as ZwQueryInformationProcess(), but ZwQueryInformationProcess() also returns the private working set size
		VM_COUNTERS_EX2 vm = {0};
		if(NT_SUCCESS(m_pfnZwQueryInformationProcess(hProcess, (PROCESSINFOCLASS)PROCESS_VM_COUNTERS, &vm, sizeof(vm), nullptr)))
			nPrivateWS = vm.PrivateWorkingSetSize;
	}

	return nPrivateWS;
}

NS_END

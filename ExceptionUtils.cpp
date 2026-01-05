#include "Base.h"
#include "ExceptionUtils.h"
#include "TimeUtils.h"
#include "FileSystemUtils.h"
#include "FilePathUtils.h"
#include <csignal>
#ifdef _WIN32
#include <dbghelp.h>
#include <intrin.h>
#include <new.h>
#else
#include "Thread.h"
#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <fcntl.h>
#endif

NS_BEGIN

static bool g_bEnableCrashDump = true;
static std::unique_ptr<CFilePath> g_pPathDumpFolder;

//#################################################################################################
void ExceptionInit(void)
{
	g_pPathDumpFolder = std::make_unique<CFilePath>();
	*g_pPathDumpFolder = GetExecutablePath().GetPath();
}

//#################################################################################################
void ExceptionFree(void)
{
	g_pPathDumpFolder.reset();
}

//#################################################################################################
bool GetCrashDumpEnabled(void) noexcept
{
	return g_bEnableCrashDump;
}

//#################################################################################################
void SetCrashDumpEnabled(const bool bEnableCrashDump) noexcept
{
	g_bEnableCrashDump = bEnableCrashDump;
}

//#################################################################################################
const CFilePath &GetProcessDumpFolder(void) noexcept
{
	return *g_pPathDumpFolder;
}

//#################################################################################################
void SetProcessDumpFolder(const CFilePath &pathDumpFolder)
{
	*g_pPathDumpFolder = pathDumpFolder;
}

#ifdef _WIN32
#pragma intrinsic(_AddressOfReturnAddress)

static bool g_bFullMemoryDump = true;

//#################################################################################################
bool IsFullMemoryDump(void) noexcept
{
	return g_bFullMemoryDump;
}

//#################################################################################################
void EnableFullMemoryDump(const bool bEnable) noexcept
{
	g_bFullMemoryDump = bEnable;
}

//#################################################################################################
void GetExceptionPointers(const DWORD dwExceptionCode, EXCEPTION_POINTERS &ep)
{
	EXCEPTION_RECORD er = {0};
	CONTEXT ctx = {0};

#ifdef _M_X64
	RtlCaptureContext(&ctx);
	DWORD64 nControlPc = ctx.Rip;
	DWORD64 nImageBase;
	PRUNTIME_FUNCTION FunctionEntry = RtlLookupFunctionEntry(nControlPc, &nImageBase, nullptr);
	if(FunctionEntry)
	{
		PVOID HandlerData;
		DWORD64 nEstablisherFrame;
		RtlVirtualUnwind(UNW_FLAG_NHANDLER, nImageBase, nControlPc, FunctionEntry, &ctx, &HandlerData, &nEstablisherFrame, nullptr);
	}
	else
	{
		ctx.Rip = (DWORD64)_ReturnAddress();
		ctx.Rsp = (DWORD64)_AddressOfReturnAddress();
	}
#elif _M_AMD64
	// TODO Add support for ARM exception pointers
#elif _M_IX86
	__asm
	{
		mov dword ptr [ctx.Eax], eax
		mov dword ptr [ctx.Ecx], ecx
		mov dword ptr [ctx.Edx], edx
		mov dword ptr [ctx.Ebx], ebx
		mov dword ptr [ctx.Esi], esi
		mov dword ptr [ctx.Edi], edi
		mov word ptr [ctx.SegSs], ss
		mov word ptr [ctx.SegCs], cs
		mov word ptr [ctx.SegDs], ds
		mov word ptr [ctx.SegEs], es
		mov word ptr [ctx.SegFs], fs
		mov word ptr [ctx.SegGs], gs
		pushfd
		pop [ctx.EFlags]
	}

	ctx.ContextFlags = CONTEXT_CONTROL;
	ctx.Eip = (DWORD)_ReturnAddress();
	ctx.Esp = (DWORD)_AddressOfReturnAddress();
	ctx.Ebp = *((PDWORD)_AddressOfReturnAddress() - 1);
#endif

	er.ExceptionCode = dwExceptionCode;
	er.ExceptionAddress = _ReturnAddress();

	PEXCEPTION_RECORD per = (PEXCEPTION_RECORD)std::malloc(sizeof(EXCEPTION_RECORD));
	CopyMemory(per, &er, sizeof(er));
	PCONTEXT pctx = (PCONTEXT)std::malloc(sizeof(CONTEXT));
	CopyMemory(pctx, &ctx, sizeof(ctx));

	ep.ExceptionRecord = per;
	ep.ContextRecord = pctx;
}

//#################################################################################################
LONG WINAPI UnhandledExceptionHandler(PEXCEPTION_POINTERS pep)
{
	CreateMiniDump(pep);
	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
	return EXCEPTION_EXECUTE_HANDLER;
}

//#################################################################################################
void PureCallExceptionHandler(void)
{
	EXCEPTION_POINTERS ep = {0};
	GetExceptionPointers(0, ep);
	CreateMiniDump(&ep);
	std::free(ep.ExceptionRecord);
	std::free(ep.ContextRecord);

	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
}

//#################################################################################################
int NewExceptionHandler(const size_t n)
{
	UNUSED(n);

	EXCEPTION_POINTERS ep = {0};
	GetExceptionPointers(0, ep);
	CreateMiniDump(&ep);
	std::free(ep.ExceptionRecord);
	std::free(ep.ContextRecord);

	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);

	return 0;
}

//#################################################################################################
void InvalidParameterExceptionHandler(PCWSTR szExpression, PCWSTR szFunction, PCWSTR szFile, unsigned int nLine, uintptr_t nReserved)
{
	UNUSED(szExpression);
	UNUSED(szFunction);
	UNUSED(szFile);
	UNUSED(nLine);
	UNUSED(nReserved);

	EXCEPTION_POINTERS ep = {0};
	GetExceptionPointers(0, ep);
	CreateMiniDump(&ep);
	std::free(ep.ExceptionRecord);
	std::free(ep.ContextRecord);

	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
}

//#################################################################################################
void SignalHandler(const int nSignalCode)
{
	UNUSED(nSignalCode);

	EXCEPTION_POINTERS ep = {0};
	GetExceptionPointers(0, ep);
	CreateMiniDump(&ep);
	std::free(ep.ExceptionRecord);
	std::free(ep.ContextRecord);

	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
}

//#################################################################################################
void SignalFloatingPointErrorHandler(const int nSignalCode, const int nSubcode)
{
	UNUSED(nSignalCode);
	UNUSED(nSubcode);

	// __pxcptinfoptrs is an alternate way to get the exception pointers, but GetExceptionPointers() appears to be more reliable
//	PEXCEPTION_POINTERS pep = (PEXCEPTION_POINTERS)__pxcptinfoptrs;
//	GetExceptionPointers(0, *pep);

	EXCEPTION_POINTERS ep = {0};
	GetExceptionPointers(0, ep);
	CreateMiniDump(&ep);
	std::free(ep.ExceptionRecord);
	std::free(ep.ContextRecord);

	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
}

//#################################################################################################
void TerminateHandler(void)
{
	EXCEPTION_POINTERS ep = {0};
	GetExceptionPointers(0, ep);
	CreateMiniDump(&ep);
	std::free(ep.ExceptionRecord);
	std::free(ep.ContextRecord);

	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
}

//#################################################################################################
void UnexpectedHandler(void)
{
	EXCEPTION_POINTERS ep = {0};
	GetExceptionPointers(0, ep);
	CreateMiniDump(&ep);
	std::free(ep.ExceptionRecord);
	std::free(ep.ContextRecord);

	TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
}

//#################################################################################################
void SetThreadExceptionHandlers(void)
{
	set_terminate(TerminateHandler);
	set_unexpected(UnexpectedHandler);
	signal(SIGFPE, (_crt_signal_t)SignalFloatingPointErrorHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGSEGV, SignalHandler);
}

//#################################################################################################
void CreateMiniDump(PEXCEPTION_POINTERS pep)
{
	if(!g_bEnableCrashDump)
		return;

	CFilePathW pathDumpFile;
	CStrW strFilename = GetExecutablePath().GetBase() + L'_' + GetFormattedTime(GetCurrentUtcTime(), true, L'-', L'-', L'_') + L".dmp";
	if(g_pPathDumpFolder && !g_pPathDumpFolder->IsEmpty())
	{
		pathDumpFile = *g_pPathDumpFolder;
		DirCreateTree(*g_pPathDumpFolder);
	}
	pathDumpFile += CFilePathSegmentW(strFilename);

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(pathDumpFile.Get(), EFM_CreateReadWrite, hFile) == FW_NO_ERROR)
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei = {0};
		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = pep;
		mdei.ClientPointers = false;

		MINIDUMP_TYPE eType;
		if(g_bFullMemoryDump)
			eType = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | MiniDumpWithFullMemoryInfo | MiniDumpWithHandleData | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules);
		else
			eType = (MINIDUMP_TYPE)(MiniDumpWithFullMemoryInfo | MiniDumpWithHandleData | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules);

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, eType, (pep != nullptr) ? &mdei : nullptr, nullptr, nullptr);

		FileClose(hFile);
	}
}

#else

//#################################################################################################
CStr8 GetStackTrace(void)
{
	CStr8 strTrace;

	PVOID callstack[128];
	const int nMaxFrames = COUNTOF(callstack);
	int nFrames = backtrace(callstack, nMaxFrames);
	PSTR *pszSymbols = backtrace_symbols(callstack, nFrames);
	Dl_info info;

	for(int n = 0; n < nFrames; ++n)
	{
		if(dladdr(callstack[n], &info) && info.dli_sname)
		{
			PSTR szDemangled = nullptr;
			int status = -1;
			if(info.dli_sname[0] == '_')
				szDemangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);

			if(status == 0)
				strTrace += CStr8(CStr8::EFT_Format, "{0} {1} {2} + {3}{4}", n, callstack[n], szDemangled, (PSTR)callstack[n] - (PSTR)info.dli_saddr, g_szEol);
			else
			{
				if(info.dli_sname == nullptr)
					strTrace += CStr8(CStr8::EFT_Format, "{0} {1} {2} + {3}{4}", n, callstack[n], pszSymbols[n], (PSTR)callstack[n] - (PSTR)info.dli_saddr, g_szEol);
				else
					strTrace += CStr8(CStr8::EFT_Format, "{0} {1} {2} + {3}{4}", n, callstack[n], info.dli_sname, (PSTR)callstack[n] - (PSTR)info.dli_saddr, g_szEol);
			}

			std::free(szDemangled);
		}
		else
			strTrace += CStr8(CStr8::EFT_Format, "{0} {1} {2}{3}", n, callstack[n], pszSymbols[n], g_szEol);
	}

	std::free(pszSymbols);

	if(nFrames == nMaxFrames)
	{
		strTrace += "[truncated]";
		strTrace += g_szEol;
	}

	return strTrace;
}

//#################################################################################################
void CreateMiniDump(const CStr8 &strDetails)
{
	if(!g_bEnableCrashDump)
		return;

	CFilePath8 pathDumpFile;
	CStr8 strFilename = GetExecutablePath().GetBase() + '_' + GetFormattedTime(GetCurrentUtcTime(), true, '-', '-', '_') + ".dmp";
	if(g_pPathDumpFolder && !g_pPathDumpFolder->IsEmpty())
	{
		pathDumpFile = *g_pPathDumpFolder;
		DirCreateTree(*g_pPathDumpFolder);
		Verify(FileSetPermissions(g_pPathDumpFolder->Get(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == FW_NO_ERROR);
	}
	pathDumpFile += CFilePathSegment8(strFilename);

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(pathDumpFile.Get(), EFM_CreateReadWrite, hFile) == FW_NO_ERROR)
	{
		size_t nBytesWritten;
		FileWrite(hFile, strDetails, strDetails.GetSize(), nBytesWritten);

		CStr8 strTrace = GetStackTrace();
		FileWrite(hFile, strTrace, strTrace.GetSize(), nBytesWritten);

		FileClose(hFile);
	}
}

//#################################################################################################
void ExceptionSignalHandler(const int nSig)
{
	CStr8 strDetails = "Unhandled core exception!  Time: " + GetFormattedTime() + ", Thread: " + CThread::GetThreadName() + ", Signal: " + IntegerToString8<int>(nSig) + g_szEol;
	CreateMiniDump(strDetails);
	std::cerr << strDetails;
	exit(EXIT_FAILURE);	// Ensure the program exits
}
#endif

//#################################################################################################
void SetProcessExceptionHandlers(void)
{
#ifdef _WIN32
	SetUnhandledExceptionFilter(UnhandledExceptionHandler);
	_set_purecall_handler(PureCallExceptionHandler);
	_set_new_handler(NewExceptionHandler);
	_set_invalid_parameter_handler(InvalidParameterExceptionHandler);
	_set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);
	signal(SIGABRT, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGTERM, SignalHandler);
#else
	struct sigaction saNew = {0};
	saNew.sa_handler = ExceptionSignalHandler;
	sigemptyset(&saNew.sa_mask);
	saNew.sa_flags = 0;

	sigaction(SIGABRT, &saNew, nullptr);	// Abort signal from abort()
	sigaction(SIGBUS, &saNew, nullptr);		// Bus error (bad memory access)
	sigaction(SIGFPE, &saNew, nullptr);		// Floating point exception
	sigaction(SIGILL, &saNew, nullptr);		// Illegal instruction
	sigaction(SIGIOT, &saNew, nullptr);		// IOT trap, same as SIGABRT
	sigaction(SIGQUIT, &saNew, nullptr);	// Quit from keyboard
	sigaction(SIGSEGV, &saNew, nullptr);	// Invalid memory reference
	sigaction(SIGSYS, &saNew, nullptr);		// Bad argument to routine
	sigaction(SIGTRAP, &saNew, nullptr);	// Trace/breakpoint trap
	sigaction(SIGXCPU, &saNew, nullptr);	// CPU time limit exceeded
	sigaction(SIGXFSZ, &saNew, nullptr);	// File size limit exceeded
#ifdef __APPLE__
	sigaction(SIGEMT, &saNew, nullptr);		// Emulation instruction executed
#endif
#endif
}

NS_END

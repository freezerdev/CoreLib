#pragma once

NS_BEGIN

void ExceptionInit(void);
void ExceptionFree(void);

bool GetCrashDumpEnabled(void) noexcept;
void SetCrashDumpEnabled(const bool bEnableCrashDump = true) noexcept;

const CFilePath &GetProcessDumpFolder(void) noexcept;
void SetProcessDumpFolder(const CFilePath &pathDumpFolder);

void SetProcessExceptionHandlers(void);

#ifdef _WIN32
bool IsFullMemoryDump(void) noexcept;
void EnableFullMemoryDump(const bool bEnable = true) noexcept;

void SetThreadExceptionHandlers(void);
void CreateMiniDump(PEXCEPTION_POINTERS pep);
#else
CStr8 GetStackTrace(void);
void CreateMiniDump(const CStr8 &strDetails);
#endif

NS_END

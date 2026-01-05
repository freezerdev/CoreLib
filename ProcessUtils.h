#pragma once

#ifndef _WIN32
#include <vector>
#endif

NS_BEGIN

uint32_t GetProcessId(void);
CFilePath GetProcessFilename(const uint32_t nPid);
CStr GetProcessCommandLine(const uint32_t nPid);

#ifdef _WIN32
ERRCODE SpawnProcessAsync(const CStrW &strApp, const CStrW &strParams = CStrW(), const bool bHidden = true, uint32_t *pnPid = nullptr);
ERRCODE SpawnProcess(const CStrW &strApp, const CStrW &strParams = CStrW(), const bool bHidden = true, ERRCODE *pnErrorCodeChild = nullptr, CStrW *pstrStdOutput = nullptr, CStrW *pstrStdError = nullptr);
#else
// Note: Linux will not spawn applications in the current working directory unless you add "./" before the program name
ERRCODE SpawnProcessAsync(const CStr8 &strApp, const CStr8 &strParams = CStr8(), const std::vector<CStr8> *pvecEnvVars = nullptr, pid_t *pnPid = nullptr, const bool bNewProcessGroup = false);
ERRCODE SpawnProcess(const CStr8 &strApp, const CStr8 &strParams = CStr8(), const std::vector<CStr8> *pvecEnvVars = nullptr, ERRCODE *pnErrorCodeChild = nullptr, CStr8 *pstrStdOutput = nullptr, CStr8 *pstrStdError = nullptr, const bool bNewProcessGroup = false);
#endif

#ifdef __APPLE__
// Returns true when the process exits, returns false if the process does not exist or if it timed out - note negative timeout values wait indefinitely
bool WaitForProcess(const pid_t nPid, const double dTimeoutSeconds = -1.0);
#endif

NS_END

#pragma once

#include <cmath>
#include <vector>

#ifndef _WIN32
#include <uuid/uuid.h>
#endif

NS_BEGIN

// Forward declarations
class CMemBuffer;

#ifdef _WIN32
HINSTANCE GetApplicationInstance(void);

// Windows headers interfere with these definitions
#undef UuidToString
#endif

std::vector<CStr> GetCommandLineArguments(void);

CStr GetFriendlyMachineName(void);
CStr GetMachineName(void);
CStr GetFQDNName(void);
CStr GetMachineDomain(void);
CStr GetSessionUsername(void);

enum EArch : uint8_t
{
	EA_Unknown = 0,
	EA_x86,
	EA_x64,
	EA_arm32,
	EA_arm64
};

EArch GetSystemArchitecture(void);

bool IsVirtualMachine(void);

CStr GetEnvVariable(PCNSTR szVariable);

#ifdef _WIN32
void ConsolePrint(PCWSTR szMessage, PCWSTR szCaption, PCWSTR szGuiMessage = nullptr);
#else
void ConsolePrint(PCSTR szMessage);
#endif

inline void SetMemoryZero(PVOID pDest, const size_t nSize)
{
#ifdef _WIN32
	// On Windows the compiler may optimize and remove calls to std::memset, so call SecureZeroMemory instead
	SecureZeroMemory(pDest, nSize);
#else
	std::memset(pDest, 0, nSize);
#endif
}

CStr8 UuidToString8(const uuid_t &uuid, const bool bAddBraces = false, const bool bUppercase = false);
CStrW UuidToStringW(const uuid_t &uuid, const bool bAddBraces = false, const bool bUppercase = false);
void StringToUuid8(const CStr8 &strUuid, uuid_t &uuid);
void StringToUuidW(const CStrW &strUuid, uuid_t &uuid);
void AddUuidBraces8(CStr8 &strUuid);
void AddUuidBracesW(CStrW &strUuid);
void RemoveUuidBraces8(CStr8 &strUuid);
void RemoveUuidBracesW(CStrW &strUuid);
bool IsValidUuid8(const CStr8 &strUuid);
bool IsValidUuidW(const CStrW &strUuid);
CStr8 GenerateNewUuid8(const bool bAddBraces = false, const bool bUppercase = false);
CStrW GenerateNewUuidW(const bool bAddBraces = false, const bool bUppercase = false);
void GenerateUuid(uuid_t &uuid);

CStr8 UUEncode8(const CMemBuffer &bufDecoded);
CStr8 UUEncode8(PCBYTE pDecoded, const size_t nDecodedSize);
CStrW UUEncodeW(const CMemBuffer &bufDecoded);
CStrW UUEncodeW(PCBYTE pDecoded, const size_t nDecodedSize);
bool UUDecode8(const CStr8 &strEncoded, CMemBuffer &bufDecoded);
bool UUDecode8(const CStr8 &strEncoded, PBYTE pDecoded, size_t &nDecodedSize);
bool UUDecodeW(const CStrW &strEncoded, CMemBuffer &bufDecoded);
bool UUDecodeW(const CStrW &strEncoded, PBYTE pDecoded, size_t &nDecodedSize);

// Floating point comparison functions; tests like (f1 == f2) do not always yield the expected results
template<typename FLOATTYPE>
inline bool FloatIsEqual(const FLOATTYPE f1, const FLOATTYPE f2, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f1 == f2
	return std::fabs(f1 - f2) <= std::fmax(std::fabs(f1), std::fabs(f2)) * fEpsilon;
}

template<typename FLOATTYPE>
inline bool FloatIsGreaterEqual(const FLOATTYPE f1, const FLOATTYPE f2, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f1 >= f2
	return (f1 > f2) || FloatIsEqual(f1, f2, fEpsilon);
}

template<typename FLOATTYPE>
inline bool FloatIsLessEqual(const FLOATTYPE f1, const FLOATTYPE f2, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f1 <= f2
	return (f1 < f2) || FloatIsEqual(f1, f2, fEpsilon);
}

template<typename FLOATTYPE>
inline bool FloatIsZero(const FLOATTYPE f, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f == 0.0F
	return (std::fabs(f) <= fEpsilon);
}

#ifdef _WIN32
#define UuidToString UuidToStringW
#define StringToUuid StringToUuidW
#define AddUuidBraces AddUuidBracesW
#define RemoveUuidBraces RemoveUuidBracesW
#define IsValidUuid IsValidUuidW
#define GenerateNewUuid GenerateNewUuidW
#define UUEncode UUEncodeW
#define UUDecode UUDecodeW
#else
#define UuidToString UuidToString8
#define StringToUuid StringToUuid8
#define AddUuidBraces AddUuidBraces8
#define RemoveUuidBraces RemoveUuidBraces8
#define IsValidUuid IsValidUuid8
#define GenerateNewUuid GenerateNewUuid8
#define UUEncode UUEncode8
#define UUDecode UUDecode8
#endif

CStr GetFriendlyErrorName(const ERRCODE nErrorCode);

NS_END

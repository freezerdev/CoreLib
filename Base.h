#pragma once

#ifdef _WIN32
//#################################################################################################
// Windows specific headers
#ifndef STRICT
#define STRICT
#endif

// Target Windows Vista/2008 and newer
#ifndef WINVER
#define WINVER			0x0600
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0600
#endif

#define VC_EXTRALEAN

#define NOMINMAX
#include <winsock2.h>
#include <windows.h>

using PCBYTE = const unsigned char*;
using PCVOID = const void*;

#ifdef _M_X64
using ssize_t = long long;
#else
using ssize_t = int;
#endif

// Defines for native handle, socket, character, and string types
using NHANDLE = HANDLE;
constexpr auto INVALID_NHANDLE = nullptr;

using NSOCKET = SOCKET;
constexpr auto INVALID_NSOCKET = ((SOCKET)-1);

using NCHAR = wchar_t;
using PNSTR = wchar_t*;
using PCNSTR = const wchar_t*;
#define _N(x)	L ## x

constexpr PCSTR g_szEol8 = "\r\n";
constexpr PCWSTR g_szEolW = L"\r\n";
constexpr const char16_t *g_szEol16 = u"\r\n";
constexpr size_t g_nEolLen = 2;
#define g_szEol	g_szEolW

// FILETIME_NULL (similar to GUID_NULL) as well as op== and op!=
constexpr FILETIME FILETIME_NULL = {0x00000000L, 0x00000000L};
inline bool operator==(const FILETIME &ft1, const FILETIME &ft2) {return (CompareFileTime(&ft1, &ft2) == 0);}
inline bool operator!=(const FILETIME &ft1, const FILETIME &ft2) {return (CompareFileTime(&ft1, &ft2) != 0);}

#elif __APPLE__
//#################################################################################################
// MacOS specific headers
#include <unistd.h>
#include <sys/syslimits.h>

using PCSTR = const char*;
using PSTR = char*;
using PCWSTR = const wchar_t*;
using PWSTR = wchar_t*;
using PCBYTE = const unsigned char*;
using PBYTE = unsigned char*;
using BYTE = unsigned char;
using PCVOID = const void*;
using PVOID = void*;

// Defines for native handle, socket, character, and string types
using NHANDLE = int;
constexpr auto INVALID_NHANDLE = -1;

using NSOCKET = int;
constexpr auto INVALID_NSOCKET = -1;

using NCHAR = char;
using PNSTR = char*;
using PCNSTR = const char*;
#define _N(x)	x

#if !defined(NDEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif

constexpr auto MAX_PATH = PATH_MAX;

constexpr PCSTR g_szEol8 = "\n";
constexpr PCWSTR g_szEolW = L"\n";
constexpr const char16_t *g_szEol16 = u"\n";
constexpr size_t g_nEolLen = 1;
#define g_szEol	g_szEol8

struct FILETIME final
{
	unsigned int dwLowDateTime;
	unsigned int dwHighDateTime;
};

union ULARGE_INTEGER
{
	struct
	{
		unsigned int LowPart;
		unsigned int HighPart;
	};
	unsigned long long QuadPart;
};

#ifndef DEFINE_UUID
#define DEFINE_UUID(name, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16)	const uuid_t name = {b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16}
#endif

#ifndef AGENT_NAME
#define AGENT_NAME "com.freezerware"
#endif

#elif __linux__
//#################################################################################################
// Linux specific headers
#include <unistd.h>
#include <cstdarg>
#include <cstring>
#include <linux/limits.h>

using PCSTR = const char*;
using PSTR = char*;
using PCWSTR = const wchar_t*;
using PWSTR = wchar_t*;
using PCBYTE = const unsigned char*;
using PBYTE = unsigned char*;
using BYTE = unsigned char;
using PCVOID = const void*;
using PVOID = void*;

// Defines for native handle, socket, character, and string types
using NHANDLE = int;
constexpr auto INVALID_NHANDLE = -1;

using NSOCKET = int;
constexpr auto INVALID_NSOCKET = -1;

using NCHAR = char;
using PNSTR = char*;
using PCNSTR = const char*;
#define _N(x)	x

#if !defined(NDEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif

constexpr auto MAX_PATH = PATH_MAX;

constexpr PCSTR g_szEol8 = "\n";
constexpr PCWSTR g_szEolW = L"\n";
constexpr const char16_t *g_szEol16 = u"\n";
constexpr size_t g_nEolLen = 1;
#define g_szEol	g_szEol8

struct FILETIME final
{
	unsigned int dwLowDateTime;
	unsigned int dwHighDateTime;
};

union ULARGE_INTEGER
{
	struct
	{
		unsigned int LowPart;
		unsigned int HighPart;
	};
	unsigned long long QuadPart;
};

#endif


//#################################################################################################
// Common macros
#define NS_BEGIN	namespace Freezerware {
#define NS_END		}
#define NS_USE		using namespace Freezerware;

#ifndef MIN
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)	((a) < (b) ? (b) : (a))
#endif

// Returns x bound by min and max. The returned value is guaranteed between min and max.
#ifndef BOUND
#define BOUND(x, min, max)	((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

#ifndef MAKEUINT64
#define MAKEUINT64(high, low)	((uint64_t)((((uint64_t)((uint32_t)(high))) << 32) | ((uint32_t)(low))))
#endif

#ifndef MAKEUINT32
#define MAKEUINT32(high, low)	((uint32_t)((((uint32_t)((uint16_t)(high))) << 16) | ((uint16_t)(low))))
#endif

#ifndef MAKEUINT16
#define MAKEUINT16(high, low)	((uint16_t)((((uint16_t)((uint8_t)(high))) << 8) | ((uint8_t)(low))))
#endif

#ifndef LO_UINT32
#define LO_UINT32(n)	((uint32_t)(n))
#endif

#ifndef HI_UINT32
#define HI_UINT32(n)	((uint32_t)((uint64_t)(n) >> 32))
#endif

#ifndef LO_UINT16
#define LO_UINT16(n)	((uint16_t)(n))
#endif

#ifndef HI_UINT16
#define HI_UINT16(n)	((uint16_t)((uint32_t)(n) >> 16))
#endif

#ifndef LO_UINT8
#define LO_UINT8(n)		((uint8_t)(n))
#endif

#ifndef HI_UINT8
#define HI_UINT8(n)		((uint8_t)((uint16_t)(n) >> 8))
#endif

#ifndef HAS_FLAG
#define HAS_FLAG(var, flag)	(((var) & (flag)) == (flag))
#endif

#ifndef ADD_FLAG
#define ADD_FLAG(var, flag)	((var) |= (flag))
#endif

#ifndef REMOVE_FLAG
#define REMOVE_FLAG(var, flag)	((var) &= ~(flag))
#endif

#ifndef IS_EVEN
#define IS_EVEN(n)	(((n) & 1) == 0)
#endif

#ifndef IS_ODD
#define IS_ODD(n)	(((n) & 1) != 0)
#endif

// Swap two identically sized variables
#define SWAP(a, b)			\
	{						\
		static_assert(sizeof(a) == sizeof(b), "Mismatched variable size");\
		auto _tmp = (a);	\
		(a) = (b);			\
		(b) = _tmp;			\
	}

// Macro to eliminate unused variable compiler warnings
#ifndef UNUSED
#define UNUSED(x)	(void)(x)
#endif


//#################################################################################################
// Common defines
using ERRCODE = unsigned int;

constexpr char g_chNull8 = '\0';
constexpr wchar_t g_chNullW = L'\0';
constexpr char16_t g_chNull16 = u'\0';

constexpr PCSTR g_szNull8 = "";
constexpr PCWSTR g_szNullW = L"";
constexpr const char16_t *g_szNull16 = u"";

constexpr BYTE g_Utf8_Bom[3] = {0xEF, 0xBB, 0xBF};
constexpr BYTE g_Utf16LE_Bom[2] = {0xFF, 0xFE};
constexpr BYTE g_Utf16BE_Bom[2] = {0xFE, 0xFF};
constexpr BYTE g_Utf32LE_Bom[4] = {0xFF, 0xFE, 0x00, 0x00};
constexpr BYTE g_Utf32BE_Bom[4] = {0x00, 0x00, 0xFE, 0xFF};

template<typename T, size_t N>
constexpr size_t COUNTOF(T (&)[N]) noexcept {return N;}

#ifdef _WIN32
#define g_chNull	g_chNullW
#define g_szNull	g_szNullW
#else
#define g_chNull	g_chNull8
#define g_szNull	g_szNull8
#endif


//#################################################################################################
// Common headers
#include <iostream>
#include <chrono>
#include <memory>
#include <algorithm>
#include "Errors.h"
#include "StrUtils.h"
#include "Str.h"
#include "Diagnostic.h"
#include "CoreLib.h"
#include "CoreUtils.h"
#include "FilePath.h"

// The chrono namespace makes type definitions very long, so "use" the namespace to simplify things
using namespace std::chrono;

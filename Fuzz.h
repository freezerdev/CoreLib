#pragma once

#include "Base.h"
#include "RandUtils.h"
#include <limits>

NS_BEGIN

//#################################################################################################
PBYTE GetRandomBuffer(const size_t nLen, const bool bAllowNulls = true)
{	// The caller must free the returned buffer
	PBYTE buf = (PBYTE)std::malloc(nLen);
	if(buf)
	{
		for(size_t n = 0; n < nLen; ++n)
			buf[n] = (BYTE)CRandom::GetInteger<uint16_t>(bAllowNulls ? 0 : 1, std::numeric_limits<BYTE>::max());
	}

	return buf;
}

//#################################################################################################
PSTR GetRandomString8(const size_t nLen, const bool bAllowNulls = false)
{	// The caller must free the returned buffer
	PSTR sz = (PSTR)std::malloc(nLen + 1);
	if(sz)
	{
		for(size_t n = 0; n < nLen; ++n)
			sz[n] = (char)CRandom::GetInteger<uint16_t>(bAllowNulls ? 0 : 1, std::numeric_limits<unsigned char>::max());
		sz[nLen] = '\0';
	}

	return sz;
}

//#################################################################################################
PWSTR GetRandomStringW(const size_t nLen, const bool bAllowNulls = false)
{	// The caller must free the returned buffer
	PWSTR sz = (PWSTR)std::malloc((nLen + 1) * sizeof(wchar_t));
	if(sz)
	{
		for(size_t n = 0; n < nLen; ++n)
#ifdef _WIN32
			sz[n] = (wchar_t)CRandom::GetInteger<uint16_t>(bAllowNulls ? 0 : 1, std::numeric_limits<wchar_t>::max());
#else
			sz[n] = (wchar_t)CRandom::GetInteger<uint32_t>(bAllowNulls ? 0 : 1, std::numeric_limits<wchar_t>::max());
#endif
		sz[nLen] = L'\0';
	}

	return sz;
}

//#################################################################################################
char16_t *GetRandomString16(const size_t nLen, const bool bAllowNulls = false)
{	// The caller must free the returned buffer
	char16_t *sz = (char16_t*)std::malloc((nLen + 1) * sizeof(char16_t));
	if(sz)
	{
		for(size_t n = 0; n < nLen; ++n)
			sz[n] = (char16_t)CRandom::GetInteger<uint16_t>(bAllowNulls ? 0 : 1, std::numeric_limits<char16_t>::max());
		sz[nLen] = u'\0';
	}

	return sz;
}

//#################################################################################################
CStr8 GetRandomStr8(const size_t nLen)
{
	CStr8 str;
	str.Attach(GetRandomString8(nLen, false), nLen + 1);
	return str;
}

//#################################################################################################
CStrW GetRandomStrW(const size_t nLen)
{
	CStrW str;
	str.Attach(GetRandomStringW(nLen, false), nLen + 1);
	return str;
}

//#################################################################################################
CStr16 GetRandomStr16(const size_t nLen)
{
	CStr16 str;
	str.Attach(GetRandomString16(nLen, false), nLen + 1);
	return str;
}

#ifdef _WIN32
#define GetRandomString GetRandomStringW
#define GetRandomStr GetRandomStrW
#else
#define GetRandomString GetRandomString8
#define GetRandomStr GetRandomStr8
#endif

NS_END

#include "Base.h"
#include "DoubleNullStr.h"
#include "StrUtils.h"

NS_BEGIN

static PCSTR g_szDoubleNull8 = "\0\0";
static PCWSTR g_szDoubleNullW = L"\0\0";

/////////////////////////////////////////////////
// CDoubleNullStr8
/////////////////////////////////////////////////

//#################################################################################################
CDoubleNullStr8::CDoubleNullStr8(void)
	: m_szBuffer((PSTR)g_szDoubleNull8),
	m_nBufLen(2),
	m_nCount(0)
{
}

//#################################################################################################
CDoubleNullStr8::CDoubleNullStr8(const CDoubleNullStr8 &src)
	: m_szBuffer((PSTR)g_szDoubleNull8),
	m_nBufLen(2),
	m_nCount(0)
{
	Assign(src);
}

//#################################################################################################
CDoubleNullStr8::CDoubleNullStr8(CDoubleNullStr8 &&src) noexcept
	: m_szBuffer(src.m_szBuffer),
	m_nBufLen(src.m_nBufLen),
	m_nCount(src.m_nCount)
{
	src.m_szBuffer = (PSTR)g_szDoubleNull8;
	src.m_nBufLen = 2;
	src.m_nCount = 0;
}

//#################################################################################################
CDoubleNullStr8::CDoubleNullStr8(PCSTR sz, const bool bDoubleNullTerm)
	: m_szBuffer((PSTR)g_szDoubleNull8),
	m_nBufLen(2),
	m_nCount(0)
{
	Assign(sz, bDoubleNullTerm);
}

//#################################################################################################
CDoubleNullStr8::~CDoubleNullStr8(void)
{
	Empty();
}

//#################################################################################################
size_t CDoubleNullStr8::GetLength(void) const
{	// Length in characters (including null-terminators)
	size_t nTotalStrLen = 0;

	if(m_nCount)
	{
		PCSTR szScan = m_szBuffer;
		for(size_t n = 0; n < m_nCount; ++n)
		{
			size_t nStrLen = StringGetLength(szScan);
			if(nStrLen == 0)
				break;
			szScan += nStrLen + 1;
			nTotalStrLen += nStrLen + 1;
		}
	}
	else
		nTotalStrLen = 1;

	return nTotalStrLen + 1;
}

//#################################################################################################
size_t CDoubleNullStr8::GetSize(void) const
{	// Size in bytes (including null-terminators)
	return GetLength();
}

//#################################################################################################
CDoubleNullStr8::operator PCSTR(void) const noexcept
{
	return m_szBuffer;
}

//#################################################################################################
bool CDoubleNullStr8::IsEmpty(void) const noexcept
{
	return (m_nCount == 0);
}

//#################################################################################################
void CDoubleNullStr8::Empty(void)
{
	if(m_nCount)
		std::free(m_szBuffer);
	m_szBuffer = (PSTR)g_szDoubleNull8;
	m_nBufLen = 2;
	m_nCount = 0;
}

//#################################################################################################
CDoubleNullStr8 &CDoubleNullStr8::operator=(const CDoubleNullStr8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CDoubleNullStr8 &CDoubleNullStr8::operator=(CDoubleNullStr8 &&src) noexcept
{
	if(this != &src)
	{
		if(m_nCount)
			std::free(m_szBuffer);

		m_szBuffer = src.m_szBuffer;
		m_nBufLen = src.m_nBufLen;
		m_nCount = src.m_nCount;

		src.m_szBuffer = (PSTR)g_szDoubleNull8;
		src.m_nBufLen = 2;
		src.m_nCount = 0;
	}

	return *this;
}

//#################################################################################################
CDoubleNullStr8 &CDoubleNullStr8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
void CDoubleNullStr8::Assign(const CDoubleNullStr8 &str)
{
	if(this != &str)
	{
		Empty();

		if(str.m_nCount)
		{
			m_szBuffer = (PSTR)std::malloc(str.m_nBufLen);
			if(m_szBuffer)
			{
				std::memcpy(m_szBuffer, str.m_szBuffer, str.m_nBufLen);
				m_nBufLen = str.m_nBufLen;
				m_nCount = str.m_nCount;
			}
		}
	}
}

//#################################################################################################
void CDoubleNullStr8::Assign(PCSTR sz, const bool bDoubleNullTerm)
{
	Empty();

	PCSTR szScan = sz;
	size_t nTotalStrLen = 0;
	size_t nCount = 0;

	do{
		size_t nStrLen = StringGetLength(szScan);
		if(nStrLen == 0)
			break;
		szScan += nStrLen + 1;
		nTotalStrLen += nStrLen + 1;
		++nCount;
	}while(bDoubleNullTerm);

	if(nTotalStrLen > 1)
	{
		m_szBuffer = (PSTR)std::malloc(nTotalStrLen + 1);
		if(m_szBuffer)
		{
			std::memcpy(m_szBuffer, sz, nTotalStrLen);
			m_szBuffer[nTotalStrLen] = g_chNull8;
			m_nBufLen = nTotalStrLen + 1;
			m_nCount = nCount;
		}
	}
}

//#################################################################################################
CDoubleNullStr8 &CDoubleNullStr8::operator+=(const CDoubleNullStr8 &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CDoubleNullStr8 &CDoubleNullStr8::operator+=(PCSTR sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
void CDoubleNullStr8::Append(const CDoubleNullStr8 &str)
{
	if(this != &str && str.m_nCount)
	{
		if(m_nCount == 0)
		{
			m_szBuffer = (PSTR)std::malloc(str.m_nBufLen);
			if(m_szBuffer)
			{
				std::memcpy(m_szBuffer, str.m_szBuffer, str.m_nBufLen);
				m_nBufLen = str.m_nBufLen;
			}
		}
		else
		{
			PSTR szTemp = m_szBuffer;
			m_szBuffer = (PSTR)std::malloc(m_nBufLen + str.m_nBufLen - 1);
			if(m_szBuffer)
			{
				std::memcpy(m_szBuffer, szTemp, m_nBufLen - 1);
				std::memcpy(m_szBuffer + m_nBufLen - 1, str.m_szBuffer, str.m_nBufLen);
				m_nBufLen += str.m_nBufLen - 1;
				std::free(szTemp);
			}
		}

		++m_nCount;
	}
}

//#################################################################################################
void CDoubleNullStr8::Append(PCSTR sz, const bool bDoubleNullTerm)
{
	PCSTR szScan = sz;
	size_t nTotalStrLen = 0;
	size_t nCount = 0;

	do{
		size_t nStrLen = StringGetLength(szScan);
		if(nStrLen == 0)
			break;
		szScan += nStrLen + 1;
		nTotalStrLen += nStrLen + 1;
		++nCount;
	}while(bDoubleNullTerm);

	if(nTotalStrLen > 1)
	{
		if(m_nCount == 0)
		{
			m_szBuffer = (PSTR)std::malloc(nTotalStrLen + 1);
			if(m_szBuffer)
			{
				std::memcpy(m_szBuffer, sz, nTotalStrLen);
				m_szBuffer[nTotalStrLen] = g_chNull8;
				m_nBufLen = nTotalStrLen + 1;
			}
		}
		else
		{
			PSTR szTemp = m_szBuffer;
			m_szBuffer = (PSTR)std::malloc(m_nBufLen + nTotalStrLen);
			if(m_szBuffer)
			{
				std::memcpy(m_szBuffer, szTemp, m_nBufLen - 1);
				std::memcpy(m_szBuffer + m_nBufLen - 1, sz, nTotalStrLen);
				m_szBuffer[m_nBufLen + nTotalStrLen - 1] = g_chNull8;
				m_nBufLen += nTotalStrLen;
			}
			std::free(szTemp);
		}

		m_nCount += nCount;
	}
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CDoubleNullStr8 &str)
{
	PCSTR *arr = str.GetArray();
	for(size_t n = 0; n < str.GetCount(); ++n)
	{
		if(n)
			stream << '|';	// Use a pipe character to separate each element in the string array
		stream << arr[n];
	}
	CDoubleNullStr8::FreeArray(arr);

	return stream;
}

//#################################################################################################
size_t CDoubleNullStr8::GetCount(void) const noexcept
{
	return m_nCount;
}

//#################################################################################################
PCSTR CDoubleNullStr8::GetAt(const size_t nIndex) const
{
	PCSTR sz = nullptr;

	if(nIndex < m_nCount)
	{
		PCSTR szScan = m_szBuffer;

		size_t n = nIndex;
		while(n--)
		{
			size_t nStrLen = StringGetLength(szScan);
			if(nStrLen == 0)
				break;
			szScan += nStrLen + 1;
		}

		sz = szScan;
	}

	return sz;
}

//#################################################################################################
PCSTR *CDoubleNullStr8::GetArray(void) const
{
	PCSTR *pszArr = nullptr;

	if(m_nCount)
	{
		pszArr = (PCSTR*)std::calloc(m_nCount, sizeof(PCSTR));		// Caller must call FreeArray() to free this memory
		if(pszArr)
		{
			PCSTR szScan = m_szBuffer;
			for(size_t n = 0; n < m_nCount; ++n)
			{
				pszArr[n] = szScan;
				size_t nStrLen = StringGetLength(szScan);
				if(nStrLen == 0)
					break;
				szScan += nStrLen + 1;
			}
		}
	}

	return pszArr;
}

//#################################################################################################
void CDoubleNullStr8::FreeArray(PVOID szArr)
{
	std::free(szArr);
}

//#################################################################################################
size_t CDoubleNullStr8::GetBufferLength(void) const noexcept
{
	return m_nBufLen;
}

//#################################################################################################
size_t CDoubleNullStr8::GetBufferSize(void) const noexcept
{
	return m_nBufLen;
}




/////////////////////////////////////////////////
// CDoubleNullStrW
/////////////////////////////////////////////////

//#################################################################################################
CDoubleNullStrW::CDoubleNullStrW(void)
	: m_szBuffer((PWSTR)g_szDoubleNullW),
	m_nBufLen(2),
	m_nCount(0)
{
}

//#################################################################################################
CDoubleNullStrW::CDoubleNullStrW(const CDoubleNullStrW &src)
	: m_szBuffer((PWSTR)g_szDoubleNullW),
	m_nBufLen(2),
	m_nCount(0)
{
	Assign(src);
}

//#################################################################################################
CDoubleNullStrW::CDoubleNullStrW(CDoubleNullStrW &&src) noexcept
	: m_szBuffer(src.m_szBuffer),
	m_nBufLen(src.m_nBufLen),
	m_nCount(src.m_nCount)
{
	src.m_szBuffer = (PWSTR)g_szDoubleNullW;
	src.m_nBufLen = 2;
	src.m_nCount = 0;
}

//#################################################################################################
CDoubleNullStrW::CDoubleNullStrW(PCWSTR sz, const bool bDoubleNullTerm)
	: m_szBuffer((PWSTR)g_szDoubleNullW),
	m_nBufLen(2),
	m_nCount(0)
{
	Assign(sz, bDoubleNullTerm);
}

//#################################################################################################
CDoubleNullStrW::~CDoubleNullStrW(void)
{
	Empty();
}

//#################################################################################################
size_t CDoubleNullStrW::GetLength(void) const
{	// Length in characters (including null-terminators)
	size_t nTotalStrLen = 0;

	if(m_nCount)
	{
		PCWSTR szScan = m_szBuffer;
		for(size_t n = 0; n < m_nCount; ++n)
		{
			size_t nStrLen = StringGetLength(szScan);
			if(nStrLen == 0)
				break;
			szScan += nStrLen + 1;
			nTotalStrLen += nStrLen + 1;
		}
	}
	else
		nTotalStrLen = 1;

	return nTotalStrLen + 1;
}

//#################################################################################################
size_t CDoubleNullStrW::GetSize(void) const
{	// Size in bytes (including null-terminators)
	return GetLength() * sizeof(wchar_t);
}

//#################################################################################################
CDoubleNullStrW::operator PCWSTR(void) const noexcept
{
	return m_szBuffer;
}

//#################################################################################################
bool CDoubleNullStrW::IsEmpty(void) const noexcept
{
	return (m_nCount == 0);
}

//#################################################################################################
void CDoubleNullStrW::Empty(void)
{
	if(m_nCount)
		std::free(m_szBuffer);
	m_szBuffer = (PWSTR)g_szDoubleNullW;
	m_nBufLen = 2;
	m_nCount = 0;
}

//#################################################################################################
CDoubleNullStrW &CDoubleNullStrW::operator=(const CDoubleNullStrW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CDoubleNullStrW &CDoubleNullStrW::operator=(CDoubleNullStrW &&src) noexcept
{
	if(this != &src)
	{
		if(m_nCount)
			std::free(m_szBuffer);

		m_szBuffer = src.m_szBuffer;
		m_nBufLen = src.m_nBufLen;
		m_nCount = src.m_nCount;

		src.m_szBuffer = (PWSTR)g_szDoubleNullW;
		src.m_nBufLen = 2;
		src.m_nCount = 0;
	}

	return *this;
}

//#################################################################################################
CDoubleNullStrW &CDoubleNullStrW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
void CDoubleNullStrW::Assign(const CDoubleNullStrW &str)
{
	if(this != &str)
	{
		Empty();

		if(str.m_nCount)
		{
			m_szBuffer = (PWSTR)std::malloc(str.m_nBufLen * sizeof(wchar_t));
			if(m_szBuffer)
			{
				std::wmemcpy(m_szBuffer, str.m_szBuffer, str.m_nBufLen);
				m_nBufLen = str.m_nBufLen;
				m_nCount = str.m_nCount;
			}
		}
	}
}

//#################################################################################################
void CDoubleNullStrW::Assign(PCWSTR sz, const bool bDoubleNullTerm)
{
	Empty();

	PCWSTR szScan = sz;
	size_t nTotalStrLen = 0;
	size_t nCount = 0;

	do{
		size_t nStrLen = StringGetLength(szScan);
		if(nStrLen == 0)
			break;
		szScan += nStrLen + 1;
		nTotalStrLen += nStrLen + 1;
		++nCount;
	}while(bDoubleNullTerm);

	if(nTotalStrLen > 1)
	{
		m_szBuffer = (PWSTR)std::malloc((nTotalStrLen + 1) * sizeof(wchar_t));
		if(m_szBuffer)
		{
			std::wmemcpy(m_szBuffer, sz, nTotalStrLen);
			m_szBuffer[nTotalStrLen] = g_chNullW;
			m_nBufLen = nTotalStrLen + 1;
			m_nCount = nCount;
		}
	}
}

//#################################################################################################
CDoubleNullStrW &CDoubleNullStrW::operator+=(const CDoubleNullStrW &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CDoubleNullStrW &CDoubleNullStrW::operator+=(PCWSTR sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
void CDoubleNullStrW::Append(const CDoubleNullStrW &str)
{
	if(this != &str && str.m_nCount)
	{
		if(m_nCount == 0)
		{
			m_szBuffer = (PWSTR)std::malloc(str.m_nBufLen * sizeof(wchar_t));
			if(m_szBuffer)
			{
				std::wmemcpy(m_szBuffer, str.m_szBuffer, str.m_nBufLen);
				m_nBufLen = str.m_nBufLen;
			}
		}
		else
		{
			PWSTR szTemp = m_szBuffer;
			m_szBuffer = (PWSTR)std::malloc((m_nBufLen + str.m_nBufLen - 1) * sizeof(wchar_t));
			if(m_szBuffer)
			{
				std::wmemcpy(m_szBuffer, szTemp, m_nBufLen - 1);
				std::wmemcpy(m_szBuffer + m_nBufLen - 1, str.m_szBuffer, str.m_nBufLen);
				m_nBufLen += str.m_nBufLen - 1;
				std::free(szTemp);
			}
		}

		++m_nCount;
	}
}

//#################################################################################################
void CDoubleNullStrW::Append(PCWSTR sz, const bool bDoubleNullTerm)
{
	PCWSTR szScan = sz;
	size_t nTotalStrLen = 0;
	size_t nCount = 0;

	do{
		size_t nStrLen = StringGetLength(szScan);
		if(nStrLen == 0)
			break;
		szScan += nStrLen + 1;
		nTotalStrLen += nStrLen + 1;
		++nCount;
	}while(bDoubleNullTerm);

	if(nTotalStrLen > 1)
	{
		if(m_nCount == 0)
		{
			m_szBuffer = (PWSTR)std::malloc((nTotalStrLen + 1) * sizeof(wchar_t));
			if(m_szBuffer)
			{
				std::wmemcpy(m_szBuffer, sz, nTotalStrLen);
				m_szBuffer[nTotalStrLen] = g_chNullW;
				m_nBufLen = nTotalStrLen + 1;
			}
		}
		else
		{
			PWSTR szTemp = m_szBuffer;
			m_szBuffer = (PWSTR)std::malloc((m_nBufLen + nTotalStrLen) * sizeof(wchar_t));
			if(m_szBuffer)
			{
				std::wmemcpy(m_szBuffer, szTemp, m_nBufLen - 1);
				std::wmemcpy(m_szBuffer + m_nBufLen - 1, sz, nTotalStrLen);
				m_szBuffer[m_nBufLen + nTotalStrLen - 1] = g_chNullW;
				m_nBufLen += nTotalStrLen;
			}
			std::free(szTemp);
		}

		m_nCount += nCount;
	}
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CDoubleNullStrW &str)
{
	PCWSTR *arr = str.GetArray();
	for(size_t n = 0; n < str.GetCount(); ++n)
	{
		if(n)
			stream << L'|';	// Use a pipe character to separate each element in the string array
		stream << arr[n];
	}
	CDoubleNullStrW::FreeArray(arr);

	return stream;
}

//#################################################################################################
size_t CDoubleNullStrW::GetCount(void) const noexcept
{
	return m_nCount;
}

//#################################################################################################
PCWSTR CDoubleNullStrW::GetAt(const size_t nIndex) const
{
	PCWSTR sz = nullptr;

	if(nIndex < m_nCount)
	{
		PCWSTR szScan = m_szBuffer;

		size_t n = nIndex;
		while(n--)
		{
			size_t nStrLen = StringGetLength(szScan);
			if(nStrLen == 0)
				break;
			szScan += nStrLen + 1;
		}

		sz = szScan;
	}

	return sz;
}

//#################################################################################################
PCWSTR *CDoubleNullStrW::GetArray(void) const
{
	PCWSTR *pszArr = nullptr;

	if(m_nCount)
	{
		pszArr = (PCWSTR*)std::calloc(m_nCount, sizeof(PCWSTR));		// Caller must call FreeArray() to free this memory
		if(pszArr)
		{
			PCWSTR szScan = m_szBuffer;
			for(size_t n = 0; n < m_nCount; ++n)
			{
				pszArr[n] = szScan;
				size_t nStrLen = StringGetLength(szScan);
				if(nStrLen == 0)
					break;
				szScan += nStrLen + 1;
			}
		}
	}

	return pszArr;
}

//#################################################################################################
void CDoubleNullStrW::FreeArray(PVOID pszArr)
{
	std::free(pszArr);
}

//#################################################################################################
size_t CDoubleNullStrW::GetBufferLength(void) const noexcept
{
	return m_nBufLen;
}

//#################################################################################################
size_t CDoubleNullStrW::GetBufferSize(void) const noexcept
{
	return m_nBufLen * sizeof(wchar_t);
}

NS_END

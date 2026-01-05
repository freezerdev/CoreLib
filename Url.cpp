#include "Base.h"
#include "Url.h"
#include "UrlUtils.h"

NS_BEGIN

static PCSTR g_szValidDomainName8 = "*-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static PCWSTR g_szValidDomainNameW = L"*-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static constexpr size_t g_nValidDomainNameLen = 65;
static PCSTR g_szInvalidParam8 = "/?&#";
static PCWSTR g_szInvalidParamW = L"/?&#";
static constexpr size_t g_nInvalidParamLen = 4;
static PCSTR g_szSchemeSeparator8 = "://";
static PCWSTR g_szSchemeSeparatorW = L"://";
static constexpr size_t g_nSchemeSeparatorLen = 3;
static PCSTR g_szDomainSeparators8 = "/?#";
static PCWSTR g_szDomainSeparatorsW = L"/?#";
static PCSTR g_szPathSeparators8 = "?#";
static PCWSTR g_szPathSeparatorsW = L"?#";

static constexpr char g_chPeriod8 = '.';
static constexpr wchar_t g_chPeriodW = L'.';
static constexpr char g_chEqual8 = '=';
static constexpr char g_chColon8 = ':';
static constexpr char g_chQuestionMark8 = '?';
static constexpr char g_chAmpersand8 = '&';
static constexpr char g_chAt8 = '@';
static constexpr char g_chHash8 = '#';
static constexpr char g_chForwardSlash8 = '/';

static constexpr wchar_t g_chEqualW = L'=';
static constexpr wchar_t g_chColonW = L':';
static constexpr wchar_t g_chQuestionMarkW = L'?';
static constexpr wchar_t g_chAmpersandW = L'&';
static constexpr wchar_t g_chAtW = L'@';
static constexpr wchar_t g_chHashW = L'#';
static constexpr wchar_t g_chForwardSlashW = L'/';

static constexpr auto MAX_PORT = 0xFFFF;

const uint32_t CUrl8::no_port = (uint32_t)-1;
const uint32_t CUrlW::no_port = (uint32_t)-1;



/////////////////////////////////////////////////
// CDomainName8
/////////////////////////////////////////////////

//#################################################################################################
CDomainName8::CDomainName8(void)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CDomainName8::CDomainName8(const CDomainName8 &src)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CDomainName8::CDomainName8(CDomainName8 &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PSTR)g_szNull8;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CDomainName8::CDomainName8(const CDomainNameW &domain)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(domain);
}

//#################################################################################################
CDomainName8::CDomainName8(const CStr8 &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CDomainName8::CDomainName8(const CStrW &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CDomainName8::CDomainName8(PCSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CDomainName8::CDomainName8(PCWSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CDomainName8::CDomainName8(const char ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CDomainName8::CDomainName8(const wchar_t ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CDomainName8::~CDomainName8(void)
{
	Dealloc();
}

//#################################################################################################
CDomainName8::operator PCSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCSTR CDomainName8::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CDomainName8::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CDomainName8::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
void CDomainName8::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CDomainName8::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
CDomainName8 CDomainName8::AsUtf8(void) const
{
	return *this;
}

//#################################################################################################
CDomainNameW CDomainName8::AsWide(void) const
{
	return CDomainNameW(*this);
}

#ifdef _WIN32
//#################################################################################################
CDomainNameW CDomainName8::AsNative(void) const
{
	return CDomainNameW(*this);
}
#else
//#################################################################################################
CDomainName8 CDomainName8::AsNative(void) const
{
	return *this;
}
#endif

//#################################################################################################
CDomainName8 &CDomainName8::operator=(const CDomainName8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(CDomainName8 &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufLen)
			std::free(m_sz);

		m_sz = src.m_sz;
		m_nBufLen = src.m_nBufLen;
		m_nStrLen = src.m_nStrLen;

		src.m_sz = (PSTR)g_szNull8;
		src.m_nBufLen = 0;
		src.m_nStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(const CDomainNameW &domain)
{
	Assign(domain);
	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CDomainName8 &CDomainName8::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(const CDomainName8 &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &domain)
	{
		if(domain.m_nStrLen)
		{
			if(Alloc(domain.m_nStrLen + 1))
			{
				std::memcpy(m_sz, domain.m_sz, domain.m_nStrLen);
				m_nStrLen = domain.m_nStrLen;
				m_sz[m_nStrLen] = g_chNull8;
			}
			else
			{
				Dealloc();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
			Dealloc();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(const CDomainNameW &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(domain.m_nStrLen)
	{
		if(Alloc(WideToUtf8(domain.m_sz, domain.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(domain.m_sz, domain.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		Dealloc();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidDomainName(str))
	{
		if(Alloc(str.GetLength() + 1))
		{
			std::memcpy(m_sz, (PCSTR)str, str.GetLength());
			m_nStrLen = str.GetLength();
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CDomainNameW::IsValidDomainName(str))
	{
		if(Alloc(WideToUtf8(str, str.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(str, str.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidDomainName(sz))
	{
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(nStrLen + 1))
		{
			std::memcpy(m_sz, sz, nStrLen);
			m_nStrLen = nStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CDomainNameW::IsValidDomainName(sz))
	{
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(WideToUtf8(sz, nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(sz, nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidDomainName(ch))
	{
		if(Alloc(2))
		{
			m_sz[0] = ch;
			m_nStrLen = 1;
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainName8::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CDomainNameW::IsValidDomainName(ch))
	{
		if(Alloc(WideToUtf8(&ch, 1, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(&ch, 1, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CDomainName8 &domain)
{
	stream << domain.m_sz;
	return stream;
}

//#################################################################################################
bool CDomainName8::operator==(const CDomainName8 &domain) const
{
	return (StringCompare8(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) == 0);
}

//#################################################################################################
bool CDomainName8::operator==(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) == 0);
}

//#################################################################################################
bool CDomainName8::operator==(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) == 0);
}

//#################################################################################################
bool CDomainName8::operator==(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) == 0);
}

//#################################################################################################
bool CDomainName8::operator!=(const CDomainName8 &domain) const
{
	return (StringCompare8(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) != 0);
}

//#################################################################################################
bool CDomainName8::operator!=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) != 0);
}

//#################################################################################################
bool CDomainName8::operator!=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) != 0);
}

//#################################################################################################
bool CDomainName8::operator!=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) != 0);
}

//#################################################################################################
bool CDomainName8::operator<(const CDomainName8 &domain) const
{
	return (StringCompare8(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) < 0);
}

//#################################################################################################
bool CDomainName8::operator<(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) < 0);
}

//#################################################################################################
bool CDomainName8::operator<(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) < 0);
}

//#################################################################################################
bool CDomainName8::operator<(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) < 0);
}

//#################################################################################################
bool CDomainName8::operator<=(const CDomainName8 &domain) const
{
	return (StringCompare8(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) <= 0);
}

//#################################################################################################
bool CDomainName8::operator<=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) <= 0);
}

//#################################################################################################
bool CDomainName8::operator<=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) <= 0);
}

//#################################################################################################
bool CDomainName8::operator<=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) <= 0);
}

//#################################################################################################
bool CDomainName8::operator>(const CDomainName8 &domain) const
{
	return (StringCompare8(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) > 0);
}

//#################################################################################################
bool CDomainName8::operator>(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) > 0);
}

//#################################################################################################
bool CDomainName8::operator>(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) > 0);
}

//#################################################################################################
bool CDomainName8::operator>(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) > 0);
}

//#################################################################################################
bool CDomainName8::operator>=(const CDomainName8 &domain) const
{
	return (StringCompare8(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) >= 0);
}

//#################################################################################################
bool CDomainName8::operator>=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) >= 0);
}

//#################################################################################################
bool CDomainName8::operator>=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) >= 0);
}

//#################################################################################################
bool CDomainName8::operator>=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) >= 0);
}

//#################################################################################################
int CDomainName8::Compare(const CDomainName8 &domain, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CDomainName8::Compare(const CStr8 &str, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CDomainName8::Compare(PCSTR sz, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CDomainName8::Compare(const char ch, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
bool CDomainName8::IsValidDomainName(const CStr8 &strDomainName)
{	// Valid domain names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.  Asterisks are allowed as a wildcard character.
	// Also, first and last character cannot be a hyphen or period.

	// Length between 1 and 63
	size_t nStrLen = strDomainName.GetLength();
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(strDomainName.GetFirst() == '-' || strDomainName.GetFirst() == g_chPeriod8 || strDomainName.GetLast() == '-' || strDomainName.GetLast() == g_chPeriod8)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(strDomainName, nStrLen, "..", 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(strDomainName, nStrLen, g_szValidDomainName8, g_nValidDomainNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CDomainName8::IsValidDomainName(PCSTR szDomainName)
{	// Valid domain names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.  Asterisks are allowed as a wildcard character.
	// Also, first and last character cannot be a hyphen or period.

	// Cannot be nullptr
	if(szDomainName == nullptr)
		return false;

	// Length between 1 and 63
	size_t nStrLen = StringGetLength(szDomainName);
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(szDomainName[0] == '-' || szDomainName[0] == g_chPeriod8 || szDomainName[nStrLen - 1] == '-' || szDomainName[nStrLen - 1] == g_chPeriod8)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(szDomainName, nStrLen, "..", 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(szDomainName, nStrLen, g_szValidDomainName8, g_nValidDomainNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CDomainName8::IsValidDomainName(const char chDomainName)
{	// Valid domain names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	return IsAlphaDigitChar8(chDomainName);
}

//#################################################################################################
bool CDomainName8::Alloc(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nBufLen)
	{	// The buffer is not large enough, realloc it
		PSTR sz;
		if(m_nBufLen)
			sz = (PSTR)std::realloc(m_sz, nBufLen);
		else
			sz = (PSTR)std::malloc(nBufLen);

		if(sz)
		{
			m_sz = sz;
			m_nBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CDomainName8::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PSTR)g_szNull8;
	m_nStrLen = m_nBufLen = 0;
}

//#################################################################################################
PCSTR CDomainName8::FindStrPtr(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCSTR szScan1 = sz;
		PCSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PCSTR)std::memchr(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(std::memcmp(szScan2, szFind, nFindLen) == 0)
				return szScan2;

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return nullptr;
}

//#################################################################################################
PCSTR CDomainName8::FindFirstNotOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if(!std::memchr(szFind, sz[nIndex], nFindLen))
			return sz + nIndex;
	}

	return nullptr;
}




/////////////////////////////////////////////////
// CDomainNameW
/////////////////////////////////////////////////

//#################################################################################################
CDomainNameW::CDomainNameW(void)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CDomainNameW::CDomainNameW(const CDomainNameW &src)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CDomainNameW::CDomainNameW(CDomainNameW &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PWSTR)g_szNullW;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CDomainNameW::CDomainNameW(const CDomainName8 &domain)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(domain);
}

//#################################################################################################
CDomainNameW::CDomainNameW(const CStrW &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CDomainNameW::CDomainNameW(const CStr8 &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CDomainNameW::CDomainNameW(PCWSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CDomainNameW::CDomainNameW(PCSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CDomainNameW::CDomainNameW(const wchar_t ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CDomainNameW::CDomainNameW(const char ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CDomainNameW::~CDomainNameW(void)
{
	Dealloc();
}

//#################################################################################################
CDomainNameW::operator PCWSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCWSTR CDomainNameW::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CDomainNameW::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CDomainNameW::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen) * sizeof(wchar_t);
}

//#################################################################################################
void CDomainNameW::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CDomainNameW::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
CDomainName8 CDomainNameW::AsUtf8(void) const
{
	return CDomainName8(*this);
}

//#################################################################################################
CDomainNameW CDomainNameW::AsWide(void) const
{
	return *this;
}

#ifdef _WIN32
//#################################################################################################
CDomainNameW CDomainNameW::AsNative(void) const
{
	return *this;
}
#else
//#################################################################################################
CDomainName8 CDomainNameW::AsNative(void) const
{
	return CDomainName8(*this);
}
#endif

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(const CDomainNameW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(CDomainNameW &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufLen)
			std::free(m_sz);

		m_sz = src.m_sz;
		m_nBufLen = src.m_nBufLen;
		m_nStrLen = src.m_nStrLen;

		src.m_sz = (PWSTR)g_szNullW;
		src.m_nBufLen = 0;
		src.m_nStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(const CDomainName8 &domain)
{
	Assign(domain);
	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CDomainNameW &CDomainNameW::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(const CDomainNameW &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &domain)
	{
		if(domain.m_nStrLen)
		{
			if(Alloc(domain.m_nStrLen + 1))
			{
				std::wmemcpy(m_sz, domain.m_sz, domain.m_nStrLen);
				m_nStrLen = domain.m_nStrLen;
				m_sz[m_nStrLen] = g_chNullW;
			}
			else
			{
				Dealloc();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
			Dealloc();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(const CDomainName8 &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(domain.m_nStrLen)
	{
		if(Alloc(Utf8ToWide(domain.m_sz, domain.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(domain.m_sz, domain.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		Dealloc();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidDomainName(str))
	{
		if(Alloc(str.GetLength() + 1))
		{
			std::wmemcpy(m_sz, (PCWSTR)str, str.GetLength());
			m_nStrLen = str.GetLength();
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CDomainName8::IsValidDomainName(str))
	{
		if(Alloc(Utf8ToWide(str, str.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(str, str.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidDomainName(sz))
	{
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(nStrLen + 1))
		{
			std::wmemcpy(m_sz, sz, nStrLen);
			m_nStrLen = nStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CDomainName8::IsValidDomainName(sz))
	{
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(Utf8ToWide(sz, nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(sz, nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidDomainName(ch))
	{
		if(Alloc(2))
		{
			m_sz[0] = ch;
			m_nStrLen = 1;
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CDomainNameW::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CDomainName8::IsValidDomainName(ch))
	{
		if(Alloc(Utf8ToWide(&ch, 1, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(&ch, 1, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CDomainNameW &domain)
{
	stream << domain.m_sz;
	return stream;
}

//#################################################################################################
bool CDomainNameW::operator==(const CDomainNameW &domain) const
{
	return (StringCompareW(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) == 0);
}

//#################################################################################################
bool CDomainNameW::operator==(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) == 0);
}

//#################################################################################################
bool CDomainNameW::operator==(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) == 0);
}

//#################################################################################################
bool CDomainNameW::operator==(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) == 0);
}

//#################################################################################################
bool CDomainNameW::operator!=(const CDomainNameW &domain) const
{
	return (StringCompareW(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) != 0);
}

//#################################################################################################
bool CDomainNameW::operator!=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) != 0);
}

//#################################################################################################
bool CDomainNameW::operator!=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) != 0);
}

//#################################################################################################
bool CDomainNameW::operator!=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) != 0);
}

//#################################################################################################
bool CDomainNameW::operator<(const CDomainNameW &domain) const
{
	return (StringCompareW(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) < 0);
}

//#################################################################################################
bool CDomainNameW::operator<(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) < 0);
}

//#################################################################################################
bool CDomainNameW::operator<(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) < 0);
}

//#################################################################################################
bool CDomainNameW::operator<(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) < 0);
}

//#################################################################################################
bool CDomainNameW::operator<=(const CDomainNameW &domain) const
{
	return (StringCompareW(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) <= 0);
}

//#################################################################################################
bool CDomainNameW::operator<=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) <= 0);
}

//#################################################################################################
bool CDomainNameW::operator<=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) <= 0);
}

//#################################################################################################
bool CDomainNameW::operator<=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) <= 0);
}

//#################################################################################################
bool CDomainNameW::operator>(const CDomainNameW &domain) const
{
	return (StringCompareW(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) > 0);
}

//#################################################################################################
bool CDomainNameW::operator>(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) > 0);
}

//#################################################################################################
bool CDomainNameW::operator>(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) > 0);
}

//#################################################################################################
bool CDomainNameW::operator>(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) > 0);
}

//#################################################################################################
bool CDomainNameW::operator>=(const CDomainNameW &domain) const
{
	return (StringCompareW(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, true) >= 0);
}

//#################################################################################################
bool CDomainNameW::operator>=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) >= 0);
}

//#################################################################################################
bool CDomainNameW::operator>=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) >= 0);
}

//#################################################################################################
bool CDomainNameW::operator>=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) >= 0);
}

//#################################################################################################
int CDomainNameW::Compare(const CDomainNameW &domain, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, domain.m_sz, domain.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CDomainNameW::Compare(const CStrW &str, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CDomainNameW::Compare(PCWSTR sz, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CDomainNameW::Compare(const wchar_t ch, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
bool CDomainNameW::IsValidDomainName(const CStrW &strDomainName)
{	// Valid domain names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.  Asterisks are allowed as a wildcard character.
	// Also, first and last character cannot be a hyphen or period.

	// Length between 1 and 63
	size_t nStrLen = strDomainName.GetLength();
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(strDomainName.GetFirst() == L'-' || strDomainName.GetFirst() == g_chPeriodW || strDomainName.GetLast() == L'-' || strDomainName.GetLast() == g_chPeriodW)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(strDomainName, nStrLen, L"..", 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(strDomainName, nStrLen, g_szValidDomainNameW, g_nValidDomainNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CDomainNameW::IsValidDomainName(PCWSTR szDomainName)
{	// Valid domain names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.  Asterisks are allowed as a wildcard character.
	// Also, first and last character cannot be a hyphen or period.

	// Cannot be nullptr
	if(szDomainName == nullptr)
		return false;

	// Length between 1 and 63
	size_t nStrLen = StringGetLength(szDomainName);
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(szDomainName[0] == L'-' || szDomainName[0] == g_chPeriodW || szDomainName[nStrLen - 1] == L'-' || szDomainName[nStrLen - 1] == g_chPeriodW)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(szDomainName, nStrLen, L"..", 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(szDomainName, nStrLen, g_szValidDomainNameW, g_nValidDomainNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CDomainNameW::IsValidDomainName(const wchar_t chDomainName)
{	// Valid domain names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	return IsAlphaDigitCharW(chDomainName);
}

//#################################################################################################
bool CDomainNameW::Alloc(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nBufLen)
	{	// The buffer is not large enough, realloc it
		PWSTR sz;
		if(m_nBufLen)
			sz = (PWSTR)std::realloc(m_sz, nBufLen * sizeof(wchar_t));
		else
			sz = (PWSTR)std::malloc(nBufLen * sizeof(wchar_t));

		if(sz)
		{
			m_sz = sz;
			m_nBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CDomainNameW::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PWSTR)g_szNullW;
	m_nStrLen = m_nBufLen = 0;
}

//#################################################################################################
PCWSTR CDomainNameW::FindStrPtr(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCWSTR szScan1 = sz;
		PCWSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PCWSTR)std::wmemchr(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(std::wmemcmp(szScan2, szFind, nFindLen) == 0)
				return szScan2;

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return nullptr;
}

//#################################################################################################
PCWSTR CDomainNameW::FindFirstNotOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if(!std::wmemchr(szFind, sz[nIndex], nFindLen))
			return sz + nIndex;
	}

	return nullptr;
}




/////////////////////////////////////////////////
// CUrlPathSegment8
/////////////////////////////////////////////////

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(void)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(const CUrlPathSegment8 &src)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(CUrlPathSegment8 &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PSTR)g_szNull8;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(const CUrlPathSegmentW &path)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(path);
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(const CStr8 &str, const bool bAlreadyEscaped)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(const CStrW &str, const bool bAlreadyEscaped)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(PCSTR sz, const bool bAlreadyEscaped)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(PCWSTR sz, const bool bAlreadyEscaped)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(const char ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CUrlPathSegment8::CUrlPathSegment8(const wchar_t ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CUrlPathSegment8::~CUrlPathSegment8(void)
{
	Dealloc();
}

//#################################################################################################
CUrlPathSegment8::operator PCSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCSTR CUrlPathSegment8::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CUrlPathSegment8::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CUrlPathSegment8::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
void CUrlPathSegment8::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CUrlPathSegment8::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
CUrlPathSegment8 CUrlPathSegment8::AsUtf8(void) const
{
	return *this;
}

//#################################################################################################
CUrlPathSegmentW CUrlPathSegment8::AsWide(void) const
{
	return CUrlPathSegmentW(*this);
}

#ifdef _WIN32
//#################################################################################################
CUrlPathSegmentW CUrlPathSegment8::AsNative(void) const
{
	return CUrlPathSegmentW(*this);
}
#else
//#################################################################################################
CUrlPathSegment8 CUrlPathSegment8::AsNative(void) const
{
	return *this;
}
#endif

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(const CUrlPathSegment8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(CUrlPathSegment8 &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufLen)
			std::free(m_sz);

		m_sz = src.m_sz;
		m_nBufLen = src.m_nBufLen;
		m_nStrLen = src.m_nStrLen;

		src.m_sz = (PSTR)g_szNull8;
		src.m_nBufLen = 0;
		src.m_nStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(const CUrlPathSegmentW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CUrlPathSegment8 &CUrlPathSegment8::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(const CUrlPathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &path)
	{
		if(path.m_nStrLen)
		{
			if(Alloc(path.m_nStrLen + 1))
			{
				std::memcpy(m_sz, path.m_sz, path.m_nStrLen);
				m_nStrLen = path.m_nStrLen;
				m_sz[m_nStrLen] = g_chNull8;
			}
			else
			{
				Dealloc();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
			Dealloc();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(const CUrlPathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(path.m_nStrLen)
	{
		if(Alloc(WideToUtf8(path.m_sz, path.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(path.m_sz, path.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		Dealloc();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(const CStr8 &str, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscaped(str);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(strEscaped.GetLength() + 1))
		{
			std::memcpy(m_sz, (PCSTR)strEscaped, strEscaped.GetLength());
			m_nStrLen = strEscaped.GetLength();
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(const CStrW &str, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscaped(str);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(WideToUtf8(strEscaped, strEscaped.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(strEscaped, strEscaped.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(PCSTR sz, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscaped(sz);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(strEscaped.GetLength() + 1))
		{
			std::memcpy(m_sz, (PCSTR)strEscaped, strEscaped.GetLength());
			m_nStrLen = strEscaped.GetLength();
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(PCWSTR sz, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscaped(sz);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(WideToUtf8(strEscaped, strEscaped.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(strEscaped, strEscaped.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscaped(ch);
	UrlEscape8(strEscaped);
	if(!strEscaped.IsEmpty())
	{
		if(Alloc(strEscaped.GetLength() + 1))
		{
			std::memcpy(m_sz, (PCSTR)strEscaped, strEscaped.GetLength());
			m_nStrLen = strEscaped.GetLength();
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegment8::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscaped(ch);
	UrlEscapeW(strEscaped);
	if(!strEscaped.IsEmpty())
	{
		if(Alloc(WideToUtf8(strEscaped, strEscaped.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(strEscaped, strEscaped.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CUrlPathSegment8 &path)
{
	stream << path.m_sz;
	return stream;
}

//#################################################################################################
bool CUrlPathSegment8::operator==(const CUrlPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) == 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator==(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength()) == 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator==(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) == 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator==(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) == 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator!=(const CUrlPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) != 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator!=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength()) != 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator!=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) != 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator!=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) != 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<(const CUrlPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) < 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength()) < 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) < 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) < 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<=(const CUrlPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) <= 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength()) <= 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) <= 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator<=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) <= 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>(const CUrlPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) > 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength()) > 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) > 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) > 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>=(const CUrlPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) >= 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength()) >= 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) >= 0);
}

//#################################################################################################
bool CUrlPathSegment8::operator>=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) >= 0);
}

//#################################################################################################
int CUrlPathSegment8::Compare(const CUrlPathSegment8 &path, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CUrlPathSegment8::Compare(const CStr8 &str, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CUrlPathSegment8::Compare(PCSTR sz, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CUrlPathSegment8::Compare(const char ch, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
bool CUrlPathSegment8::IsValidSegment(const CStr8 &strSegment)
{	// Valid Url path segments are of length 1 or greater,
	// and can contain any character except '/'

	// Length greater than 1
	size_t nStrLen = strSegment.GetLength();
	if(nStrLen == 0)
		return false;

	// Does it contain a forward slash?
	if(std::memchr(strSegment, '/', nStrLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlPathSegment8::IsValidSegment(PCSTR szSegment)
{	// Valid Url path segments are of length 1 or greater,
	// and can contain any character except '/'

	// Cannot be nullptr
	if(szSegment == nullptr)
		return false;

	// Length greater than 1
	size_t nStrLen = StringGetLength(szSegment);
	if(nStrLen == 0)
		return false;

	// Does it contain a forward slash?
	if(std::memchr(szSegment, '/', nStrLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlPathSegment8::IsValidSegment(const char chSegment) noexcept
{	// Valid Url path segments are of length 1 or greater,
	// and can contain any character except '/'

	return (chSegment != g_chNull8 && chSegment != '/');
}

//#################################################################################################
bool CUrlPathSegment8::Alloc(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nBufLen)
	{	// The buffer is not large enough, realloc it
		PSTR sz;
		if(m_nBufLen)
			sz = (PSTR)std::realloc(m_sz, nBufLen);
		else
			sz = (PSTR)std::malloc(nBufLen);

		if(sz)
		{
			m_sz = sz;
			m_nBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CUrlPathSegment8::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PSTR)g_szNull8;
	m_nStrLen = m_nBufLen = 0;
}




/////////////////////////////////////////////////
// CUrlPathSegmentW
/////////////////////////////////////////////////

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(void)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(const CUrlPathSegmentW &src)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(CUrlPathSegmentW &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PWSTR)g_szNullW;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(const CUrlPathSegment8 &path)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(path);
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(const CStrW &str, const bool bAlreadyEscaped)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(const CStr8 &str, const bool bAlreadyEscaped)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(PCWSTR sz, const bool bAlreadyEscaped)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(PCSTR sz, const bool bAlreadyEscaped)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz, bAlreadyEscaped);
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(const wchar_t ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CUrlPathSegmentW::CUrlPathSegmentW(const char ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CUrlPathSegmentW::~CUrlPathSegmentW(void)
{
	Dealloc();
}

//#################################################################################################
CUrlPathSegmentW::operator PCWSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCWSTR CUrlPathSegmentW::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CUrlPathSegmentW::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CUrlPathSegmentW::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen) * sizeof(wchar_t);
}

//#################################################################################################
void CUrlPathSegmentW::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CUrlPathSegmentW::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
CUrlPathSegment8 CUrlPathSegmentW::AsUtf8(void) const
{
	return CUrlPathSegment8(*this);
}

//#################################################################################################
CUrlPathSegmentW CUrlPathSegmentW::AsWide(void) const
{
	return *this;
}

#ifdef _WIN32
//#################################################################################################
CUrlPathSegmentW CUrlPathSegmentW::AsNative(void) const
{
	return *this;
}
#else
//#################################################################################################
CUrlPathSegment8 CUrlPathSegmentW::AsNative(void) const
{
	return CUrlPathSegment8(*this);
}
#endif

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(const CUrlPathSegmentW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(CUrlPathSegmentW &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufLen)
			std::free(m_sz);

		m_sz = src.m_sz;
		m_nBufLen = src.m_nBufLen;
		m_nStrLen = src.m_nStrLen;

		src.m_sz = (PWSTR)g_szNullW;
		src.m_nBufLen = 0;
		src.m_nStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(const CUrlPathSegment8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CUrlPathSegmentW &CUrlPathSegmentW::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(const CUrlPathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &path)
	{
		if(path.m_nStrLen)
		{
			if(Alloc(path.m_nStrLen + 1))
			{
				std::wmemcpy(m_sz, path.m_sz, path.m_nStrLen);
				m_nStrLen = path.m_nStrLen;
				m_sz[m_nStrLen] = g_chNullW;
			}
			else
			{
				Dealloc();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
			Dealloc();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(const CUrlPathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(path.m_nStrLen)
	{
		if(Alloc(Utf8ToWide(path.m_sz, path.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(path.m_sz, path.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		Dealloc();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(const CStrW &str, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscaped(str);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(strEscaped.GetLength() + 1))
		{
			std::wmemcpy(m_sz, (PCWSTR)strEscaped, strEscaped.GetLength());
			m_nStrLen = strEscaped.GetLength();
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(const CStr8 &str, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscaped(str);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(Utf8ToWide(strEscaped, strEscaped.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(strEscaped, strEscaped.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(PCWSTR sz, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscaped(sz);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(strEscaped.GetLength() + 1))
		{
			std::wmemcpy(m_sz, (PCWSTR)strEscaped, strEscaped.GetLength());
			m_nStrLen = strEscaped.GetLength();
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(PCSTR sz, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscaped(sz);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscaped);

	if(!strEscaped.IsEmpty())
	{
		if(Alloc(Utf8ToWide(strEscaped, strEscaped.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(strEscaped, strEscaped.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscaped(ch);
	UrlEscapeW(strEscaped);
	if(!strEscaped.IsEmpty())
	{
		if(Alloc(strEscaped.GetLength() + 1))
		{
			std::wmemcpy(m_sz, (PCWSTR)strEscaped, strEscaped.GetLength());
			m_nStrLen = strEscaped.GetLength();
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlPathSegmentW::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscaped(ch);
	UrlEscape8(strEscaped);
	if(!strEscaped.IsEmpty())
	{
		if(Alloc(Utf8ToWide(strEscaped, strEscaped.GetLength(), nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(strEscaped, strEscaped.GetLength(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
		{
			Dealloc();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		Dealloc();
		nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CUrlPathSegmentW &path)
{
	stream << path.m_sz;
	return stream;
}

//#################################################################################################
bool CUrlPathSegmentW::operator==(const CUrlPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) == 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator==(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength()) == 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator==(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) == 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator==(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) == 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator!=(const CUrlPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) != 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator!=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength()) != 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator!=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) != 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator!=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) != 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<(const CUrlPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) < 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength()) < 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) < 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) < 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<=(const CUrlPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) <= 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength()) <= 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) <= 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator<=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) <= 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>(const CUrlPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) > 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength()) > 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) > 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) > 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>=(const CUrlPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen) >= 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength()) >= 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) >= 0);
}

//#################################################################################################
bool CUrlPathSegmentW::operator>=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) >= 0);
}

//#################################################################################################
int CUrlPathSegmentW::Compare(const CUrlPathSegmentW &path, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CUrlPathSegmentW::Compare(const CStrW &str, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CUrlPathSegmentW::Compare(PCWSTR sz, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CUrlPathSegmentW::Compare(const wchar_t ch, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
bool CUrlPathSegmentW::IsValidSegment(const CStrW &strSegment)
{	// Valid Url path segments are of length 1 or greater,
	// and can contain any character except '/'

	// Length greater than 1
	size_t nStrLen = strSegment.GetLength();
	if(nStrLen == 0)
		return false;

	// Does it contain a forward slash?
	if(std::wmemchr(strSegment, L'/', nStrLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlPathSegmentW::IsValidSegment(PCWSTR szSegment)
{	// Valid Url path segments are of length 1 or greater,
	// and can contain any character except '/'

	// Cannot be nullptr
	if(szSegment == nullptr)
		return false;

	// Length greater than 1
	size_t nStrLen = StringGetLength(szSegment);
	if(nStrLen == 0)
		return false;

	// Does it contain a forward slash?
	if(std::wmemchr(szSegment, L'/', nStrLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlPathSegmentW::IsValidSegment(const wchar_t chSegment) noexcept
{	// Valid Url path segments are of length 1 or greater,
	// and can contain any character except '/'

	return (chSegment != g_chNullW && chSegment != L'/');
}

//#################################################################################################
bool CUrlPathSegmentW::Alloc(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nBufLen)
	{	// The buffer is not large enough, realloc it
		PWSTR sz;
		if(m_nBufLen)
			sz = (PWSTR)std::realloc(m_sz, nBufLen * sizeof(wchar_t));
		else
			sz = (PWSTR)std::malloc(nBufLen * sizeof(wchar_t));

		if(sz)
		{
			m_sz = sz;
			m_nBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CUrlPathSegmentW::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PWSTR)g_szNullW;
	m_nStrLen = m_nBufLen = 0;
}




/////////////////////////////////////////////////
// CUrlParameter8
/////////////////////////////////////////////////

//#################################################################################################
CUrlParameter8::CUrlParameter8(void)
	: m_szName((PSTR)g_szNull8),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PSTR)g_szNull8),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
}

//#################################################################################################
CUrlParameter8::CUrlParameter8(const CUrlParameter8 &src)
	: m_szName((PSTR)g_szNull8),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PSTR)g_szNull8),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CUrlParameter8::CUrlParameter8(CUrlParameter8 &&src) noexcept
	: m_szName(src.m_szName),
	m_nNameBufLen(src.m_nNameBufLen),
	m_nNameStrLen(src.m_nNameStrLen),
	m_szValue(src.m_szValue),
	m_nValueBufLen(src.m_nValueBufLen),
	m_nValueStrLen(src.m_nValueStrLen)
{
	src.m_szName = (PSTR)g_szNull8;
	src.m_nNameBufLen = 0;
	src.m_nNameStrLen = 0;
	src.m_szValue = (PSTR)g_szNull8;
	src.m_nValueBufLen = 0;
	src.m_nValueStrLen = 0;
}

//#################################################################################################
CUrlParameter8::CUrlParameter8(const CUrlParameterW &param)
	: m_szName((PSTR)g_szNull8),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PSTR)g_szNull8),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(param);
}

//#################################################################################################
CUrlParameter8::CUrlParameter8(const CStr8 &strName, const CStr8 &strValue, const bool bAlreadyEscaped)
	: m_szName((PSTR)g_szNull8),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PSTR)g_szNull8),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(strName, strValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameter8::CUrlParameter8(const CStrW &strName, const CStrW &strValue, const bool bAlreadyEscaped)
	: m_szName((PSTR)g_szNull8),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PSTR)g_szNull8),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(strName, strValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameter8::CUrlParameter8(PCSTR szName, PCSTR szValue, const bool bAlreadyEscaped)
	: m_szName((PSTR)g_szNull8),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PSTR)g_szNull8),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(szName, szValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameter8::CUrlParameter8(PCWSTR szName, PCWSTR szValue, const bool bAlreadyEscaped)
	: m_szName((PSTR)g_szNull8),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PSTR)g_szNull8),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(szName, szValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameter8::~CUrlParameter8(void)
{
	DeallocName();
	DeallocValue();
}

//#################################################################################################
CUrlParameter8::operator CStr8(void) const
{
	return Get();
}

//#################################################################################################
CStr8 CUrlParameter8::Get(const bool bAlwaysIncludeEqual) const
{
	CStr8 str;

	if(m_nNameStrLen)
	{
		str.Assign(m_szName);
		if(m_nValueStrLen || bAlwaysIncludeEqual)
			str.Append(g_chEqual8);
		str.Append(m_szValue);
	}

	return str;
}

//#################################################################################################
size_t CUrlParameter8::GetLength(const bool bAlwaysIncludeEqual, const bool bIncludeNullTerm) const
{	// Length in characters
	size_t nSize = (bIncludeNullTerm) ? m_nNameStrLen + 1 : m_nNameStrLen;

	if(m_nNameStrLen && (m_nValueStrLen || bAlwaysIncludeEqual))
		nSize += m_nValueStrLen + 1;

	return nSize;
}

//#################################################################################################
size_t CUrlParameter8::GetSize(const bool bAlwaysIncludeEqual, const bool bIncludeNullTerm) const
{	// Size in bytes
	return GetLength(bAlwaysIncludeEqual, bIncludeNullTerm);
}

//#################################################################################################
PCSTR CUrlParameter8::GetName(void) const noexcept
{
	return m_szName;
}

//#################################################################################################
size_t CUrlParameter8::GetNameLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nNameStrLen + 1 : m_nNameStrLen;
}

//#################################################################################################
size_t CUrlParameter8::GetNameSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return (bIncludeNullTerm) ? m_nNameStrLen + 1 : m_nNameStrLen;
}

//#################################################################################################
PCSTR CUrlParameter8::GetValue(void) const noexcept
{
	return m_szValue;
}

//#################################################################################################
size_t CUrlParameter8::GetValueLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nValueStrLen + 1 : m_nValueStrLen;
}

//#################################################################################################
size_t CUrlParameter8::GetValueSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return (bIncludeNullTerm) ? m_nValueStrLen + 1 : m_nValueStrLen;
}

//#################################################################################################
void CUrlParameter8::Empty(void)
{
	DeallocName();
	DeallocValue();
}

//#################################################################################################
bool CUrlParameter8::IsEmpty(void) const noexcept
{
	return (m_nNameStrLen == 0);
}

//#################################################################################################
bool CUrlParameter8::IsValueEmpty(void) const noexcept
{
	return (m_nValueStrLen == 0);
}

//#################################################################################################
CUrlParameter8 CUrlParameter8::AsUtf8(void) const
{
	return *this;
}

//#################################################################################################
CUrlParameterW CUrlParameter8::AsWide(void) const
{
	return CUrlParameterW(*this);
}

#ifdef _WIN32
//#################################################################################################
CUrlParameterW CUrlParameter8::AsNative(void) const
{
	return CUrlParameterW(*this);
}
#else
//#################################################################################################
CUrlParameter8 CUrlParameter8::AsNative(void) const
{
	return *this;
}
#endif

//#################################################################################################
CUrlParameter8 &CUrlParameter8::operator=(const CUrlParameter8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CUrlParameter8 &CUrlParameter8::operator=(CUrlParameter8 &&src) noexcept
{
	if(this != &src)
	{
		if(m_nNameBufLen)
			std::free(m_szName);
		if(m_nValueBufLen)
			std::free(m_szValue);

		m_szName = src.m_szName;
		m_nNameBufLen = src.m_nNameBufLen;
		m_nNameStrLen = src.m_nNameStrLen;
		m_szValue = src.m_szValue;
		m_nValueBufLen = src.m_nValueBufLen;
		m_nValueStrLen = src.m_nValueStrLen;

		src.m_szName = (PSTR)g_szNull8;
		src.m_nNameBufLen = 0;
		src.m_nNameStrLen = 0;
		src.m_szValue = (PSTR)g_szNull8;
		src.m_nValueBufLen = 0;
		src.m_nValueStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CUrlParameter8 &CUrlParameter8::operator=(const CUrlParameterW &param)
{
	Assign(param);
	return *this;
}

//#################################################################################################
ERRCODE CUrlParameter8::Assign(const CUrlParameter8 &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &param)
	{
		if(param.m_nNameStrLen)
		{
			if(AllocName(param.m_nNameStrLen + 1))
			{
				std::memcpy(m_szName, param.m_szName, param.m_nNameStrLen);
				m_nNameStrLen = param.m_nNameStrLen;
				m_szName[m_nNameStrLen] = g_chNull8;

				if(param.m_nValueStrLen)
				{
					if(AllocValue(param.m_nValueStrLen + 1))
					{
						std::memcpy(m_szValue, param.m_szValue, param.m_nValueStrLen);
						m_nValueStrLen = param.m_nValueStrLen;
						m_szValue[m_nValueStrLen] = g_chNull8;
					}
					else
					{
						DeallocName();
						DeallocValue();
						nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
					}
				}
				else
					DeallocValue();
			}
			else
			{
				DeallocName();
				DeallocValue();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
		{
			DeallocName();
			DeallocValue();
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::Assign(const CUrlParameterW &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(param.m_nNameStrLen)
	{
		if(AllocName(WideToUtf8(param.m_szName, param.m_nNameStrLen, nullptr, 0) + 1))
		{
			m_nNameStrLen = WideToUtf8(param.m_szName, param.m_nNameStrLen, m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNull8;

			if(param.m_nValueStrLen)
			{
				if(AllocValue(WideToUtf8(param.m_szValue, param.m_nValueStrLen, nullptr, 0) + 1))
				{
					m_nValueStrLen = WideToUtf8(param.m_szValue, param.m_nValueStrLen, m_szValue, m_nValueBufLen);
					m_szValue[m_nValueStrLen] = g_chNull8;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::Assign(const CStr8 &strName, const CStr8 &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::memcpy(m_szName, (PCSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNull8;

			CStr8 strEscapedValue(strValue);
			if(!bAlreadyEscaped)
				UrlEscape8(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(strEscapedValue.GetLength() + 1))
				{
					std::memcpy(m_szValue, (PCSTR)strEscapedValue, strEscapedValue.GetLength());
					m_nValueStrLen = strEscapedValue.GetLength();
					m_szValue[m_nValueStrLen] = g_chNull8;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::Assign(const CStrW &strName, const CStrW &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(WideToUtf8(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = WideToUtf8(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNull8;

			CStrW strEscapedValue(strValue);
			if(!bAlreadyEscaped)
				UrlEscapeW(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
				{
					m_nValueStrLen = WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
					m_szValue[m_nValueStrLen] = g_chNull8;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::Assign(PCSTR szName, PCSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::memcpy(m_szName, (PCSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNull8;

			CStr8 strEscapedValue(szValue);
			if(!bAlreadyEscaped)
				UrlEscape8(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(strEscapedValue.GetLength() + 1))
				{
					std::memcpy(m_szValue, (PCSTR)strEscapedValue, strEscapedValue.GetLength());
					m_nValueStrLen = strEscapedValue.GetLength();
					m_szValue[m_nValueStrLen] = g_chNull8;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::Assign(PCWSTR szName, PCWSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(WideToUtf8(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = WideToUtf8(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNull8;

			CStrW strEscapedValue(szValue);
			if(!bAlreadyEscaped)
				UrlEscapeW(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
				{
					m_nValueStrLen = WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
					m_szValue[m_nValueStrLen] = g_chNull8;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(const CUrlParameter8 &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &param)
	{
		if(param.m_nNameStrLen)
		{
			if(AllocName(param.m_nNameStrLen + 1))
			{
				std::memcpy(m_szName, param.m_szName, param.m_nNameStrLen);
				m_nNameStrLen = param.m_nNameStrLen;
				m_szName[m_nNameStrLen] = g_chNull8;
			}
			else
			{
				DeallocName();
				DeallocValue();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
		{
			DeallocName();
			DeallocValue();
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(const CUrlParameterW &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(param.m_nNameStrLen)
	{
		if(AllocName(WideToUtf8(param.m_szName, param.m_nNameStrLen, nullptr, 0) + 1))
		{
			m_nNameStrLen = WideToUtf8(param.m_szName, param.m_nNameStrLen, m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(const CStr8 &strName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::memcpy(m_szName, (PCSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(const CStrW &strName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(WideToUtf8(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = WideToUtf8(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(PCSTR szName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::memcpy(m_szName, (PCSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(PCWSTR szName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(WideToUtf8(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = WideToUtf8(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(const char chName)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(chName);
	UrlEscape8(strEscapedName);
	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::memcpy(m_szName, (PCSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetName(const wchar_t chName)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(chName);
	UrlEscapeW(strEscapedName);
	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(WideToUtf8(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = WideToUtf8(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(const CUrlParameter8 &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &param)
	{
		if(param.m_nValueStrLen)
		{
			if(AllocValue(param.m_nValueStrLen + 1))
			{
				std::memcpy(m_szValue, param.m_szValue, param.m_nValueStrLen);
				m_nValueStrLen = param.m_nValueStrLen;
				m_szValue[m_nValueStrLen] = g_chNull8;
			}
			else
			{
				DeallocName();
				DeallocValue();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
			DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(const CUrlParameterW &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(param.m_nValueStrLen)
	{
		if(AllocValue(WideToUtf8(param.m_szValue, param.m_nValueStrLen, nullptr, 0) + 1))
		{
			m_nValueStrLen = WideToUtf8(param.m_szValue, param.m_nValueStrLen, m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(const CStr8 &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedValue(strValue);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(strEscapedValue.GetLength() + 1))
		{
			std::memcpy(m_szValue, (PCSTR)strEscapedValue, strEscapedValue.GetLength());
			m_nValueStrLen = strEscapedValue.GetLength();
			m_szValue[m_nValueStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(const CStrW &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedValue(strValue);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
		{
			m_nValueStrLen = WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(PCSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedValue(szValue);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(strEscapedValue.GetLength() + 1))
		{
			std::memcpy(m_szValue, (PCSTR)strEscapedValue, strEscapedValue.GetLength());
			m_nValueStrLen = strEscapedValue.GetLength();
			m_szValue[m_nValueStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(PCWSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedValue(szValue);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
		{
			m_nValueStrLen = WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(const char chValue)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedValue(chValue);
	UrlEscape8(strEscapedValue);
	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(strEscapedValue.GetLength() + 1))
		{
			std::memcpy(m_szValue, (PCSTR)strEscapedValue, strEscapedValue.GetLength());
			m_nValueStrLen = strEscapedValue.GetLength();
			m_szValue[m_nValueStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameter8::SetValue(const wchar_t chValue)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedValue(chValue);
	UrlEscapeW(strEscapedValue);
	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
		{
			m_nValueStrLen = WideToUtf8(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNull8;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
void CUrlParameter8::DeleteValue(void)
{
	DeallocValue();
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CUrlParameter8 &param)
{
	stream << param.m_szName;
	if(param.m_nValueStrLen)
		stream << g_chEqual8 << param.m_szValue;
	return stream;
}

//#################################################################################################
bool CUrlParameter8::operator==(const CUrlParameter8 &param) const
{
	return (Compare(param) == 0);
}

//#################################################################################################
bool CUrlParameter8::operator!=(const CUrlParameter8 &param) const
{
	return (Compare(param) != 0);
}

//#################################################################################################
bool CUrlParameter8::operator<(const CUrlParameter8 &param) const
{
	return (Compare(param) < 0);
}

//#################################################################################################
bool CUrlParameter8::operator<=(const CUrlParameter8 &param) const
{
	return (Compare(param) <= 0);
}

//#################################################################################################
bool CUrlParameter8::operator>(const CUrlParameter8 &param) const
{
	return (Compare(param) > 0);
}

//#################################################################################################
bool CUrlParameter8::operator>=(const CUrlParameter8 &param) const
{
	return (Compare(param) >= 0);
}

//#################################################################################################
int CUrlParameter8::Compare(const CUrlParameter8 &param, const bool bCaseInsensitive) const
{
	int nCompareRtn = StringCompare8(m_szName, m_nNameStrLen, param.m_szName, param.m_nNameStrLen, bCaseInsensitive);
	if(nCompareRtn == 0)
		nCompareRtn = StringCompare8(m_szValue, m_nValueStrLen, param.m_szValue, param.m_nValueStrLen, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CUrlParameter8::Compare(const CStr8 &strName, const CStr8 &strValue, const bool bCaseInsensitive) const
{
	int nCompareRtn = StringCompare8(m_szName, m_nNameStrLen, strName, strName.GetLength(), bCaseInsensitive);
	if(nCompareRtn == 0)
		nCompareRtn = StringCompare8(m_szValue, m_nValueStrLen, strValue, strValue.GetLength(), bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CUrlParameter8::Compare(PCSTR szName, PCSTR szValue, const bool bCaseInsensitive) const
{
	int nCompareRtn = StringCompare8(m_szName, m_nNameStrLen, szName, StringGetLength(szName), bCaseInsensitive);
	if(nCompareRtn == 0)
		nCompareRtn = StringCompare8(m_szValue, m_nValueStrLen, szValue, StringGetLength(szValue), bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
bool CUrlParameter8::IsValidParameter(const CStr8 &strParam)
{	// Valid Url parameters are of length 1 or greater,
	// may contain at most one '='
	// cannot begin with '='
	// and contain any character except the following:
	//   / ? & #

	// Length greater than 1
	size_t nStrLen = strParam.GetLength();
	if(nStrLen == 0)
		return false;

	// Does it begin with an equal?
	if(strParam[0] == g_chEqual8)
		return false;

	// Does it contain more than 1 equal?
	size_t nPos;
	if((nPos = strParam.Find(g_chEqual8)) != CStr8::not_found && strParam.Contains(g_chEqual8, nPos + 1))
		return false;

	// Does it contain an invalid character?
	if(FindFirstOf(strParam, nStrLen, g_szInvalidParam8, g_nInvalidParamLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlParameter8::IsValidParameter(PCSTR szParam)
{	// Valid Url parameters are of length 1 or greater,
	// may contain at most one '='
	// cannot begin with '='
	// and contain any character except the following:
	//   / ? & #

	// Cannot be nullptr
	if(szParam == nullptr)
		return false;

	// Length greater than 1
	size_t nStrLen = StringGetLength(szParam);
	if(nStrLen == 0)
		return false;

	// Does it begin with an equal?
	if(szParam[0] == g_chEqual8)
		return false;

	// Does it contain more than 1 equal?
	PCSTR szPos;
	if((szPos = (PCSTR)std::memchr(szParam, g_chEqual8, nStrLen)) != nullptr && std::memchr(szPos + 1, g_chEqual8, nStrLen - (size_t)(szPos - szParam) - 1) != nullptr)
		return false;

	// Does it contain an invalid character?
	if(FindFirstOf(szParam, nStrLen, g_szInvalidParam8, g_nInvalidParamLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlParameter8::IsValidParameter(const char chParam)
{	// Valid Url parameters are of length 1 or greater,
	// cannot begin with '='
	// and contain any character except the following:
	//   / ? & #

	// Cannot be nullptr or '='
	if(chParam == g_chNull8 || chParam == g_chEqual8)
		return false;

	// Is it an invalid character?
	if(FindFirstOf(&chParam, 1, g_szInvalidParam8, g_nInvalidParamLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlParameter8::AllocName(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nNameBufLen)
	{	// The buffer is not large enough, realloc it
		PSTR sz;
		if(m_nNameBufLen)
			sz = (PSTR)std::realloc(m_szName, nBufLen);
		else
			sz = (PSTR)std::malloc(nBufLen);

		if(sz)
		{
			m_szName = sz;
			m_nNameBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CUrlParameter8::DeallocName(void)
{
	if(m_nNameBufLen)
		std::free(m_szName);
	m_szName = (PSTR)g_szNull8;
	m_nNameStrLen = m_nNameBufLen = 0;
}

//#################################################################################################
bool CUrlParameter8::AllocValue(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nValueBufLen)
	{	// The buffer is not large enough, realloc it
		PSTR sz;
		if(m_nValueBufLen)
			sz = (PSTR)std::realloc(m_szValue, nBufLen);
		else
			sz = (PSTR)std::malloc(nBufLen);

		if(sz)
		{
			m_szValue = sz;
			m_nValueBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CUrlParameter8::DeallocValue(void)
{
	if(m_nValueBufLen)
		std::free(m_szValue);
	m_szValue = (PSTR)g_szNull8;
	m_nValueStrLen = m_nValueBufLen = 0;
}

//#################################################################################################
PCSTR CUrlParameter8::FindFirstOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	PCSTR szResult = nullptr;
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if((szResult = (PCSTR)std::memchr(szFind, sz[nIndex], nFindLen)) != nullptr)
			return szResult;
	}

	return szResult;
}




/////////////////////////////////////////////////
// CUrlParameterW
/////////////////////////////////////////////////

//#################################################################################################
CUrlParameterW::CUrlParameterW(void)
	: m_szName((PWSTR)g_szNullW),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PWSTR)g_szNullW),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
}

//#################################################################################################
CUrlParameterW::CUrlParameterW(const CUrlParameterW &src)
	: m_szName((PWSTR)g_szNullW),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PWSTR)g_szNullW),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CUrlParameterW::CUrlParameterW(CUrlParameterW &&src) noexcept
	: m_szName(src.m_szName),
	m_nNameBufLen(src.m_nNameBufLen),
	m_nNameStrLen(src.m_nNameStrLen),
	m_szValue(src.m_szValue),
	m_nValueBufLen(src.m_nValueBufLen),
	m_nValueStrLen(src.m_nValueStrLen)
{
	src.m_szName = (PWSTR)g_szNullW;
	src.m_nNameBufLen = 0;
	src.m_nNameStrLen = 0;
	src.m_szValue = (PWSTR)g_szNullW;
	src.m_nValueBufLen = 0;
	src.m_nValueStrLen = 0;
}

//#################################################################################################
CUrlParameterW::CUrlParameterW(const CUrlParameter8 &param)
	: m_szName((PWSTR)g_szNullW),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PWSTR)g_szNullW),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(param);
}

//#################################################################################################
CUrlParameterW::CUrlParameterW(const CStrW &strName, const CStrW &strValue, const bool bAlreadyEscaped)
	: m_szName((PWSTR)g_szNullW),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PWSTR)g_szNullW),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(strName, strValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameterW::CUrlParameterW(const CStr8 &strName, const CStr8 &strValue, const bool bAlreadyEscaped)
	: m_szName((PWSTR)g_szNullW),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PWSTR)g_szNullW),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(strName, strValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameterW::CUrlParameterW(PCWSTR szName, PCWSTR szValue, const bool bAlreadyEscaped)
	: m_szName((PWSTR)g_szNullW),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PWSTR)g_szNullW),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(szName, szValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameterW::CUrlParameterW(PCSTR szName, PCSTR szValue, const bool bAlreadyEscaped)
	: m_szName((PWSTR)g_szNullW),
	m_nNameBufLen(0),
	m_nNameStrLen(0),
	m_szValue((PWSTR)g_szNullW),
	m_nValueBufLen(0),
	m_nValueStrLen(0)
{
	Assign(szName, szValue, bAlreadyEscaped);
}

//#################################################################################################
CUrlParameterW::~CUrlParameterW(void)
{
	DeallocName();
	DeallocValue();
}

//#################################################################################################
CUrlParameterW::operator CStrW(void) const
{
	return Get();
}

//#################################################################################################
CStrW CUrlParameterW::Get(const bool bAlwaysIncludeEqual) const
{
	CStrW str;

	if(m_nNameStrLen)
	{
		str.Assign(m_szName);
		if(m_nValueStrLen || bAlwaysIncludeEqual)
			str.Append(g_chEqualW);
		str.Append(m_szValue);
	}

	return str;
}

//#################################################################################################
size_t CUrlParameterW::GetLength(const bool bAlwaysIncludeEqual, const bool bIncludeNullTerm) const
{	// Length in characters
	size_t nSize = (bIncludeNullTerm) ? m_nNameStrLen + 1 : m_nNameStrLen;

	if(m_nNameStrLen && (m_nValueStrLen || bAlwaysIncludeEqual))
		nSize += m_nValueStrLen + 1;

	return nSize;
}

//#################################################################################################
size_t CUrlParameterW::GetSize(const bool bAlwaysIncludeEqual, const bool bIncludeNullTerm) const
{	// Size in bytes
	return GetLength(bAlwaysIncludeEqual, bIncludeNullTerm) * sizeof(wchar_t);
}

//#################################################################################################
PCWSTR CUrlParameterW::GetName(void) const noexcept
{
	return m_szName;
}

//#################################################################################################
size_t CUrlParameterW::GetNameLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nNameStrLen + 1 : m_nNameStrLen;
}

//#################################################################################################
size_t CUrlParameterW::GetNameSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nNameStrLen + 1 : m_nNameStrLen) * sizeof(wchar_t);
}

//#################################################################################################
PCWSTR CUrlParameterW::GetValue(void) const noexcept
{
	return m_szValue;
}

//#################################################################################################
size_t CUrlParameterW::GetValueLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nValueStrLen + 1 : m_nValueStrLen;
}

//#################################################################################################
size_t CUrlParameterW::GetValueSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nValueStrLen + 1 : m_nValueStrLen) * sizeof(wchar_t);
}

//#################################################################################################
void CUrlParameterW::Empty(void)
{
	DeallocName();
	DeallocValue();
}

//#################################################################################################
bool CUrlParameterW::IsEmpty(void) const noexcept
{
	return (m_nNameStrLen == 0);
}

//#################################################################################################
bool CUrlParameterW::IsValueEmpty(void) const noexcept
{
	return (m_nValueStrLen == 0);
}

//#################################################################################################
CUrlParameter8 CUrlParameterW::AsUtf8(void) const
{
	return CUrlParameter8(*this);
}

//#################################################################################################
CUrlParameterW CUrlParameterW::AsWide(void) const
{
	return *this;
}

#ifdef _WIN32
//#################################################################################################
CUrlParameterW CUrlParameterW::AsNative(void) const
{
	return *this;
}
#else
//#################################################################################################
CUrlParameter8 CUrlParameterW::AsNative(void) const
{
	return CUrlParameter8(*this);
}
#endif

//#################################################################################################
CUrlParameterW &CUrlParameterW::operator=(const CUrlParameterW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CUrlParameterW &CUrlParameterW::operator=(CUrlParameterW &&src) noexcept
{
	if(this != &src)
	{
		if(m_nNameBufLen)
			std::free(m_szName);
		if(m_nValueBufLen)
			std::free(m_szValue);

		m_szName = src.m_szName;
		m_nNameBufLen = src.m_nNameBufLen;
		m_nNameStrLen = src.m_nNameStrLen;
		m_szValue = src.m_szValue;
		m_nValueBufLen = src.m_nValueBufLen;
		m_nValueStrLen = src.m_nValueStrLen;

		src.m_szName = (PWSTR)g_szNullW;
		src.m_nNameBufLen = 0;
		src.m_nNameStrLen = 0;
		src.m_szValue = (PWSTR)g_szNullW;
		src.m_nValueBufLen = 0;
		src.m_nValueStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CUrlParameterW &CUrlParameterW::operator=(const CUrlParameter8 &param)
{
	Assign(param);
	return *this;
}

//#################################################################################################
ERRCODE CUrlParameterW::Assign(const CUrlParameterW &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &param)
	{
		if(param.m_nNameStrLen)
		{
			if(AllocName(param.m_nNameStrLen + 1))
			{
				std::wmemcpy(m_szName, param.m_szName, param.m_nNameStrLen);
				m_nNameStrLen = param.m_nNameStrLen;
				m_szName[m_nNameStrLen] = g_chNullW;

				if(param.m_nValueStrLen)
				{
					if(AllocValue(param.m_nValueStrLen + 1))
					{
						std::wmemcpy(m_szValue, param.m_szValue, param.m_nValueStrLen);
						m_nValueStrLen = param.m_nValueStrLen;
						m_szValue[m_nValueStrLen] = g_chNullW;
					}
					else
					{
						DeallocName();
						DeallocValue();
						nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
					}
				}
				else
					DeallocValue();
			}
			else
			{
				DeallocName();
				DeallocValue();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
		{
			DeallocName();
			DeallocValue();
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::Assign(const CUrlParameter8 &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(param.m_nNameStrLen)
	{
		if(AllocName(Utf8ToWide(param.m_szName, param.m_nNameStrLen, nullptr, 0) + 1))
		{
			m_nNameStrLen = Utf8ToWide(param.m_szName, param.m_nNameStrLen, m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNullW;

			if(param.m_nValueStrLen)
			{
				if(AllocValue(Utf8ToWide(param.m_szValue, param.m_nValueStrLen, nullptr, 0) + 1))
				{
					m_nValueStrLen = Utf8ToWide(param.m_szValue, param.m_nValueStrLen, m_szValue, m_nValueBufLen);
					m_szValue[m_nValueStrLen] = g_chNullW;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::Assign(const CStrW &strName, const CStrW &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::wmemcpy(m_szName, (PCWSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNullW;

			CStrW strEscapedValue(strValue);
			if(!bAlreadyEscaped)
				UrlEscapeW(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(strEscapedValue.GetLength() + 1))
				{
					std::wmemcpy(m_szValue, (PCWSTR)strEscapedValue, strEscapedValue.GetLength());
					m_nValueStrLen = strEscapedValue.GetLength();
					m_szValue[m_nValueStrLen] = g_chNullW;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::Assign(const CStr8 &strName, const CStr8 &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(Utf8ToWide(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = Utf8ToWide(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNullW;

			CStr8 strEscapedValue(strValue);
			if(!bAlreadyEscaped)
				UrlEscape8(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
				{
					m_nValueStrLen = Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
					m_szValue[m_nValueStrLen] = g_chNullW;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::Assign(PCWSTR szName, PCWSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::wmemcpy(m_szName, (PCWSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNullW;

			CStrW strEscapedValue(szValue);
			if(!bAlreadyEscaped)
				UrlEscapeW(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(strEscapedValue.GetLength() + 1))
				{
					std::wmemcpy(m_szValue, (PCWSTR)strEscapedValue, strEscapedValue.GetLength());
					m_nValueStrLen = strEscapedValue.GetLength();
					m_szValue[m_nValueStrLen] = g_chNullW;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::Assign(PCSTR szName, PCSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(Utf8ToWide(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = Utf8ToWide(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNullW;

			CStr8 strEscapedValue(szValue);
			if(!bAlreadyEscaped)
				UrlEscape8(strEscapedValue);

			if(!strEscapedValue.IsEmpty())
			{
				if(AllocValue(Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
				{
					m_nValueStrLen = Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
					m_szValue[m_nValueStrLen] = g_chNullW;
				}
				else
				{
					DeallocName();
					DeallocValue();
					nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
				}
			}
			else
				DeallocValue();
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(const CUrlParameterW &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &param)
	{
		if(param.m_nNameStrLen)
		{
			if(AllocName(param.m_nNameStrLen + 1))
			{
				std::wmemcpy(m_szName, param.m_szName, param.m_nNameStrLen);
				m_nNameStrLen = param.m_nNameStrLen;
				m_szName[m_nNameStrLen] = g_chNullW;
			}
			else
			{
				DeallocName();
				DeallocValue();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
		{
			DeallocName();
			DeallocValue();
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(const CUrlParameter8 &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(param.m_nNameStrLen)
	{
		if(AllocName(Utf8ToWide(param.m_szName, param.m_nNameStrLen, nullptr, 0) + 1))
		{
			m_nNameStrLen = Utf8ToWide(param.m_szName, param.m_nNameStrLen, m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(const CStrW &strName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::wmemcpy(m_szName, (PCWSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(const CStr8 &strName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(strName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(Utf8ToWide(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = Utf8ToWide(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(PCWSTR szName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::wmemcpy(m_szName, (PCWSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(PCSTR szName, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(szName);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedName);

	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(Utf8ToWide(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = Utf8ToWide(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(const wchar_t chName)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedName(chName);
	UrlEscapeW(strEscapedName);
	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(strEscapedName.GetLength() + 1))
		{
			std::wmemcpy(m_szName, (PCWSTR)strEscapedName, strEscapedName.GetLength());
			m_nNameStrLen = strEscapedName.GetLength();
			m_szName[m_nNameStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetName(const char chName)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedName(chName);
	UrlEscape8(strEscapedName);
	if(!strEscapedName.IsEmpty())
	{
		if(AllocName(Utf8ToWide(strEscapedName, strEscapedName.GetLength(), nullptr, 0) + 1))
		{
			m_nNameStrLen = Utf8ToWide(strEscapedName, strEscapedName.GetLength(), m_szName, m_nNameBufLen);
			m_szName[m_nNameStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
	{
		DeallocName();
		DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(const CUrlParameterW &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &param)
	{
		if(param.m_nValueStrLen)
		{
			if(AllocValue(param.m_nValueStrLen + 1))
			{
				std::wmemcpy(m_szValue, param.m_szValue, param.m_nValueStrLen);
				m_nValueStrLen = param.m_nValueStrLen;
				m_szValue[m_nValueStrLen] = g_chNullW;
			}
			else
			{
				DeallocName();
				DeallocValue();
				nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
			DeallocValue();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(const CUrlParameter8 &param)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(param.m_nValueStrLen)
	{
		if(AllocValue(Utf8ToWide(param.m_szValue, param.m_nValueStrLen, nullptr, 0) + 1))
		{
			m_nValueStrLen = Utf8ToWide(param.m_szValue, param.m_nValueStrLen, m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(const CStrW &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedValue(strValue);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(strEscapedValue.GetLength() + 1))
		{
			std::wmemcpy(m_szValue, (PCWSTR)strEscapedValue, strEscapedValue.GetLength());
			m_nValueStrLen = strEscapedValue.GetLength();
			m_szValue[m_nValueStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(const CStr8 &strValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedValue(strValue);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
		{
			m_nValueStrLen = Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(PCWSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedValue(szValue);
	if(!bAlreadyEscaped)
		UrlEscapeW(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(strEscapedValue.GetLength() + 1))
		{
			std::wmemcpy(m_szValue, (PCWSTR)strEscapedValue, strEscapedValue.GetLength());
			m_nValueStrLen = strEscapedValue.GetLength();
			m_szValue[m_nValueStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(PCSTR szValue, const bool bAlreadyEscaped)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedValue(szValue);
	if(!bAlreadyEscaped)
		UrlEscape8(strEscapedValue);

	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
		{
			m_nValueStrLen = Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(const wchar_t chValue)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStrW strEscapedValue(chValue);
	UrlEscapeW(strEscapedValue);
	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(strEscapedValue.GetLength() + 1))
		{
			std::wmemcpy(m_szValue, (PCWSTR)strEscapedValue, strEscapedValue.GetLength());
			m_nValueStrLen = strEscapedValue.GetLength();
			m_szValue[m_nValueStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlParameterW::SetValue(const char chValue)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	CStr8 strEscapedValue(chValue);
	UrlEscape8(strEscapedValue);
	if(!strEscapedValue.IsEmpty())
	{
		if(AllocValue(Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), nullptr, 0) + 1))
		{
			m_nValueStrLen = Utf8ToWide(strEscapedValue, strEscapedValue.GetLength(), m_szValue, m_nValueBufLen);
			m_szValue[m_nValueStrLen] = g_chNullW;
		}
		else
		{
			DeallocName();
			DeallocValue();
			nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	else
		DeallocValue();

	return nErrorCode;
}

//#################################################################################################
void CUrlParameterW::DeleteValue(void)
{
	DeallocValue();
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CUrlParameterW &param)
{
	stream << param.m_szName;
	if(param.m_nValueStrLen)
		stream << g_chEqualW << param.m_szValue;
	return stream;
}

//#################################################################################################
bool CUrlParameterW::operator==(const CUrlParameterW &param) const
{
	return (Compare(param) == 0);
}

//#################################################################################################
bool CUrlParameterW::operator!=(const CUrlParameterW &param) const
{
	return (Compare(param) != 0);
}

//#################################################################################################
bool CUrlParameterW::operator<(const CUrlParameterW &param) const
{
	return (Compare(param) < 0);
}

//#################################################################################################
bool CUrlParameterW::operator<=(const CUrlParameterW &param) const
{
	return (Compare(param) <= 0);
}

//#################################################################################################
bool CUrlParameterW::operator>(const CUrlParameterW &param) const
{
	return (Compare(param) > 0);
}

//#################################################################################################
bool CUrlParameterW::operator>=(const CUrlParameterW &param) const
{
	return (Compare(param) >= 0);
}

//#################################################################################################
int CUrlParameterW::Compare(const CUrlParameterW &param, const bool bCaseInsensitive) const
{
	int nCompareRtn = StringCompareW(m_szName, m_nNameStrLen, param.m_szName, param.m_nNameStrLen, bCaseInsensitive);
	if(nCompareRtn == 0)
		nCompareRtn = StringCompareW(m_szValue, m_nValueStrLen, param.m_szValue, param.m_nValueStrLen, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CUrlParameterW::Compare(const CStrW &strName, const CStrW &strValue, const bool bCaseInsensitive) const
{
	int nCompareRtn = StringCompareW(m_szName, m_nNameStrLen, strName, strName.GetLength(), bCaseInsensitive);
	if(nCompareRtn == 0)
		nCompareRtn = StringCompareW(m_szValue, m_nValueStrLen, strValue, strValue.GetLength(), bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CUrlParameterW::Compare(PCWSTR szName, PCWSTR szValue, const bool bCaseInsensitive) const
{
	int nCompareRtn = StringCompareW(m_szName, m_nNameStrLen, szName, StringGetLength(szName), bCaseInsensitive);
	if(nCompareRtn == 0)
		nCompareRtn = StringCompareW(m_szValue, m_nValueStrLen, szValue, StringGetLength(szValue), bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
bool CUrlParameterW::IsValidParameter(const CStrW &strParam)
{	// Valid Url parameters are of length 1 or greater,
	// may contain at most one '='
	// cannot begin with '='
	// and contain any character except the following:
	//   / ? & #

	// Length greater than 1
	size_t nStrLen = strParam.GetLength();
	if(nStrLen == 0)
		return false;

	// Does it begin with an equal?
	if(strParam[0] == g_chEqualW)
		return false;

	// Does it contain more than 1 equal?
	size_t nPos;
	if((nPos = strParam.Find(g_chEqualW)) != CStrW::not_found && strParam.Contains(g_chEqualW, nPos + 1))
		return false;

	// Does it contain an invalid character?
	if(FindFirstOf(strParam, nStrLen, g_szInvalidParamW, g_nInvalidParamLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlParameterW::IsValidParameter(PCWSTR szParam)
{	// Valid Url parameters are of length 1 or greater,
	// may contain at most one '='
	// cannot begin with '='
	// and contain any character except the following:
	//   / ? & #

	// Cannot be nullptr
	if(szParam == nullptr)
		return false;

	// Length greater than 1
	size_t nStrLen = StringGetLength(szParam);
	if(nStrLen == 0)
		return false;

	// Does it begin with an equal?
	if(szParam[0] == g_chEqualW)
		return false;

	// Does it contain more than 1 equal?
	PCWSTR szPos;
	if((szPos = (PCWSTR)std::wmemchr(szParam, g_chEqualW, nStrLen)) != nullptr && std::wmemchr(szPos + 1, g_chEqualW, nStrLen - (size_t)(szPos - szParam) - 1) != nullptr)
		return false;

	// Does it contain an invalid character?
	if(FindFirstOf(szParam, nStrLen, g_szInvalidParamW, g_nInvalidParamLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlParameterW::IsValidParameter(const wchar_t chParam)
{	// Valid Url parameters are of length 1 or greater,
	// cannot begin with '='
	// and contain any character except the following:
	//   / ? & #

	// Cannot be nullptr or '='
	if(chParam == g_chNullW || chParam == g_chEqualW)
		return false;

	// Is it an invalid character?
	if(FindFirstOf(&chParam, 1, g_szInvalidParamW, g_nInvalidParamLen))
		return false;

	return true;
}

//#################################################################################################
bool CUrlParameterW::AllocName(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nNameBufLen)
	{	// The buffer is not large enough, realloc it
		PWSTR sz;
		if(m_nNameBufLen)
			sz = (PWSTR)std::realloc(m_szName, nBufLen * sizeof(wchar_t));
		else
			sz = (PWSTR)std::malloc(nBufLen * sizeof(wchar_t));

		if(sz)
		{
			m_szName = sz;
			m_nNameBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CUrlParameterW::DeallocName(void)
{
	if(m_nNameBufLen)
		std::free(m_szName);
	m_szName = (PWSTR)g_szNullW;
	m_nNameStrLen = m_nNameBufLen = 0;
}

//#################################################################################################
bool CUrlParameterW::AllocValue(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nValueBufLen)
	{	// The buffer is not large enough, realloc it
		PWSTR sz;
		if(m_nValueBufLen)
			sz = (PWSTR)std::realloc(m_szValue, nBufLen * sizeof(wchar_t));
		else
			sz = (PWSTR)std::malloc(nBufLen * sizeof(wchar_t));

		if(sz)
		{
			m_szValue = sz;
			m_nValueBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CUrlParameterW::DeallocValue(void)
{
	if(m_nValueBufLen)
		std::free(m_szValue);
	m_szValue = (PWSTR)g_szNullW;
	m_nValueStrLen = m_nValueBufLen = 0;
}

//#################################################################################################
PCWSTR CUrlParameterW::FindFirstOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	PCWSTR szResult = nullptr;
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if((szResult = (PCWSTR)std::wmemchr(szFind, sz[nIndex], nFindLen)) != nullptr)
			return szResult;
	}

	return szResult;
}




/////////////////////////////////////////////////
// CUrl8
/////////////////////////////////////////////////

//#################################################################################################
CUrl8::CUrl8(void)
	: m_nPort(no_port)
{
}

//#################################################################################################
CUrl8::CUrl8(const CUrl8 &src)
	: m_nPort(no_port)
{
	Assign(src);
}

//#################################################################################################
CUrl8::CUrl8(CUrl8 &&src) noexcept
	: m_strScheme(std::move(src.m_strScheme)),
	m_strUsername(std::move(src.m_strUsername)),
	m_strPassword(std::move(src.m_strPassword)),
	m_domain(std::move(src.m_domain)),
	m_nPort(src.m_nPort),
	m_vecPath(std::move(src.m_vecPath)),
	m_vecParams(std::move(src.m_vecParams)),
	m_strFragment(std::move(src.m_strFragment))
{
	src.m_nPort = no_port;
}

//#################################################################################################
CUrl8::CUrl8(const CUrlW &url)
	: m_nPort(no_port)
{
	Assign(url);
}

//#################################################################################################
CUrl8::CUrl8(const CDomainName8 &domain)
	: m_domain(domain),
	m_nPort(no_port)
{
}

//#################################################################################################
CUrl8::CUrl8(const CDomainNameW &domain)
	: m_domain(domain),
	m_nPort(no_port)
{
}

//#################################################################################################
CUrl8::CUrl8(const CStr8 &str)
	: m_nPort(no_port)
{
	ParseUrl(str);
}

//#################################################################################################
CUrl8::CUrl8(const CStrW &str)
	: m_nPort(no_port)
{
	ParseUrl(str.AsUtf8());
}

//#################################################################################################
CUrl8::CUrl8(PCSTR sz)
	: m_nPort(no_port)
{
	ParseUrl(CStr8(sz));
}

//#################################################################################################
CUrl8::CUrl8(PCWSTR sz)
	: m_nPort(no_port)
{
	ParseUrl(CStr8(sz));
}

//#################################################################################################
CUrl8::operator CStr8(void) const
{
	return Get();
}

//#################################################################################################
CStr8 CUrl8::Get(const bool bIncludeScheme, const bool bIncludeUsername, const bool bIncludePassword, const bool bIncludePort, const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{	// Convert everything into a single string
	CStr8 str;

	if(!IsEmpty())
	{	// Scheme
		if(bIncludeScheme && !m_strScheme.IsEmpty())
		{
			str.Append(m_strScheme);
			str.Append(g_szSchemeSeparator8);
		}

		// Username and password
		if(bIncludeUsername && !m_strUsername.IsEmpty())
		{
			str.Append(m_strUsername);

			if(bIncludePassword && !m_strPassword.IsEmpty())
			{
				str.Append(g_chColon8);
				str.Append(m_strPassword);
			}

			str.Append(g_chAt8);
		}

		// Domain
		str.Append(m_domain);

		// Port
		if(bIncludePort && m_nPort != no_port)
		{
			str.Append(g_chColon8);
			str.Append(IntegerToString8(m_nPort));
		}

		if(!m_vecPath.empty() || (bIncludeParams && !m_vecParams.empty()) || (bIncludeFragment && !m_strFragment.IsEmpty()))
			str.Append(g_chForwardSlash8);

		// Path
		if(!m_vecPath.empty())
		{
			auto itr = m_vecPath.begin();
			str.Append(itr->Get());

			while(++itr != m_vecPath.end())
			{
				str.Append(g_chForwardSlash8);
				str.Append(itr->Get());
			}
		}

		// Parameters
		if(bIncludeParams && !m_vecParams.empty())
		{
			str.Append(g_chQuestionMark8);

			auto itr = m_vecParams.begin();
			str.Append(itr->Get(bAlwaysIncludeEqual));

			while(++itr != m_vecParams.end())
			{
				str.Append(g_chAmpersand8);
				str.Append(itr->Get(bAlwaysIncludeEqual));
			}
		}

		// Fragment
		if(bIncludeFragment && !m_strFragment.IsEmpty())
		{
			str.Append(g_chHash8);
			str.Append(m_strFragment);
		}
	}

	return str;
}

//#################################################################################################
size_t CUrl8::GetLength(const bool bIncludeNullTerm, const bool bIncludeScheme, const bool bIncludeUsername, const bool bIncludePassword, const bool bIncludePort, const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{	// Length in characters
	size_t nStrLen = 0;

	if(!IsEmpty())
	{	// Scheme
		if(bIncludeScheme && !m_strScheme.IsEmpty())
			nStrLen += m_strScheme.GetLength() + g_nSchemeSeparatorLen;

		// Username and password
		if(bIncludeUsername && !m_strUsername.IsEmpty())
		{
			nStrLen += m_strUsername.GetLength() + 1;	// Add one for the at sign

			if(bIncludePassword && !m_strPassword.IsEmpty())
				nStrLen += m_strPassword.GetLength() + 1;	// Add one for the colon
		}

		// Domain
		nStrLen += m_domain.GetLength();

		// Port
		if(bIncludePort && m_nPort != no_port)
			nStrLen += IntegerToString8(m_nPort).GetLength() + 1;	// Add one for the colon

		if(!m_vecPath.empty() || (bIncludeParams && !m_vecParams.empty()) || (bIncludeFragment && !m_strFragment.IsEmpty()))
			++nStrLen;	// Add one for the backslash

		// Path
		if(!m_vecPath.empty())
		{
			auto itr = m_vecPath.begin();
			nStrLen += itr->GetLength();

			while(++itr != m_vecPath.end())
				nStrLen += itr->GetLength() + 1;	// Add one for the backslash
		}

		// Parameters
		if(bIncludeParams && !m_vecParams.empty())
		{
			auto itr = m_vecParams.begin();
			nStrLen += itr->Get(bAlwaysIncludeEqual).GetLength() + 1;	// Add one for the question mark

			while(++itr != m_vecParams.end())
				nStrLen += itr->Get(bAlwaysIncludeEqual).GetLength() + 1;	// Add one for the ampersand
		}

		// Fragment
		if(bIncludeFragment && !m_strFragment.IsEmpty())
			nStrLen += m_strFragment.GetLength() + 1;	// Add one for the hash

		if(bIncludeNullTerm)
			++nStrLen;
	}

	return nStrLen;
}

//#################################################################################################
size_t CUrl8::GetSize(const bool bIncludeNullTerm, const bool bIncludeScheme, const bool bIncludeUsername, const bool bIncludePassword, const bool bIncludePort, const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{	// Size in bytes
	return GetLength(bIncludeNullTerm, bIncludeScheme, bIncludeUsername, bIncludePassword, bIncludePort, bIncludeParams, bAlwaysIncludeEqual, bIncludeFragment);
}

//#################################################################################################
CUrl8 &CUrl8::operator=(const CUrl8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(CUrl8 &&src) noexcept
{
	if(this != &src)
	{
		m_strScheme = std::move(src.m_strScheme);
		m_strUsername = std::move(src.m_strUsername);
		m_strPassword = std::move(src.m_strPassword);
		m_domain = std::move(src.m_domain);
		m_nPort = src.m_nPort;
		m_vecPath = std::move(src.m_vecPath);
		m_vecParams = std::move(src.m_vecParams);
		m_strFragment = std::move(src.m_strFragment);

		src.m_nPort = no_port;
	}

	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(const CUrlW &url)
{
	Assign(url);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(const CDomainName8 &domain)
{
	Empty();
	m_domain.Assign(domain);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(const CDomainNameW &domain)
{
	Empty();
	m_domain.Assign(domain);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(const CStr8 &str)
{
	ParseUrl(str);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(const CStrW &str)
{
	ParseUrl(str.AsUtf8());
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(PCSTR sz)
{
	ParseUrl(CStr8(sz));
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator=(PCWSTR sz)
{
	ParseUrl(CStr8(sz));
	return *this;
}

//#################################################################################################
ERRCODE CUrl8::Assign(const CUrl8 &url)
{
	if(this != &url)
	{
		Empty();

		m_strScheme.Assign(url.m_strScheme);
		m_strUsername.Assign(url.m_strUsername);
		m_strPassword.Assign(url.m_strPassword);
		m_domain.Assign(url.m_domain);
		m_nPort = url.m_nPort;

		// Add segments to the path
		for(const auto &segment : url.m_vecPath)
			m_vecPath.push_back(segment);

		// Add parameters to the path
		for(const auto &param : url.m_vecParams)
			m_vecParams.push_back(param);

		m_strFragment.Assign(url.m_strFragment);
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrl8::Assign(const CUrlW &url)
{
	Empty();

	m_strScheme.Assign(url.m_strScheme);
	m_strUsername.Assign(url.m_strUsername);
	m_strPassword.Assign(url.m_strPassword);
	m_domain.Assign(url.m_domain);
	m_nPort = url.m_nPort;

	// Add segments to the path
	for(const auto &segment : url.m_vecPath)
		m_vecPath.emplace_back(segment);

	// Add parameters to the path
	for(const auto &param : url.m_vecParams)
		m_vecParams.emplace_back(param);

	m_strFragment.Assign(url.m_strFragment);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrl8::Assign(const CDomainName8 &domain)
{
	Empty();
	m_domain.Assign(domain);
	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrl8::Assign(const CDomainNameW &domain)
{
	Empty();
	m_domain.Assign(domain);
	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrl8::Assign(const CStr8 &str)
{
	return ParseUrl(str);
}

//#################################################################################################
ERRCODE CUrl8::Assign(const CStrW &str)
{
	return ParseUrl(str.AsUtf8());
}

//#################################################################################################
ERRCODE CUrl8::Assign(PCSTR sz)
{
	return ParseUrl(CStr8(sz));
}

//#################################################################################################
ERRCODE CUrl8::Assign(PCWSTR sz)
{
	return ParseUrl(CStr8(sz));
}

//#################################################################################################
CUrl8 &CUrl8::operator+=(const CDomainName8 &domain)
{
	Append(domain);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator+=(const CDomainNameW &domain)
{
	Append(domain);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator+=(const CUrlPathSegment8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator+=(const CUrlPathSegmentW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator+=(const CUrlParameter8 &param)
{
	Append(param);
	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::operator+=(const CUrlParameterW &param)
{
	Append(param);
	return *this;
}

//#################################################################################################
ERRCODE CUrl8::Append(const CDomainName8 &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsEmpty())
		m_domain.Assign(domain);
	else
		nErrorCode = FW_ERROR_INVALID_DATA;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrl8::Append(const CDomainNameW &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsEmpty())
		m_domain.Assign(domain);
	else
		nErrorCode = FW_ERROR_INVALID_DATA;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrl8::Append(const CUrlPathSegment8 &path)
{
	if(!path.IsEmpty())
		m_vecPath.push_back(path);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrl8::Append(const CUrlPathSegmentW &path)
{
	if(!path.IsEmpty())
		m_vecPath.emplace_back(path);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrl8::Append(const CUrlParameter8 &param)
{
	if(!param.IsEmpty())
		m_vecParams.push_back(param);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrl8::Append(const CUrlParameterW &param)
{
	if(!param.IsEmpty())
		m_vecParams.emplace_back(param);

	return FW_NO_ERROR;
}

#ifndef _WIN32
//#################################################################################################
CUrl8 operator+(const CUrl8 &url, const CUrlPathSegment8 &path)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CUrl8 &url, const CUrlPathSegmentW &path)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CUrlW &url, const CUrlPathSegment8 &path)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CUrlW &url, const CUrlPathSegmentW &path)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CUrl8 &url, const CUrlParameter8 &param)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CUrl8 &url, const CUrlParameterW &param)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CUrlW &url, const CUrlParameter8 &param)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CUrlW &url, const CUrlParameterW &param)
{
	CUrl8 urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainName8 &domain, const CUrlPathSegment8 &path)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainName8 &domain, const CUrlPathSegmentW &path)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainNameW &domain, const CUrlPathSegment8 &path)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainNameW &domain, const CUrlPathSegmentW &path)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainName8 &domain, const CUrlParameter8 &param)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainName8 &domain, const CUrlParameterW &param)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainNameW &domain, const CUrlParameter8 &param)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrl8 operator+(const CDomainNameW &domain, const CUrlParameterW &param)
{
	CUrl8 urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}
#endif

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CUrl8 &url)
{
	stream << url.Get();
	return stream;
}

//#################################################################################################
bool CUrl8::operator==(const CUrl8 &url) const
{	// Perform the easy checks first
	if(m_strScheme != url.m_strScheme ||
		m_domain != url.m_domain ||
		m_strUsername != url.m_strUsername ||
		m_strPassword != url.m_strPassword ||
		m_strFragment != url.m_strFragment ||
		m_nPort != url.m_nPort)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecPath.begin(), itr2 = url.m_vecPath.begin(); itr1 != m_vecPath.end() && itr2 != url.m_vecPath.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return false;
	}

	// We need to compare each parameter
	for(auto itr1 = m_vecParams.begin(), itr2 = url.m_vecParams.begin(); itr1 != m_vecParams.end() && itr2 != url.m_vecParams.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return false;
	}

	return true;
}

//#################################################################################################
bool CUrl8::operator!=(const CUrl8 &url) const
{	// Perform the easy checks first
	if(m_strScheme != url.m_strScheme ||
		m_domain != url.m_domain ||
		m_strUsername != url.m_strUsername ||
		m_strPassword != url.m_strPassword ||
		m_strFragment != url.m_strFragment ||
		m_nPort != url.m_nPort)
		return true;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecPath.begin(), itr2 = url.m_vecPath.begin(); itr1 != m_vecPath.end() && itr2 != url.m_vecPath.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return true;
	}

	// We need to compare each parameter
	for(auto itr1 = m_vecParams.begin(), itr2 = url.m_vecParams.begin(); itr1 != m_vecParams.end() && itr2 != url.m_vecParams.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return true;
	}

	return false;
}

//#################################################################################################
bool CUrl8::operator<(const CUrl8 &url) const
{
	return (Get().Compare(url.Get(), true) < 0);
}

//#################################################################################################
bool CUrl8::operator<=(const CUrl8 &url) const
{
	return (Get().Compare(url.Get(), true) <= 0);
}

//#################################################################################################
bool CUrl8::operator>(const CUrl8 &url) const
{
	return (Get().Compare(url.Get(), true) > 0);
}

//#################################################################################################
bool CUrl8::operator>=(const CUrl8 &url) const
{
	return (Get().Compare(url.Get(), true) >= 0);
}

//#################################################################################################
bool CUrl8::Compare(const CUrl8 &url, const bool bCaseInsensitive) const
{	// Perform the easy checks first
	if(m_strScheme.Compare(url.m_strScheme, bCaseInsensitive) != 0 ||
		m_domain.Compare(url.m_domain, bCaseInsensitive) != 0 ||
		m_strUsername.Compare(url.m_strUsername, bCaseInsensitive) != 0 ||
		m_strPassword.Compare(url.m_strPassword, bCaseInsensitive) != 0 ||
		m_strFragment.Compare(url.m_strFragment, bCaseInsensitive) != 0 ||
		m_nPort != url.m_nPort)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecPath.begin(), itr2 = url.m_vecPath.begin(); itr1 != m_vecPath.end() && itr2 != url.m_vecPath.end(); ++itr1, ++itr2)
	{
		if(itr1->Compare(*itr2, bCaseInsensitive) != 0)
			return false;
	}

	// We need to compare each parameter
	for(auto itr1 = m_vecParams.begin(), itr2 = url.m_vecParams.begin(); itr1 != m_vecParams.end() && itr2 != url.m_vecParams.end(); ++itr1, ++itr2)
	{
		if(itr1->Compare(*itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
void CUrl8::Empty(void)
{
	m_strScheme.Empty();
	m_strUsername.Empty();
	m_strPassword.Empty();
	m_domain.Empty();
	m_nPort = no_port;
	m_vecPath.clear();
	m_vecParams.clear();
	m_strFragment.Empty();
}

//#################################################################################################
bool CUrl8::IsEmpty(void) const noexcept
{
	return m_domain.IsEmpty();
}

//#################################################################################################
const CStr8 &CUrl8::GetScheme(void) const noexcept
{
	return m_strScheme;
}

//#################################################################################################
void CUrl8::SetScheme(const CStr8 &strScheme)
{
	if(!strScheme.IsEmpty())
	{
		m_strScheme.Assign(strScheme);
		UrlEscape8(m_strScheme);
	}
	else
		m_strScheme.Empty();
}

//#################################################################################################
const CStr8 &CUrl8::GetUsername(void) const noexcept
{
	return m_strUsername;
}

//#################################################################################################
void CUrl8::SetUsername(const CStr8 &strUsername)
{
	if(!strUsername.IsEmpty())
	{
		m_strUsername.Assign(strUsername);
		UrlEscape8(m_strUsername);
	}
	else
		m_strUsername.Empty();
}

//#################################################################################################
const CStr8 &CUrl8::GetPassword(void) const noexcept
{
	return m_strPassword;
}

//#################################################################################################
void CUrl8::SetPassword(const CStr8 &strPassword)
{
	if(!strPassword.IsEmpty())
	{
		m_strPassword.Assign(strPassword);
		UrlEscape8(m_strPassword);
	}
	else
		m_strPassword.Empty();
}

//#################################################################################################
const CDomainName8 &CUrl8::GetDomain(void) const noexcept
{
	return m_domain;
}

//#################################################################################################
void CUrl8::SetDomain(const CDomainName8 &domain)
{
	m_domain.Assign(domain);
}

//#################################################################################################
uint32_t CUrl8::GetPort(void) const noexcept
{
	return m_nPort;
}

//#################################################################################################
void CUrl8::SetPort(const uint32_t nPort)
{
	m_nPort = (nPort <= MAX_PORT) ? nPort : MAX_PORT;
}

//#################################################################################################
size_t CUrl8::GetSegmentCount(void) const noexcept
{
	return m_vecPath.size();
}

//#################################################################################################
CUrlPathSegment8 CUrl8::GetSegment(const size_t nSegment) const
{
	CUrlPathSegment8 path;

	if(nSegment < m_vecPath.size())
	{
		size_t nCount = nSegment;
		auto itr = m_vecPath.begin();
		while(nCount-- != 0)
			++itr;

		path.Assign(*itr);
	}

	return path;
}

//#################################################################################################
CUrlPathSegment8 CUrl8::GetFirstSegment(void) const
{
	return (!m_vecPath.empty()) ? m_vecPath.front() : CUrlPathSegment8();
}

//#################################################################################################
CUrlPathSegment8 CUrl8::GetLastSegment(void) const
{
	return (!m_vecPath.empty()) ? m_vecPath.back() : CUrlPathSegment8();
}

//#################################################################################################
CUrl8 &CUrl8::TrimLeftSegment(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecPath.size())
			m_vecPath.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecPath.erase(m_vecPath.begin());
		}
	}

	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::TrimRightSegment(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecPath.size())
			m_vecPath.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecPath.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::CropLeftSegment(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecPath.size())
	{
		if(nSegmentCount == 0)
			m_vecPath.clear();
		else
		{
			size_t nToRemove = m_vecPath.size() - nSegmentCount;
			while(nToRemove--)
				m_vecPath.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::CropRightSegment(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecPath.size())
	{
		if(nSegmentCount == 0)
			m_vecPath.clear();
		else
		{
			size_t nToRemove = m_vecPath.size() - nSegmentCount;
			while(nToRemove--)
				m_vecPath.erase(m_vecPath.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CUrl8::InsertSegment(const size_t nSegment, const CUrlPathSegment8 &path)
{
	if(!path.IsEmpty())
	{
		if(m_vecPath.empty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecPath.begin();
			while(itr != m_vecPath.end() && nCount-- != 0)
				++itr;

			m_vecPath.insert(itr, path);
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
CUrl8 &CUrl8::DeleteSegment(const size_t nSegment, const size_t nSegmentCount)
{
	if(nSegment < m_vecPath.size() && nSegmentCount != 0)
	{
		size_t nCount = nSegment;
		auto itr = m_vecPath.begin();
		while(itr != m_vecPath.end() && nCount-- != 0)
			++itr;

		nCount = nSegmentCount;
		while(itr != m_vecPath.end() && nCount-- != 0)
			itr = m_vecPath.erase(itr);
	}

	return *this;
}

//#################################################################################################
size_t CUrl8::RemoveSegment(const CUrlPathSegment8 &path, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecPath.begin();
	while(itr != m_vecPath.end())
	{
		if(itr->Compare(path, bCaseInsensitive) == 0)
		{
			itr = m_vecPath.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	return nCount;
}

//#################################################################################################
size_t CUrl8::ReplaceSegment(const CUrlPathSegment8 &pathOld, const CUrlPathSegment8 &pathNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!pathNew.IsEmpty())
	{
		for(auto &itr : m_vecPath)
		{
			if(itr.Compare(pathOld, bCaseInsensitive) == 0)
			{
				itr = pathNew;
				++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
CUrl8 &CUrl8::ModifySegment(const size_t nSegment, const CUrlPathSegment8 &path)
{
	if(nSegment < m_vecPath.size() && !path.IsEmpty())
	{
		size_t nCount = nSegment;
		auto itr = m_vecPath.begin();
		while(nCount-- != 0)
			++itr;

		*itr = path;
	}

	return *this;
}

//#################################################################################################
size_t CUrl8::GetParameterCount(void) const noexcept
{
	return m_vecParams.size();
}

//#################################################################################################
CUrlParameter8 CUrl8::GetParameter(const size_t nParam) const
{
	CUrlParameter8 param;

	if(nParam < m_vecParams.size())
	{
		size_t nCount = nParam;
		auto itr = m_vecParams.begin();
		while(nCount-- != 0)
			++itr;

		param.Assign(*itr);
	}

	return param;
}

//#################################################################################################
CUrlParameter8 CUrl8::GetFirstParameter(void) const
{
	return (!m_vecParams.empty()) ? m_vecParams.front() : CUrlParameter8();
}

//#################################################################################################
CUrlParameter8 CUrl8::GetLastParameter(void) const
{
	return (!m_vecParams.empty()) ? m_vecParams.back() : CUrlParameter8();
}

//#################################################################################################
CUrl8 &CUrl8::TrimLeftParameter(const size_t nParamCount)
{
	if(nParamCount)
	{
		if(nParamCount >= m_vecParams.size())
			m_vecParams.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nParamCount)
				m_vecParams.erase(m_vecParams.begin());
		}
	}

	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::TrimRightParameter(const size_t nParamCount)
{
	if(nParamCount)
	{
		if(nParamCount >= m_vecParams.size())
			m_vecParams.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nParamCount)
				m_vecParams.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::CropLeftParameter(const size_t nParamCount)
{
	if(nParamCount < m_vecParams.size())
	{
		if(nParamCount == 0)
			m_vecParams.clear();
		else
		{
			size_t nToRemove = m_vecParams.size() - nParamCount;
			while(nToRemove--)
				m_vecParams.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrl8 &CUrl8::CropRightParameter(const size_t nParamCount)
{
	if(nParamCount < m_vecParams.size())
	{
		if(nParamCount == 0)
			m_vecParams.clear();
		else
		{
			size_t nToRemove = m_vecParams.size() - nParamCount;
			while(nToRemove--)
				m_vecParams.erase(m_vecParams.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CUrl8::InsertParameter(const size_t nParam, const CUrlParameter8 &param)
{
	if(!param.IsEmpty())
	{
		if(m_vecParams.empty())
			Assign(param);
		else
		{
			size_t nCount = nParam;
			auto itr = m_vecParams.begin();
			while(itr != m_vecParams.end() && nCount-- != 0)
				++itr;

			m_vecParams.insert(itr, param);
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
CUrl8 &CUrl8::DeleteParameter(const size_t nParam, const size_t nParamCount)
{
	if(nParam < m_vecParams.size() && nParamCount != 0)
	{
		size_t nCount = nParam;
		auto itr = m_vecParams.begin();
		while(itr != m_vecParams.end() && nCount-- != 0)
			++itr;

		nCount = nParamCount;
		while(itr != m_vecParams.end() && nCount-- != 0)
			itr = m_vecParams.erase(itr);
	}

	return *this;
}

//#################################################################################################
size_t CUrl8::RemoveParameter(const CUrlParameter8 &param, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecParams.begin();
	while(itr != m_vecParams.end())
	{
		if(itr->Compare(param, bCaseInsensitive) == 0)
		{
			itr = m_vecParams.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	return nCount;
}

//#################################################################################################
size_t CUrl8::ReplaceParameter(const CUrlParameter8 &paramOld, const CUrlParameter8 &paramNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!paramNew.IsEmpty())
	{
		for(auto &itr : m_vecParams)
		{
			if(itr.Compare(paramOld, bCaseInsensitive) == 0)
			{
				itr = paramNew;
				++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
CUrl8 &CUrl8::ModifyParameter(const size_t nParam, const CUrlParameter8 &param)
{
	if(nParam < m_vecParams.size() && !param.IsEmpty())
	{
		size_t nCount = nParam;
		auto itr = m_vecParams.begin();
		while(nCount-- != 0)
			++itr;

		*itr = param;
	}

	return *this;
}

//#################################################################################################
bool CUrl8::HasFragment(void) const noexcept
{
	return !m_strFragment.IsEmpty();
}

//#################################################################################################
const CStr8 &CUrl8::GetFragment(void) const noexcept
{
	return m_strFragment;
}

//#################################################################################################
void CUrl8::SetFragment(const CStr8 &strFragment)
{
	if(!strFragment.IsEmpty())
	{
		m_strFragment.Assign(strFragment);
		UrlEscape8(m_strFragment);
	}
	else
		m_strFragment.Empty();
}

//#################################################################################################
CStr8 CUrl8::GetPath(const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{
	CStr8 strPath;

	if(!m_vecPath.empty())
	{
		auto itr = m_vecPath.begin();
		strPath.Assign(itr->Get());

		while(++itr != m_vecPath.end())
		{
			strPath.Append(g_chForwardSlash8);
			strPath.Append(itr->Get());
		}
	}

	if(bIncludeParams && !m_vecParams.empty())
	{
		strPath.Append(g_chQuestionMark8);

		auto itr = m_vecParams.begin();
		strPath.Append(itr->Get(bAlwaysIncludeEqual));

		while(++itr != m_vecParams.end())
		{
			strPath.Append(g_chAmpersand8);
			strPath.Append(itr->Get(bAlwaysIncludeEqual));
		}
	}

	if(bIncludeFragment && !m_strFragment.IsEmpty())
		strPath.Append(m_strFragment);

	return strPath;
}

//#################################################################################################
ERRCODE CUrl8::SetPath(const CStr8 &strPath)
{
	m_vecPath.clear();

	return AppendPath(strPath);
}

//#################################################################################################
ERRCODE CUrl8::AppendPath(const CStr8 &strPath)
{
	ERRCODE nErrorCode;

	size_t nPos = strPath.FindFirstOf(g_szPathSeparators8);
	if(nPos == CStr8::not_found)
		nErrorCode = ParsePath(strPath);
	else
		nErrorCode = ParsePath(strPath.GetLeft(nPos));

	return nErrorCode;
}

//#################################################################################################
CStr8 CUrl8::GetParameters(const bool bAlwaysIncludeEqual) const
{
	CStr8 strParams;

	if(!m_vecParams.empty())
	{
		auto itr = m_vecParams.begin();
		strParams.Append(itr->Get(bAlwaysIncludeEqual));

		while(++itr != m_vecParams.end())
		{
			strParams.Append(g_chAmpersand8);
			strParams.Append(itr->Get(bAlwaysIncludeEqual));
		}
	}

	return strParams;
}

//#################################################################################################
ERRCODE CUrl8::SetParameters(const CStr8 &strParams)
{
	m_vecParams.clear();

	return AppendParameters(strParams);
}

//#################################################################################################
ERRCODE CUrl8::AppendParameters(const CStr8 &strParams)
{
	ERRCODE nErrorCode;

	size_t nPos = strParams.Find(g_chHash8);
	if(nPos == CStr8::not_found)
		nErrorCode = ParseParams(strParams);
	else
		nErrorCode = ParseParams(strParams.GetLeft(nPos));

	return nErrorCode;
}

//#################################################################################################
void CUrl8::DebugPrint(const bool bAppendNewline) const
{
#ifdef _WIN32
	CStr8 str(Get());
	if(bAppendNewline)
		str.Append(g_szEol8);
	OutputDebugStringA(str);
#else
	std::cout << Get();
	if(bAppendNewline)
		std::cout << std::endl;
#endif
}

//#################################################################################################
ERRCODE CUrl8::ParseUrl(const CStr8 &strUrl)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Empty();

	if(strUrl.IsEmpty())
		return nErrorCode;

	size_t nStartPos = 0;
	size_t nEndPos;

	if((nEndPos = strUrl.Find(g_szSchemeSeparator8)) != CStr8::not_found)
	{
		if(nEndPos)
		{
			m_strScheme.Assign(strUrl, 0, nEndPos);
			UrlEscape8(m_strScheme);
		}
		nStartPos = nEndPos + g_nSchemeSeparatorLen;
	}

	CStr8 strDomain;
	if((nEndPos = strUrl.FindFirstOf(g_szDomainSeparators8, nStartPos)) == CStr8::not_found)
	{
		nErrorCode = ParseDomain(strUrl.GetMid(nStartPos));
		if(nErrorCode != FW_NO_ERROR)
			Empty();
	}
	else
	{
		nErrorCode = ParseDomain(strUrl.GetMid(nStartPos, nEndPos - nStartPos));
		if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_EMPTY)
		{
			nStartPos = nEndPos + 1;
			nEndPos = strUrl.FindFirstOf(g_szPathSeparators8, nStartPos);

			if(nEndPos == CStr8::not_found)
			{
				nErrorCode = ParsePath(strUrl.GetMid(nStartPos));
				if(nErrorCode != FW_NO_ERROR && nErrorCode != FW_ERROR_EMPTY)
					Empty();
			}
			else
			{
				nErrorCode = ParsePath(strUrl.GetMid(nStartPos, nEndPos - nStartPos));
				if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_EMPTY)
				{
					if(strUrl[nEndPos] == g_chQuestionMark8)
					{
						nStartPos = nEndPos + 1;
						nEndPos = strUrl.Find(g_chHash8, nStartPos);
						if(nEndPos == CStr8::not_found)
						{
							nErrorCode = ParseParams(strUrl.GetMid(nStartPos));
							if(nErrorCode != FW_NO_ERROR && nErrorCode != FW_ERROR_EMPTY)
								Empty();
						}
						else
						{
							nErrorCode = ParseParams(strUrl.GetMid(nStartPos, nEndPos - nStartPos));
							if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_EMPTY)
							{
								m_strFragment.Assign(strUrl, nEndPos + 1);
								UrlEscape8(m_strFragment);
							}
							else
								Empty();
						}
					}
					else if(strUrl[nEndPos] == g_chHash8)
					{
						m_strFragment.Assign(strUrl, nEndPos + 1);
						UrlEscape8(m_strFragment);
					}
				}
				else
					Empty();
			}
		}
		else
			Empty();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrl8::ParseDomain(const CStr8 &strDomain)
{
	ERRCODE nErrorCode;

	if(!strDomain.IsEmpty())
	{
		size_t nStartPos = 0;
		size_t nEndPos;
		size_t nColonPos;

		nEndPos = strDomain.Find(g_chAt8);
		if(nEndPos != CStr8::not_found)
		{
			nColonPos = strDomain.Find(g_chColon8);
			if(nColonPos != CStr8::not_found && nColonPos < nEndPos)
			{
				m_strUsername.Assign(strDomain, 0, nColonPos);
				UrlEscape8(m_strUsername);
				m_strPassword.Assign(strDomain, nColonPos + 1, nEndPos - nColonPos - 1);
				UrlEscape8(m_strPassword);
			}
			else
			{
				m_strUsername.Assign(strDomain, 0, nEndPos);
				UrlEscape8(m_strUsername);
			}
			nStartPos = nEndPos + 1;
		}

		nColonPos = strDomain.Find(g_chColon8, nStartPos);
		if(nColonPos != CStr8::not_found)
		{
			nErrorCode = m_domain.Assign(strDomain.GetMid(nStartPos, nColonPos - nStartPos));

			CStr8 strPort(strDomain, nColonPos + 1, CStr8::end);
			if(IsDigitString8(strPort))
			{
				m_nPort = StringToInteger8<uint32_t>(strPort);
				if(m_nPort > MAX_PORT)
					m_nPort = no_port;
			}
		}
		else
			nErrorCode = m_domain.Assign(strDomain.GetMid(nStartPos));
	}
	else
		nErrorCode = FW_ERROR_EMPTY;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrl8::ParsePath(const CStr8 &strPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!strPath.IsEmpty())
	{
		size_t nStart = 0;
		CStr8 strToken;

		strToken.Assign(strPath.Tokenize(g_chForwardSlash8, nStart));
		while(nStart != CStr8::not_found)
		{
			CUrlPathSegment8 path(strToken);
			if(path.IsEmpty())
			{
				nErrorCode = FW_ERROR_INVALID_DATA;
				break;
			}

			m_vecPath.push_back(std::move(path));

			strToken.Assign(strPath.Tokenize(g_chForwardSlash8, nStart));
		}
	}
	else
		nErrorCode = FW_ERROR_EMPTY;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrl8::ParseParams(const CStr8 &strParams)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!strParams.IsEmpty())
	{
		size_t nStart = 0;
		CStr8 strToken;
		CStr8 strName;
		CStr8 strValue;

		strToken.Assign(strParams.Tokenize(g_chAmpersand8, nStart));
		while(nStart != CStr8::not_found)
		{
			size_t nPos = strToken.Find(g_chEqual8);
			if(nPos == CStr8::not_found)
			{
				strName.Assign(strToken);
				strValue.Empty();
			}
			else
			{
				strName.Assign(strToken, 0, nPos);
				strValue.Assign(strToken, nPos + 1);
			}

			CUrlParameter8 param(strName, strValue, true);
			if(param.IsEmpty())
			{
				nErrorCode = FW_ERROR_INVALID_DATA;
				break;
			}

			m_vecParams.push_back(std::move(param));

			strToken.Assign(strParams.Tokenize(g_chAmpersand8, nStart));
		}
	}
	else
		nErrorCode = FW_ERROR_EMPTY;

	return nErrorCode;
}

//#################################################################################################
bool CUrl8::SplitUrl(PCSTR szUrl, CStr8 &strScheme, CStr8 &strUsername, CStr8 &strPassword, CStr8 &strDomain, CStr8 &strPort, CStr8 &strPath, CStr8 &strParams, CStr8 &strFragment, const bool bAlwaysIncludeEqual)
{
	bool bValid = false;

	strScheme.Empty();
	strUsername.Empty();
	strPassword.Empty();
	strDomain.Empty();
	strPort.Empty();
	strPath.Empty();
	strParams.Empty();
	strFragment.Empty();

	CUrl8 url;
	if(url.Assign(szUrl) == FW_NO_ERROR)
	{
		strScheme.Assign(url.m_strScheme);
		strUsername.Assign(url.m_strUsername);
		strPassword.Assign(url.m_strPassword);
		strDomain.Assign(url.m_domain);
		if(url.m_nPort != no_port)
			strPort.Assign(IntegerToString8(url.m_nPort));

		if(!url.m_vecPath.empty())
		{
			auto itr = url.m_vecPath.begin();
			strPath.Assign(itr->Get());

			while(++itr != url.m_vecPath.end())
			{
				strPath.Append(g_chForwardSlash8);
				strPath.Append(itr->Get());
			}
		}

		if(!url.m_vecParams.empty())
		{
			auto itr = url.m_vecParams.begin();
			strParams.Assign(itr->Get(bAlwaysIncludeEqual));

			while(++itr != url.m_vecParams.end())
			{
				strParams.Append(g_chAmpersand8);
				strParams.Append(itr->Get(bAlwaysIncludeEqual));
			}
		}

		strFragment.Assign(url.m_strFragment);
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CUrl8::IsValidUrl(const CStr8 &strUrl)
{
	bool bValid = true;

	CUrl8 url;
	if(url.Assign(strUrl) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CUrl8::IsValidUrl(PCSTR szUrl)
{
	bool bValid = true;

	CUrl8 url;
	if(url.Assign(szUrl) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}




/////////////////////////////////////////////////
// CUrlW
/////////////////////////////////////////////////

//#################################################################################################
CUrlW::CUrlW(void)
	: m_nPort(no_port)
{
}

//#################################################################################################
CUrlW::CUrlW(const CUrlW &src)
	: m_nPort(no_port)
{
	Assign(src);
}

//#################################################################################################
CUrlW::CUrlW(CUrlW &&src) noexcept
	: m_strScheme(std::move(src.m_strScheme)),
	m_strUsername(std::move(src.m_strUsername)),
	m_strPassword(std::move(src.m_strPassword)),
	m_domain(std::move(src.m_domain)),
	m_nPort(src.m_nPort),
	m_vecPath(std::move(src.m_vecPath)),
	m_vecParams(std::move(src.m_vecParams)),
	m_strFragment(std::move(src.m_strFragment))
{
	src.m_nPort = no_port;
}

//#################################################################################################
CUrlW::CUrlW(const CUrl8 &url)
	: m_nPort(no_port)
{
	Assign(url);
}

//#################################################################################################
CUrlW::CUrlW(const CDomainNameW &domain)
	: m_domain(domain),
	m_nPort(no_port)
{
}

//#################################################################################################
CUrlW::CUrlW(const CDomainName8 &domain)
	: m_domain(domain),
	m_nPort(no_port)
{
}

//#################################################################################################
CUrlW::CUrlW(const CStrW &str)
	: m_nPort(no_port)
{
	ParseUrl(str);
}

//#################################################################################################
CUrlW::CUrlW(const CStr8 &str)
	: m_nPort(no_port)
{
	ParseUrl(str.AsWide());
}

//#################################################################################################
CUrlW::CUrlW(PCWSTR sz)
	: m_nPort(no_port)
{
	ParseUrl(CStrW(sz));
}

//#################################################################################################
CUrlW::CUrlW(PCSTR sz)
	: m_nPort(no_port)
{
	ParseUrl(CStrW(sz));
}

//#################################################################################################
CUrlW::operator CStrW(void) const
{
	return Get();
}

//#################################################################################################
CStrW CUrlW::Get(const bool bIncludeScheme, const bool bIncludeUsername, const bool bIncludePassword, const bool bIncludePort, const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{	// Convert everything into a single string
	CStrW str;

	if(!IsEmpty())
	{	// Scheme
		if(bIncludeScheme && !m_strScheme.IsEmpty())
		{
			str.Append(m_strScheme);
			str.Append(g_szSchemeSeparatorW);
		}

		// Username and password
		if(bIncludeUsername && !m_strUsername.IsEmpty())
		{
			str.Append(m_strUsername);

			if(bIncludePassword && !m_strPassword.IsEmpty())
			{
				str.Append(g_chColonW);
				str.Append(m_strPassword);
			}

			str.Append(g_chAtW);
		}

		// Domain
		str.Append(m_domain);

		// Port
		if(bIncludePort && m_nPort != no_port)
		{
			str.Append(g_chColonW);
			str.Append(IntegerToStringW(m_nPort));
		}

		if(!m_vecPath.empty() || (bIncludeParams && !m_vecParams.empty()) || (bIncludeFragment && !m_strFragment.IsEmpty()))
			str.Append(g_chForwardSlashW);

		// Path
		if(!m_vecPath.empty())
		{
			auto itr = m_vecPath.begin();
			str.Append(itr->Get());

			while(++itr != m_vecPath.end())
			{
				str.Append(g_chForwardSlashW);
				str.Append(itr->Get());
			}
		}

		// Parameters
		if(bIncludeParams && !m_vecParams.empty())
		{
			str.Append(g_chQuestionMarkW);

			auto itr = m_vecParams.begin();
			str.Append(itr->Get(bAlwaysIncludeEqual));

			while(++itr != m_vecParams.end())
			{
				str.Append(g_chAmpersandW);
				str.Append(itr->Get(bAlwaysIncludeEqual));
			}
		}

		// Fragment
		if(bIncludeFragment && !m_strFragment.IsEmpty())
		{
			str.Append(g_chHashW);
			str.Append(m_strFragment);
		}
	}

	return str;
}

//#################################################################################################
size_t CUrlW::GetLength(const bool bIncludeNullTerm, const bool bIncludeScheme, const bool bIncludeUsername, const bool bIncludePassword, const bool bIncludePort, const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{	// Length in characters
	size_t nStrLen = 0;

	if(!IsEmpty())
	{	// Scheme
		if(bIncludeScheme && !m_strScheme.IsEmpty())
			nStrLen += m_strScheme.GetLength() + g_nSchemeSeparatorLen;

		// Username and password
		if(bIncludeUsername && !m_strUsername.IsEmpty())
		{
			nStrLen += m_strUsername.GetLength() + 1;	// Add one for the at sign

			if(bIncludePassword && !m_strPassword.IsEmpty())
				nStrLen += m_strPassword.GetLength() + 1;	// Add one for the colon
		}

		// Domain
		nStrLen += m_domain.GetLength();

		// Port
		if(bIncludePort && m_nPort != no_port)
			nStrLen += IntegerToStringW(m_nPort).GetLength() + 1;	// Add one for the colon

		if(!m_vecPath.empty() || (bIncludeParams && !m_vecParams.empty()) || (bIncludeFragment && !m_strFragment.IsEmpty()))
			++nStrLen;	// Add one for the backslash

		// Path
		if(!m_vecPath.empty())
		{
			auto itr = m_vecPath.begin();
			nStrLen += itr->GetLength();

			while(++itr != m_vecPath.end())
				nStrLen += itr->GetLength() + 1;	// Add one for the backslash
		}

		// Parameters
		if(bIncludeParams && !m_vecParams.empty())
		{
			auto itr = m_vecParams.begin();
			nStrLen += itr->Get(bAlwaysIncludeEqual).GetLength() + 1;	// Add one for the question mark

			while(++itr != m_vecParams.end())
				nStrLen += itr->Get(bAlwaysIncludeEqual).GetLength() + 1;	// Add one for the ampersand
		}

		// Fragment
		if(bIncludeFragment && !m_strFragment.IsEmpty())
			nStrLen += m_strFragment.GetLength() + 1;	// Add one for the hash

		if(bIncludeNullTerm)
			++nStrLen;
	}

	return nStrLen;
}

//#################################################################################################
size_t CUrlW::GetSize(const bool bIncludeNullTerm, const bool bIncludeScheme, const bool bIncludeUsername, const bool bIncludePassword, const bool bIncludePort, const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{	// Size in bytes
	return GetLength(bIncludeNullTerm, bIncludeScheme, bIncludeUsername, bIncludePassword, bIncludePort, bIncludeParams, bAlwaysIncludeEqual, bIncludeFragment) * sizeof(wchar_t);
}

//#################################################################################################
CUrlW &CUrlW::operator=(const CUrlW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(CUrlW &&src) noexcept
{
	if(this != &src)
	{
		m_strScheme = std::move(src.m_strScheme);
		m_strUsername = std::move(src.m_strUsername);
		m_strPassword = std::move(src.m_strPassword);
		m_domain = std::move(src.m_domain);
		m_nPort = src.m_nPort;
		m_vecPath = std::move(src.m_vecPath);
		m_vecParams = std::move(src.m_vecParams);
		m_strFragment = std::move(src.m_strFragment);

		src.m_nPort = no_port;
	}

	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(const CUrl8 &url)
{
	Assign(url);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(const CDomainNameW &domain)
{
	Empty();
	m_domain.Assign(domain);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(const CDomainName8 &domain)
{
	Empty();
	m_domain.Assign(domain);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(const CStrW &str)
{
	ParseUrl(str);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(const CStr8 &str)
{
	ParseUrl(str.AsWide());
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(PCWSTR sz)
{
	ParseUrl(CStrW(sz));
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator=(PCSTR sz)
{
	ParseUrl(CStrW(sz));
	return *this;
}

//#################################################################################################
ERRCODE CUrlW::Assign(const CUrlW &url)
{
	if(this != &url)
	{
		Empty();

		m_strScheme.Assign(url.m_strScheme);
		m_strUsername.Assign(url.m_strUsername);
		m_strPassword.Assign(url.m_strPassword);
		m_domain.Assign(url.m_domain);
		m_nPort = url.m_nPort;

		// Add segments to the path
		for(const auto &segment : url.m_vecPath)
			m_vecPath.push_back(segment);

		// Add parameters to the path
		for(const auto &param : url.m_vecParams)
			m_vecParams.push_back(param);

		m_strFragment.Assign(url.m_strFragment);
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrlW::Assign(const CUrl8 &url)
{
	Empty();

	m_strScheme.Assign(url.m_strScheme);
	m_strUsername.Assign(url.m_strUsername);
	m_strPassword.Assign(url.m_strPassword);
	m_domain.Assign(url.m_domain);
	m_nPort = url.m_nPort;

	// Add segments to the path
	for(const auto &segment : url.m_vecPath)
		m_vecPath.emplace_back(segment);

	// Add parameters to the path
	for(const auto &param : url.m_vecParams)
		m_vecParams.emplace_back(param);

	m_strFragment.Assign(url.m_strFragment);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrlW::Assign(const CDomainNameW &domain)
{
	Empty();
	m_domain.Assign(domain);
	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrlW::Assign(const CDomainName8 &domain)
{
	Empty();
	m_domain.Assign(domain);
	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrlW::Assign(const CStrW &str)
{
	return ParseUrl(str);
}

//#################################################################################################
ERRCODE CUrlW::Assign(const CStr8 &str)
{
	return ParseUrl(str.AsWide());
}

//#################################################################################################
ERRCODE CUrlW::Assign(PCWSTR sz)
{
	return ParseUrl(CStrW(sz));
}

//#################################################################################################
ERRCODE CUrlW::Assign(PCSTR sz)
{
	return ParseUrl(CStrW(sz));
}

//#################################################################################################
CUrlW &CUrlW::operator+=(const CDomainNameW &domain)
{
	Append(domain);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator+=(const CDomainName8 &domain)
{
	Append(domain);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator+=(const CUrlPathSegmentW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator+=(const CUrlPathSegment8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator+=(const CUrlParameterW &param)
{
	Append(param);
	return *this;
}

//#################################################################################################
CUrlW &CUrlW::operator+=(const CUrlParameter8 &param)
{
	Append(param);
	return *this;
}

//#################################################################################################
ERRCODE CUrlW::Append(const CDomainNameW &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsEmpty())
		m_domain.Assign(domain);
	else
		nErrorCode = FW_ERROR_INVALID_DATA;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlW::Append(const CDomainName8 &domain)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsEmpty())
		m_domain.Assign(domain);
	else
		nErrorCode = FW_ERROR_INVALID_DATA;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlW::Append(const CUrlPathSegmentW &path)
{
	if(!path.IsEmpty())
		m_vecPath.push_back(path);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrlW::Append(const CUrlPathSegment8 &path)
{
	if(!path.IsEmpty())
		m_vecPath.emplace_back(path);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrlW::Append(const CUrlParameterW &param)
{
	if(!param.IsEmpty())
		m_vecParams.push_back(param);

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CUrlW::Append(const CUrlParameter8 &param)
{
	if(!param.IsEmpty())
		m_vecParams.emplace_back(param);

	return FW_NO_ERROR;
}

#ifdef _WIN32
//#################################################################################################
CUrlW operator+(const CUrlW &url, const CUrlPathSegmentW &path)
{
	CUrlW urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CUrlW &url, const CUrlPathSegment8 &path)
{
	CUrlW urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CUrl8 &url, const CUrlPathSegmentW &path)
{
	CUrlW urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CUrl8 &url, const CUrlPathSegment8 &path)
{
	CUrlW urlRtn(url);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CUrlW &url, const CUrlParameterW &param)
{
	CUrlW urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CUrlW &url, const CUrlParameter8 &param)
{
	CUrlW urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CUrl8 &url, const CUrlParameterW &param)
{
	CUrlW urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CUrl8 &url, const CUrlParameter8 &param)
{
	CUrlW urlRtn(url);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainNameW &domain, const CUrlPathSegmentW &path)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainNameW &domain, const CUrlPathSegment8 &path)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainName8 &domain, const CUrlPathSegmentW &path)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainName8 &domain, const CUrlPathSegment8 &path)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(path);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainNameW &domain, const CUrlParameterW &param)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainNameW &domain, const CUrlParameter8 &param)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainName8 &domain, const CUrlParameterW &param)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}

//#################################################################################################
CUrlW operator+(const CDomainName8 &domain, const CUrlParameter8 &param)
{
	CUrlW urlRtn(domain);
	urlRtn.Append(param);
	return urlRtn;
}
#endif

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CUrlW &url)
{
	stream << url.Get();
	return stream;
}

//#################################################################################################
bool CUrlW::operator==(const CUrlW &url) const
{	// Perform the easy checks first
	if(m_strScheme != url.m_strScheme ||
		m_domain != url.m_domain ||
		m_strUsername != url.m_strUsername ||
		m_strPassword != url.m_strPassword ||
		m_strFragment != url.m_strFragment ||
		m_nPort != url.m_nPort)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecPath.begin(), itr2 = url.m_vecPath.begin(); itr1 != m_vecPath.end() && itr2 != url.m_vecPath.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return false;
	}

	// We need to compare each parameter
	for(auto itr1 = m_vecParams.begin(), itr2 = url.m_vecParams.begin(); itr1 != m_vecParams.end() && itr2 != url.m_vecParams.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return false;
	}

	return true;
}

//#################################################################################################
bool CUrlW::operator!=(const CUrlW &url) const
{	// Perform the easy checks first
	if(m_strScheme != url.m_strScheme ||
		m_domain != url.m_domain ||
		m_strUsername != url.m_strUsername ||
		m_strPassword != url.m_strPassword ||
		m_strFragment != url.m_strFragment ||
		m_nPort != url.m_nPort)
		return true;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecPath.begin(), itr2 = url.m_vecPath.begin(); itr1 != m_vecPath.end() && itr2 != url.m_vecPath.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return true;
	}

	// We need to compare each parameter
	for(auto itr1 = m_vecParams.begin(), itr2 = url.m_vecParams.begin(); itr1 != m_vecParams.end() && itr2 != url.m_vecParams.end(); ++itr1, ++itr2)
	{
		if(*itr1 != *itr2)
			return true;
	}

	return false;
}

//#################################################################################################
bool CUrlW::operator<(const CUrlW &url) const
{
	return (Get().Compare(url.Get(), true) < 0);
}

//#################################################################################################
bool CUrlW::operator<=(const CUrlW &url) const
{
	return (Get().Compare(url.Get(), true) <= 0);
}

//#################################################################################################
bool CUrlW::operator>(const CUrlW &url) const
{
	return (Get().Compare(url.Get(), true) > 0);
}

//#################################################################################################
bool CUrlW::operator>=(const CUrlW &url) const
{
	return (Get().Compare(url.Get(), true) >= 0);
}

//#################################################################################################
bool CUrlW::Compare(const CUrlW &url, const bool bCaseInsensitive) const
{	// Perform the easy checks first
	if(m_strScheme.Compare(url.m_strScheme, bCaseInsensitive) != 0 ||
		m_domain.Compare(url.m_domain, bCaseInsensitive) != 0 ||
		m_strUsername.Compare(url.m_strUsername, bCaseInsensitive) != 0 ||
		m_strPassword.Compare(url.m_strPassword, bCaseInsensitive) != 0 ||
		m_strFragment.Compare(url.m_strFragment, bCaseInsensitive) != 0 ||
		m_nPort != url.m_nPort)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecPath.begin(), itr2 = url.m_vecPath.begin(); itr1 != m_vecPath.end() && itr2 != url.m_vecPath.end(); ++itr1, ++itr2)
	{
		if(itr1->Compare(*itr2, bCaseInsensitive) != 0)
			return false;
	}

	// We need to compare each parameter
	for(auto itr1 = m_vecParams.begin(), itr2 = url.m_vecParams.begin(); itr1 != m_vecParams.end() && itr2 != url.m_vecParams.end(); ++itr1, ++itr2)
	{
		if(itr1->Compare(*itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
void CUrlW::Empty(void)
{
	m_strScheme.Empty();
	m_strUsername.Empty();
	m_strPassword.Empty();
	m_domain.Empty();
	m_nPort = no_port;
	m_vecPath.clear();
	m_vecParams.clear();
	m_strFragment.Empty();
}

//#################################################################################################
bool CUrlW::IsEmpty(void) const noexcept
{
	return m_domain.IsEmpty();
}

//#################################################################################################
const CStrW &CUrlW::GetScheme(void) const noexcept
{
	return m_strScheme;
}

//#################################################################################################
void CUrlW::SetScheme(const CStrW &strScheme)
{
	if(!strScheme.IsEmpty())
	{
		m_strScheme.Assign(strScheme);
		UrlEscapeW(m_strScheme);
	}
	else
		m_strScheme.Empty();
}

//#################################################################################################
const CStrW &CUrlW::GetUsername(void) const noexcept
{
	return m_strUsername;
}

//#################################################################################################
void CUrlW::SetUsername(const CStrW &strUsername)
{
	if(!strUsername.IsEmpty())
	{
		m_strUsername.Assign(strUsername);
		UrlEscapeW(m_strUsername);
	}
	else
		m_strUsername.Empty();
}

//#################################################################################################
const CStrW &CUrlW::GetPassword(void) const noexcept
{
	return m_strPassword;
}

//#################################################################################################
void CUrlW::SetPassword(const CStrW &strPassword)
{
	if(!strPassword.IsEmpty())
	{
		m_strPassword.Assign(strPassword);
		UrlEscapeW(m_strPassword);
	}
	else
		m_strPassword.Empty();
}

//#################################################################################################
const CDomainNameW &CUrlW::GetDomain(void) const noexcept
{
	return m_domain;
}

//#################################################################################################
void CUrlW::SetDomain(const CDomainNameW &domain)
{
	m_domain.Assign(domain);
}

//#################################################################################################
uint32_t CUrlW::GetPort(void) const noexcept
{
	return m_nPort;
}

//#################################################################################################
void CUrlW::SetPort(const uint32_t nPort)
{
	m_nPort = (nPort <= MAX_PORT) ? nPort : MAX_PORT;
}

//#################################################################################################
size_t CUrlW::GetSegmentCount(void) const noexcept
{
	return m_vecPath.size();
}

//#################################################################################################
CUrlPathSegmentW CUrlW::GetSegment(const size_t nSegment) const
{
	CUrlPathSegmentW path;

	if(nSegment < m_vecPath.size())
	{
		size_t nCount = nSegment;
		auto itr = m_vecPath.begin();
		while(nCount-- != 0)
			++itr;

		path.Assign(*itr);
	}

	return path;
}

//#################################################################################################
CUrlPathSegmentW CUrlW::GetFirstSegment(void) const
{
	return (!m_vecPath.empty()) ? m_vecPath.front() : CUrlPathSegmentW();
}

//#################################################################################################
CUrlPathSegmentW CUrlW::GetLastSegment(void) const
{
	return (!m_vecPath.empty()) ? m_vecPath.back() : CUrlPathSegmentW();
}

//#################################################################################################
CUrlW &CUrlW::TrimLeftSegment(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecPath.size())
			m_vecPath.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecPath.erase(m_vecPath.begin());
		}
	}

	return *this;
}

//#################################################################################################
CUrlW &CUrlW::TrimRightSegment(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecPath.size())
			m_vecPath.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecPath.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrlW &CUrlW::CropLeftSegment(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecPath.size())
	{
		if(nSegmentCount == 0)
			m_vecPath.clear();
		else
		{
			size_t nToRemove = m_vecPath.size() - nSegmentCount;
			while(nToRemove--)
				m_vecPath.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrlW &CUrlW::CropRightSegment(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecPath.size())
	{
		if(nSegmentCount == 0)
			m_vecPath.clear();
		else
		{
			size_t nToRemove = m_vecPath.size() - nSegmentCount;
			while(nToRemove--)
				m_vecPath.erase(m_vecPath.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CUrlW::InsertSegment(const size_t nSegment, const CUrlPathSegmentW &path)
{
	if(!path.IsEmpty())
	{
		if(m_vecPath.empty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecPath.begin();
			while(itr != m_vecPath.end() && nCount-- != 0)
				++itr;

			m_vecPath.insert(itr, path);
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
CUrlW &CUrlW::DeleteSegment(const size_t nSegment, const size_t nSegmentCount)
{
	if(nSegment < m_vecPath.size() && nSegmentCount != 0)
	{
		size_t nCount = nSegment;
		auto itr = m_vecPath.begin();
		while(itr != m_vecPath.end() && nCount-- != 0)
			++itr;

		nCount = nSegmentCount;
		while(itr != m_vecPath.end() && nCount-- != 0)
			itr = m_vecPath.erase(itr);
	}

	return *this;
}

//#################################################################################################
size_t CUrlW::RemoveSegment(const CUrlPathSegmentW &path, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecPath.begin();
	while(itr != m_vecPath.end())
	{
		if(itr->Compare(path, bCaseInsensitive) == 0)
		{
			itr = m_vecPath.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	return nCount;
}

//#################################################################################################
size_t CUrlW::ReplaceSegment(const CUrlPathSegmentW &pathOld, const CUrlPathSegmentW &pathNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!pathNew.IsEmpty())
	{
		for(auto &itr : m_vecPath)
		{
			if(itr.Compare(pathOld, bCaseInsensitive) == 0)
			{
				itr = pathNew;
				++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
CUrlW &CUrlW::ModifySegment(const size_t nSegment, const CUrlPathSegmentW &path)
{
	if(nSegment < m_vecPath.size() && !path.IsEmpty())
	{
		size_t nCount = nSegment;
		auto itr = m_vecPath.begin();
		while(nCount-- != 0)
			++itr;

		*itr = path;
	}

	return *this;
}

//#################################################################################################
size_t CUrlW::GetParameterCount(void) const noexcept
{
	return m_vecParams.size();
}

//#################################################################################################
CUrlParameterW CUrlW::GetParameter(const size_t nParam) const
{
	CUrlParameterW param;

	if(nParam < m_vecParams.size())
	{
		size_t nCount = nParam;
		auto itr = m_vecParams.begin();
		while(nCount-- != 0)
			++itr;

		param.Assign(*itr);
	}

	return param;
}

//#################################################################################################
CUrlParameterW CUrlW::GetFirstParameter(void) const
{
	return (!m_vecParams.empty()) ? m_vecParams.front() : CUrlParameterW();
}

//#################################################################################################
CUrlParameterW CUrlW::GetLastParameter(void) const
{
	return (!m_vecParams.empty()) ? m_vecParams.back() : CUrlParameterW();
}

//#################################################################################################
CUrlW &CUrlW::TrimLeftParameter(const size_t nParamCount)
{
	if(nParamCount)
	{
		if(nParamCount >= m_vecParams.size())
			m_vecParams.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nParamCount)
				m_vecParams.erase(m_vecParams.begin());
		}
	}

	return *this;
}

//#################################################################################################
CUrlW &CUrlW::TrimRightParameter(const size_t nParamCount)
{
	if(nParamCount)
	{
		if(nParamCount >= m_vecParams.size())
			m_vecParams.clear();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nParamCount)
				m_vecParams.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrlW &CUrlW::CropLeftParameter(const size_t nParamCount)
{
	if(nParamCount < m_vecParams.size())
	{
		if(nParamCount == 0)
			m_vecParams.clear();
		else
		{
			size_t nToRemove = m_vecParams.size() - nParamCount;
			while(nToRemove--)
				m_vecParams.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CUrlW &CUrlW::CropRightParameter(const size_t nParamCount)
{
	if(nParamCount < m_vecParams.size())
	{
		if(nParamCount == 0)
			m_vecParams.clear();
		else
		{
			size_t nToRemove = m_vecParams.size() - nParamCount;
			while(nToRemove--)
				m_vecParams.erase(m_vecParams.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CUrlW::InsertParameter(const size_t nParam, const CUrlParameterW &param)
{
	if(!param.IsEmpty())
	{
		if(m_vecParams.empty())
			Assign(param);
		else
		{
			size_t nCount = nParam;
			auto itr = m_vecParams.begin();
			while(itr != m_vecParams.end() && nCount-- != 0)
				++itr;

			m_vecParams.insert(itr, param);
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
CUrlW &CUrlW::DeleteParameter(const size_t nParam, const size_t nParamCount)
{
	if(nParam < m_vecParams.size() && nParamCount != 0)
	{
		size_t nCount = nParam;
		auto itr = m_vecParams.begin();
		while(itr != m_vecParams.end() && nCount-- != 0)
			++itr;

		nCount = nParamCount;
		while(itr != m_vecParams.end() && nCount-- != 0)
			itr = m_vecParams.erase(itr);
	}

	return *this;
}

//#################################################################################################
size_t CUrlW::RemoveParameter(const CUrlParameterW &param, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecParams.begin();
	while(itr != m_vecParams.end())
	{
		if(itr->Compare(param, bCaseInsensitive) == 0)
		{
			itr = m_vecParams.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	return nCount;
}

//#################################################################################################
size_t CUrlW::ReplaceParameter(const CUrlParameterW &paramOld, const CUrlParameterW &paramNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!paramNew.IsEmpty())
	{
		for(auto &itr : m_vecParams)
		{
			if(itr.Compare(paramOld, bCaseInsensitive) == 0)
			{
				itr = paramNew;
				++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
CUrlW &CUrlW::ModifyParameter(const size_t nParam, const CUrlParameterW &param)
{
	if(nParam < m_vecParams.size() && !param.IsEmpty())
	{
		size_t nCount = nParam;
		auto itr = m_vecParams.begin();
		while(nCount-- != 0)
			++itr;

		*itr = param;
	}

	return *this;
}

//#################################################################################################
bool CUrlW::HasFragment(void) const noexcept
{
	return !m_strFragment.IsEmpty();
}

//#################################################################################################
const CStrW &CUrlW::GetFragment(void) const noexcept
{
	return m_strFragment;
}

//#################################################################################################
void CUrlW::SetFragment(const CStrW &strFragment)
{
	if(!strFragment.IsEmpty())
	{
		m_strFragment.Assign(strFragment);
		UrlEscapeW(m_strFragment);
	}
	else
		m_strFragment.Empty();
}

//#################################################################################################
CStrW CUrlW::GetPath(const bool bIncludeParams, const bool bAlwaysIncludeEqual, const bool bIncludeFragment) const
{
	CStrW strPath;

	if(!m_vecPath.empty())
	{
		auto itr = m_vecPath.begin();
		strPath.Assign(itr->Get());

		while(++itr != m_vecPath.end())
		{
			strPath.Append(g_chForwardSlashW);
			strPath.Append(itr->Get());
		}
	}

	if(bIncludeParams && !m_vecParams.empty())
	{
		strPath.Append(g_chQuestionMarkW);

		auto itr = m_vecParams.begin();
		strPath.Append(itr->Get(bAlwaysIncludeEqual));

		while(++itr != m_vecParams.end())
		{
			strPath.Append(g_chAmpersandW);
			strPath.Append(itr->Get(bAlwaysIncludeEqual));
		}
	}

	if(bIncludeFragment && !m_strFragment.IsEmpty())
		strPath.Append(m_strFragment);

	return strPath;
}

//#################################################################################################
ERRCODE CUrlW::SetPath(const CStrW &strPath)
{
	m_vecPath.clear();

	return AppendPath(strPath);
}

//#################################################################################################
ERRCODE CUrlW::AppendPath(const CStrW &strPath)
{
	ERRCODE nErrorCode;

	size_t nPos = strPath.FindFirstOf(g_szPathSeparatorsW);
	if(nPos == CStrW::not_found)
		nErrorCode = ParsePath(strPath);
	else
		nErrorCode = ParsePath(strPath.GetLeft(nPos));

	return nErrorCode;
}

//#################################################################################################
CStrW CUrlW::GetParameters(const bool bAlwaysIncludeEqual) const
{
	CStrW strParams;

	if(!m_vecParams.empty())
	{
		auto itr = m_vecParams.begin();
		strParams.Append(itr->Get(bAlwaysIncludeEqual));

		while(++itr != m_vecParams.end())
		{
			strParams.Append(g_chAmpersandW);
			strParams.Append(itr->Get(bAlwaysIncludeEqual));
		}
	}

	return strParams;
}

//#################################################################################################
ERRCODE CUrlW::SetParameters(const CStrW &strParams)
{
	m_vecParams.clear();

	return AppendParameters(strParams);
}

//#################################################################################################
ERRCODE CUrlW::AppendParameters(const CStrW &strParams)
{
	ERRCODE nErrorCode;

	size_t nPos = strParams.Find(g_chHashW);
	if(nPos == CStrW::not_found)
		nErrorCode = ParseParams(strParams);
	else
		nErrorCode = ParseParams(strParams.GetLeft(nPos));

	return nErrorCode;
}

//#################################################################################################
void CUrlW::DebugPrint(const bool bAppendNewline) const
{
#ifdef _WIN32
	CStrW str(Get());
	if(bAppendNewline)
		str.Append(g_szEolW);
	OutputDebugStringW(str);
#else
	CStr8 str(Get());
	std::cout << str;
	if(bAppendNewline)
		std::cout << std::endl;
#endif
}

//#################################################################################################
ERRCODE CUrlW::ParseUrl(const CStrW &strUrl)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Empty();

	if(strUrl.IsEmpty())
		return nErrorCode;

	size_t nStartPos = 0;
	size_t nEndPos;

	if((nEndPos = strUrl.Find(g_szSchemeSeparatorW)) != CStrW::not_found)
	{
		if(nEndPos)
		{
			m_strScheme.Assign(strUrl, 0, nEndPos);
			UrlEscapeW(m_strScheme);
		}
		nStartPos = nEndPos + g_nSchemeSeparatorLen;
	}

	CStrW strDomain;
	if((nEndPos = strUrl.FindFirstOf(g_szDomainSeparatorsW, nStartPos)) == CStrW::not_found)
	{
		nErrorCode = ParseDomain(strUrl.GetMid(nStartPos));
		if(nErrorCode != FW_NO_ERROR)
			Empty();
	}
	else
	{
		nErrorCode = ParseDomain(strUrl.GetMid(nStartPos, nEndPos - nStartPos));
		if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_EMPTY)
		{
			nStartPos = nEndPos + 1;
			nEndPos = strUrl.FindFirstOf(g_szPathSeparatorsW, nStartPos);

			if(nEndPos == CStrW::not_found)
			{
				nErrorCode = ParsePath(strUrl.GetMid(nStartPos));
				if(nErrorCode != FW_NO_ERROR && nErrorCode != FW_ERROR_EMPTY)
					Empty();
			}
			else
			{
				nErrorCode = ParsePath(strUrl.GetMid(nStartPos, nEndPos - nStartPos));
				if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_EMPTY)
				{
					if(strUrl[nEndPos] == g_chQuestionMarkW)
					{
						nStartPos = nEndPos + 1;
						nEndPos = strUrl.Find(g_chHashW, nStartPos);
						if(nEndPos == CStrW::not_found)
						{
							nErrorCode = ParseParams(strUrl.GetMid(nStartPos));
							if(nErrorCode != FW_NO_ERROR && nErrorCode != FW_ERROR_EMPTY)
								Empty();
						}
						else
						{
							nErrorCode = ParseParams(strUrl.GetMid(nStartPos, nEndPos - nStartPos));
							if(nErrorCode == FW_NO_ERROR || nErrorCode == FW_ERROR_EMPTY)
							{
								m_strFragment.Assign(strUrl, nEndPos + 1);
								UrlEscapeW(m_strFragment);
							}
							else
								Empty();
						}
					}
					else if(strUrl[nEndPos] == g_chHashW)
					{
						m_strFragment.Assign(strUrl, nEndPos + 1);
						UrlEscapeW(m_strFragment);
					}
				}
				else
					Empty();
			}
		}
		else
			Empty();
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlW::ParseDomain(const CStrW &strDomain)
{
	ERRCODE nErrorCode;

	if(!strDomain.IsEmpty())
	{
		size_t nStartPos = 0;
		size_t nEndPos;
		size_t nColonPos;

		nEndPos = strDomain.Find(g_chAtW);
		if(nEndPos != CStrW::not_found)
		{
			nColonPos = strDomain.Find(g_chColonW);
			if(nColonPos != CStrW::not_found && nColonPos < nEndPos)
			{
				m_strUsername.Assign(strDomain, 0, nColonPos);
				UrlEscapeW(m_strUsername);
				m_strPassword.Assign(strDomain, nColonPos + 1, nEndPos - nColonPos - 1);
				UrlEscapeW(m_strPassword);
			}
			else
			{
				m_strUsername.Assign(strDomain, 0, nEndPos);
				UrlEscapeW(m_strUsername);
			}
			nStartPos = nEndPos + 1;
		}

		nColonPos = strDomain.Find(g_chColonW, nStartPos);
		if(nColonPos != CStrW::not_found)
		{
			nErrorCode = m_domain.Assign(strDomain.GetMid(nStartPos, nColonPos - nStartPos));

			CStrW strPort(strDomain, nColonPos + 1, CStrW::end);
			if(IsDigitStringW(strPort))
			{
				m_nPort = StringToIntegerW<uint32_t>(strPort);
				if(m_nPort > MAX_PORT)
					m_nPort = no_port;
			}
		}
		else
			nErrorCode = m_domain.Assign(strDomain.GetMid(nStartPos));
	}
	else
		nErrorCode = FW_ERROR_EMPTY;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlW::ParsePath(const CStrW &strPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!strPath.IsEmpty())
	{
		size_t nStart = 0;
		CStrW strToken;

		strToken.Assign(strPath.Tokenize(g_chForwardSlashW, nStart));
		while(nStart != CStrW::not_found)
		{
			CUrlPathSegmentW path(strToken);
			if(path.IsEmpty())
			{
				nErrorCode = FW_ERROR_INVALID_DATA;
				break;
			}

			m_vecPath.push_back(std::move(path));

			strToken.Assign(strPath.Tokenize(g_chForwardSlashW, nStart));
		}
	}
	else
		nErrorCode = FW_ERROR_EMPTY;

	return nErrorCode;
}

//#################################################################################################
ERRCODE CUrlW::ParseParams(const CStrW &strParams)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!strParams.IsEmpty())
	{
		size_t nStart = 0;
		CStrW strToken;
		CStrW strName;
		CStrW strValue;

		strToken.Assign(strParams.Tokenize(g_chAmpersandW, nStart));
		while(nStart != CStrW::not_found)
		{
			size_t nPos = strToken.Find(g_chEqualW);
			if(nPos == CStrW::not_found)
			{
				strName.Assign(strToken);
				strValue.Empty();
			}
			else
			{
				strName.Assign(strToken, 0, nPos);
				strValue.Assign(strToken, nPos + 1);
			}

			CUrlParameterW param(strName, strValue, true);
			if(param.IsEmpty())
			{
				nErrorCode = FW_ERROR_INVALID_DATA;
				break;
			}

			m_vecParams.push_back(std::move(param));

			strToken.Assign(strParams.Tokenize(g_chAmpersandW, nStart));
		}
	}
	else
		nErrorCode = FW_ERROR_EMPTY;

	return nErrorCode;
}

//#################################################################################################
bool CUrlW::SplitUrl(PCWSTR szUrl, CStrW &strScheme, CStrW &strUsername, CStrW &strPassword, CStrW &strDomain, CStrW &strPort, CStrW &strPath, CStrW &strParams, CStrW &strFragment, const bool bAlwaysIncludeEqual)
{
	bool bValid = false;

	strScheme.Empty();
	strUsername.Empty();
	strPassword.Empty();
	strDomain.Empty();
	strPort.Empty();
	strPath.Empty();
	strParams.Empty();
	strFragment.Empty();

	CUrlW url;
	if(url.Assign(szUrl) == FW_NO_ERROR)
	{
		strScheme.Assign(url.m_strScheme);
		strUsername.Assign(url.m_strUsername);
		strPassword.Assign(url.m_strPassword);
		strDomain.Assign(url.m_domain);
		if(url.m_nPort != no_port)
			strPort.Assign(IntegerToStringW(url.m_nPort));

		if(!url.m_vecPath.empty())
		{
			auto itr = url.m_vecPath.begin();
			strPath.Assign(itr->Get());

			while(++itr != url.m_vecPath.end())
			{
				strPath.Append(g_chForwardSlashW);
				strPath.Append(itr->Get());
			}
		}

		if(!url.m_vecParams.empty())
		{
			auto itr = url.m_vecParams.begin();
			strParams.Assign(itr->Get(bAlwaysIncludeEqual));

			while(++itr != url.m_vecParams.end())
			{
				strParams.Append(g_chAmpersandW);
				strParams.Append(itr->Get(bAlwaysIncludeEqual));
			}
		}

		strFragment.Assign(url.m_strFragment);
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CUrlW::IsValidUrl(const CStrW &strUrl)
{
	bool bValid = true;

	CUrlW url;
	if(url.Assign(strUrl) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CUrlW::IsValidUrl(PCWSTR szUrl)
{
	bool bValid = true;

	CUrlW url;
	if(url.Assign(szUrl) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

NS_END

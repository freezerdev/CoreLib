#include "Base.h"
#include "FilePath.h"

NS_BEGIN

#ifdef _WIN32
static constexpr char g_chColon8 = ':';
static constexpr wchar_t g_chColonW = L':';
static constexpr char g_chDelimiter8 = '\\';
static constexpr wchar_t g_chDelimiterW = L'\\';
static PCSTR g_szDoubleDelimiter8 = "\\\\";
static PCWSTR g_szDoubleDelimiterW = L"\\\\";
#else
static constexpr char g_chDelimiter8 = '/';
static constexpr wchar_t g_chDelimiterW = L'/';
static PCSTR g_szDoubleDelimiter8 = "//";
static PCWSTR g_szDoubleDelimiterW = L"//";
#endif

static constexpr char g_chPeriod8 = '.';
static constexpr wchar_t g_chPeriodW = L'.';
static constexpr char g_chAsterisk8 = '*';
static constexpr wchar_t g_chAsteriskW = L'*';
static constexpr char g_chQuestionMark8 = '?';
static constexpr wchar_t g_chQuestionMarkW = L'?';
static constexpr char g_chHyphen8 = '-';
static constexpr wchar_t g_chHyphenW = L'-';

static PCSTR g_szDoublePeriod8 = "..";
static PCWSTR g_szDoublePeriodW = L"..";
static PCSTR g_szValidMachineName8 = "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static PCWSTR g_szValidMachineNameW = L"-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static constexpr size_t g_nValidMachineNameLen = 64;

#ifdef _WIN32
static PCSTR g_szInvalidSegment8 = "\"/<>\\|";
static PCWSTR g_szInvalidSegmentW = L"\"/<>\\|";
static constexpr size_t g_nInvalidSegmentLen = 6;
static PCSTR g_szKernelPrefix8 = "\\\\?\\";
static PCWSTR g_szKernelPrefixW = L"\\\\?\\";
static constexpr size_t g_nKernelPrefixLen = 4;
static PCSTR g_szUNCKernelPrefix8 = "\\\\?\\UNC\\";
static PCWSTR g_szUNCKernelPrefixW = L"\\\\?\\UNC\\";
static constexpr size_t g_nUNCKernelPrefixLen = 8;
static PCSTR g_szUNCKernelPartialPrefix8 = "?\\UNC\\";
static PCWSTR g_szUNCKernelPartialPrefixW = L"?\\UNC\\";
#endif

const size_t CFilePathSegment8::not_found = (size_t)-1;
const size_t CFilePathSegment8::end = (size_t)-1;
const size_t CFilePathSegmentW::not_found = (size_t)-1;
const size_t CFilePathSegmentW::end = (size_t)-1;

#ifdef _WIN32
// The upper limit is 260 for files but 247 for folders
static constexpr auto INTERNAL_MAX_PATH = 247;
#endif


/////////////////////////////////////////////////
// CFilePathSegment8
/////////////////////////////////////////////////

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(void)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(const CFilePathSegment8 &src)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(CFilePathSegment8 &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PSTR)g_szNull8;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(const CFilePathSegmentW &path)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(path);
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(const CStr8 &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(const CStrW &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(PCSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(PCWSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(const char ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CFilePathSegment8::CFilePathSegment8(const wchar_t ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CFilePathSegment8::~CFilePathSegment8(void)
{
	Dealloc();
}

//#################################################################################################
CFilePathSegment8::operator PCSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCSTR CFilePathSegment8::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CFilePathSegment8::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CFilePathSegment8::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
void CFilePathSegment8::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CFilePathSegment8::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
bool CFilePathSegment8::IsDrive(void) const noexcept
{
#ifdef _WIN32
	return (m_nStrLen == 2 && m_sz[1] == g_chColon8);
#else
	return false;
#endif
}

//#################################################################################################
bool CFilePathSegment8::HasWildcard(void) const
{
	bool bWildcard = false;

	PCSTR szScan = m_sz;
	while(*szScan != g_chNull8)
	{
		if(*szScan == g_chAsterisk8 || *szScan == g_chQuestionMark8)
		{
			bWildcard = true;
			break;
		}

		++szScan;
	}

	return bWildcard;
}

//#################################################################################################
CFilePathSegment8 CFilePathSegment8::AsUtf8(void) const
{
	return *this;
}

//#################################################################################################
CFilePathSegmentW CFilePathSegment8::AsWide(void) const
{
	return CFilePathSegmentW(*this);
}

#ifdef _WIN32
//#################################################################################################
CFilePathSegmentW CFilePathSegment8::AsNative(void) const
{
	return CFilePathSegmentW(*this);
}
#else
//#################################################################################################
CFilePathSegment8 CFilePathSegment8::AsNative(void) const
{
	return *this;
}
#endif

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(const CFilePathSegment8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(CFilePathSegment8 &&src) noexcept
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
CFilePathSegment8 &CFilePathSegment8::operator=(const CFilePathSegmentW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CFilePathSegment8 &CFilePathSegment8::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CFilePathSegment8::Assign(const CFilePathSegment8 &path)
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
ERRCODE CFilePathSegment8::Assign(const CFilePathSegmentW &path)
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
ERRCODE CFilePathSegment8::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidSegment(str))
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
ERRCODE CFilePathSegment8::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CFilePathSegmentW::IsValidSegment(str))
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
ERRCODE CFilePathSegment8::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidSegment(sz))
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
ERRCODE CFilePathSegment8::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CFilePathSegmentW::IsValidSegment(sz))
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
ERRCODE CFilePathSegment8::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidSegment(ch))
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
ERRCODE CFilePathSegment8::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CFilePathSegmentW::IsValidSegment(ch))
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
std::ostream &operator<<(std::ostream &stream, const CFilePathSegment8 &path)
{
	stream << path.m_sz;
	return stream;
}

//#################################################################################################
bool CFilePathSegment8::operator==(const CFilePathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegment8::operator==(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegment8::operator==(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegment8::operator==(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegment8::operator!=(const CFilePathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegment8::operator!=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegment8::operator!=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegment8::operator!=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<(const CFilePathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<=(const CFilePathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegment8::operator<=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>(const CFilePathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>=(const CFilePathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePathSegment8::operator>=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
int CFilePathSegment8::Compare(const CFilePathSegment8 &path, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegment8::Compare(const CStr8 &str, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegment8::Compare(PCSTR sz, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegment8::Compare(const char ch, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegment8::CompareBase(const CStr8 &strBase, const bool bCaseInsensitive) const
{
	int nCompareRtn;

	size_t nThisLen = ReverseFind(m_sz, m_nStrLen, g_chPeriod8);
	if(nThisLen == not_found)
		nThisLen = m_nStrLen;

	size_t nThatLen = strBase.GetLength();
	if(strBase.GetLast() == g_chPeriod8)
		--nThatLen;

	nCompareRtn = StringCompare8(m_sz, nThisLen, strBase, nThatLen, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CFilePathSegment8::CompareBase(PCSTR szBase, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nThisLen = ReverseFind(m_sz, m_nStrLen, g_chPeriod8);
	if(nThisLen == not_found)
		nThisLen = m_nStrLen;

	size_t nStrLen = StringGetLength(szBase);
	if(nStrLen)
	{
		size_t nThatLen = nStrLen;
		if(szBase[nStrLen - 1] == g_chPeriod8)
			--nThatLen;

		nCompareRtn = StringCompare8(m_sz, nThisLen, szBase, nThatLen, bCaseInsensitive);
	}
	else if(nThisLen)
		nCompareRtn = 1;

	return nCompareRtn;
}

//#################################################################################################
int CFilePathSegment8::CompareExtension(const CStr8 &strExt, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nThisStart = ReverseFind(m_sz, m_nStrLen, g_chPeriod8);
	if(nThisStart != not_found)
	{
		size_t nThisLen = m_nStrLen - ++nThisStart;
		size_t nThatStart = 0;

		if(strExt.GetFirst() == g_chPeriod8)
			++nThatStart;
		size_t nThatLen = strExt.GetLength() - nThatStart;

		nCompareRtn = StringCompare8(m_sz + nThisStart, nThisLen, (PCSTR)strExt + nThatStart, nThatLen, bCaseInsensitive);
	}
	else if(!strExt.IsEmpty())
		nCompareRtn = -1;

	return nCompareRtn;
}

//#################################################################################################
int CFilePathSegment8::CompareExtension(PCSTR szExt, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = StringGetLength(szExt);

	size_t nThisStart = ReverseFind(m_sz, m_nStrLen, g_chPeriod8);
	if(nThisStart != not_found)
	{
		size_t nThisLen = m_nStrLen - ++nThisStart;

		if(nStrLen)
		{
			size_t nThatStart = 0;
			if(szExt[0] == g_chPeriod8)
				++nThatStart;
			size_t nThatLen = nStrLen - nThatStart;

			nCompareRtn = StringCompare8(m_sz + nThisStart, nThisLen, szExt + nThatStart, nThatLen, bCaseInsensitive);
		}
		else if(nThisLen)
			nCompareRtn = 1;
	}
	else if(nStrLen)
		nCompareRtn = -1;

	return nCompareRtn;
}

//#################################################################################################
CStr8 CFilePathSegment8::GetBase(void) const
{
	CStr8 str;

	if(m_nStrLen)
	{
		str.Assign(m_sz);

		// Remove the extension if there is one
		size_t nPos = str.ReverseFind(g_chPeriod8);
		if(nPos != CStr8::not_found)
			str.Delete(nPos, CStr8::end);
	}

	return str;
}

//#################################################################################################
CStr8 CFilePathSegment8::GetExtension(void) const
{
	CStr8 str;

	if(m_nStrLen)
	{
		size_t nPos = ReverseFind(m_sz, m_nStrLen, g_chPeriod8, m_nStrLen);
		if(nPos != not_found)
			str.Assign(m_sz + nPos + 1);
	}

	return str;
}

//#################################################################################################
bool CFilePathSegment8::SetBase(const CStr8 &strBase)
{
	bool bSet = false;

	size_t nExtLen = 0;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriod8, m_nStrLen);
	if(nExtPos != not_found)
		nExtLen = m_nStrLen - nExtPos;

	size_t nNewLen = strBase.GetLength() + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<char[]>(nNewLen + 1);
		if(!strBase.IsEmpty())
			std::memcpy(szTemp.get(), strBase, strBase.GetLength());
		if(nExtLen)
			std::memcpy(szTemp.get() + strBase.GetLength(), m_sz + nExtPos, nExtLen);
		szTemp[nNewLen] = g_chNull8;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegment8::SetBase(PCSTR szBase)
{
	bool bSet = false;

	size_t nExtLen = 0;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriod8, m_nStrLen);
	if(nExtPos != not_found)
		nExtLen = m_nStrLen - nExtPos;

	size_t nBaseLen = StringGetLength(szBase);
	size_t nNewLen = nBaseLen + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<char[]>(nNewLen + 1);
		if(nBaseLen)
			std::memcpy(szTemp.get(), szBase, nBaseLen);
		if(nExtLen)
			std::memcpy(szTemp.get() + nBaseLen, m_sz + nExtPos, nExtLen);
		szTemp[nNewLen] = g_chNull8;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegment8::SetExtension(const CStr8 &strExt)
{
	bool bSet = false;

	size_t nBaseLen = m_nStrLen;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriod8, m_nStrLen);
	if(nExtPos != not_found)
		nBaseLen = nExtPos;

	size_t nExtLen = strExt.GetLength();
	if(nExtLen && strExt[0] != g_chPeriod8)
		++nExtLen;

	size_t nNewLen = nBaseLen + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<char[]>(nNewLen + 1);
		if(nBaseLen)
			std::memcpy(szTemp.get(), m_sz, nBaseLen);
		if(nExtLen)
		{
			if(strExt[0] != g_chPeriod8)
			{
				szTemp[nBaseLen] = g_chPeriod8;
				std::memcpy(szTemp.get() + nBaseLen + 1, strExt, strExt.GetLength());
			}
			else
				std::memcpy(szTemp.get() + nBaseLen, strExt, strExt.GetLength());
		}
		szTemp[nNewLen] = g_chNull8;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegment8::SetExtension(PCSTR szExt)
{
	bool bSet = false;

	size_t nBaseLen = m_nStrLen;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriod8, m_nStrLen);
	if(nExtPos != not_found)
		nBaseLen = nExtPos;

	size_t nExtLen = StringGetLength(szExt);
	if(nExtLen && szExt[0] != g_chPeriod8)
		++nExtLen;

	size_t nNewLen = nBaseLen + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<char[]>(nNewLen + 1);
		if(nBaseLen)
			std::memcpy(szTemp.get(), m_sz, nBaseLen);
		if(nExtLen)
		{
			if(szExt[0] != g_chPeriod8)
			{
				szTemp[nBaseLen] = g_chPeriod8;
				std::memcpy(szTemp.get() + nBaseLen + 1, szExt, nExtLen - 1);
			}
			else
				std::memcpy(szTemp.get() + nBaseLen, szExt, nExtLen);
		}
		szTemp[nNewLen] = g_chNull8;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegment8::AddExtension(const CStr8 &strExt)
{
	bool bSet = false;

	size_t nExtLen = strExt.GetLength();
	if(nExtLen)
	{
		if(strExt[0] != g_chPeriod8)
			++nExtLen;

		size_t nNewLen = m_nStrLen + nExtLen;
		auto szTemp = std::make_unique<char[]>(nNewLen + 1);
		if(m_nStrLen)
			std::memcpy(szTemp.get(), m_sz, m_nStrLen);
		if(strExt[0] != g_chPeriod8)
		{
			szTemp[m_nStrLen] = g_chPeriod8;
			std::memcpy(szTemp.get() + m_nStrLen + 1, strExt, strExt.GetLength());
		}
		else
			std::memcpy(szTemp.get() + m_nStrLen, strExt, strExt.GetLength());
		szTemp[nNewLen] = g_chNull8;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegment8::AddExtension(PCSTR szExt)
{
	bool bSet = false;

	size_t nExtLen = StringGetLength(szExt);
	if(nExtLen)
	{
		if(szExt[0] != g_chPeriod8)
			++nExtLen;

		size_t nNewLen = m_nStrLen + nExtLen;
		auto szTemp = std::make_unique<char[]>(nNewLen + 1);
		if(m_nStrLen)
			std::memcpy(szTemp.get(), m_sz, m_nStrLen);
		if(szExt[0] != g_chPeriod8)
		{
			szTemp[m_nStrLen] = g_chPeriod8;
			std::memcpy(szTemp.get() + m_nStrLen + 1, szExt, nExtLen - 1);
		}
		else
			std::memcpy(szTemp.get() + m_nStrLen, szExt, nExtLen);
		szTemp[nNewLen] = g_chNull8;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
void CFilePathSegment8::RemoveExtension(void)
{
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriod8, m_nStrLen);
	if(nExtPos != not_found)
	{
		m_sz[nExtPos] = g_chNull8;
		m_nStrLen = nExtPos;
	}
}

//#################################################################################################
bool CFilePathSegment8::IsValidSegment(const CStr8 &strSegment)
{	// Valid Windows path segments are of length 1 through MAX_PATH,
	// and can contain any character except the following:
	//   " \ / | * ? < >
	// It can only contain a ':' if it is X:
	// Note: Allow * and ? as they are wildcards.
	// Valid Linux/MacOS path segments are of length 1 through MAX_PATH,
	// and can contain any character except '/'

	// Length between 1 and MAX_PATH
	size_t nStrLen = strSegment.GetLength();
	if(nStrLen == 0 || nStrLen > MAX_PATH)
		return false;

#ifdef _WIN32
	// Does it contain an invalid character or a colon out of place?
	if(FindFirstOf(strSegment, nStrLen, g_szInvalidSegment8, g_nInvalidSegmentLen) || (std::memchr(strSegment, g_chColon8, nStrLen) && (nStrLen != 2 || strSegment[0] == g_chColon8)))
		return false;
#else
	// Does it contain an invalid character?
	if(std::memchr(strSegment, g_chDelimiter8, nStrLen))
		return false;
#endif

	return true;
}

//#################################################################################################
bool CFilePathSegment8::IsValidSegment(PCSTR szSegment)
{	// Valid Windows path segments are of length 1 through MAX_PATH,
	// and can contain any character except the following:
	//   " \ / | * ? < >
	// It can only contain a ':' if it is X:
	// Note: Allow * and ? as they are wildcards.
	// Valid Linux/MacOS path segments are of length 1 through MAX_PATH,
	// and can contain any character except '/'

	// Cannot be nullptr
	if(szSegment == nullptr)
		return false;

	// Length between 1 and MAX_PATH
	size_t nStrLen = StringGetLength(szSegment);
	if(nStrLen == 0 || nStrLen > MAX_PATH)
		return false;

#ifdef _WIN32
	// Does it contain an invalid character or a colon out of place
	if(FindFirstOf(szSegment, nStrLen, g_szInvalidSegment8, g_nInvalidSegmentLen) || (std::memchr(szSegment, g_chColon8, nStrLen) && (nStrLen != 2 || *szSegment == g_chColon8)))
		return false;
#else
	// Does it contain an invalid character?
	if(std::memchr(szSegment, g_chDelimiter8, nStrLen))
		return false;
#endif

	return true;
}

//#################################################################################################
bool CFilePathSegment8::IsValidSegment(const char chSegment)
{	// Valid Windows path segments are of length 1 through MAX_PATH,
	// and can contain any character except the following:
	//   " \ / | * ? < >
	// It can only contain a ':' if it is X:
	// Note: Allow * and ? as they are wildcards.
	// Valid Linux/MacOS path segments are of length 1 through MAX_PATH,
	// and can contain any character except '/'

#ifdef _WIN32
	// Cannot be nullptr, ':', or an invalid character
	if(chSegment == g_chNull8 || chSegment == g_chColon8 || FindFirstOf(&chSegment, 1, g_szInvalidSegment8, g_nInvalidSegmentLen))
		return false;
#else
	// Cannot be nullptr or '/'
	if(chSegment == g_chNull8 || chSegment == g_chDelimiter8)
		return false;
#endif

	return true;
}

//#################################################################################################
bool CFilePathSegment8::Alloc(const size_t nBufLen)
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
void CFilePathSegment8::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PSTR)g_szNull8;
	m_nStrLen = m_nBufLen = 0;
}

//#################################################################################################
PCSTR CFilePathSegment8::FindFirstOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	PCSTR szResult = nullptr;
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if((szResult = (PCSTR)std::memchr(szFind, sz[nIndex], nFindLen)) != nullptr)
			return szResult;
	}

	return szResult;
}

//#################################################################################################
size_t CFilePathSegment8::ReverseFind(PCSTR sz, const size_t nStrLen, const char chFind, const size_t nStart)
{
	if(nStrLen && nStart)
	{
		size_t nIndex;
		if(nStart >= nStrLen)
			nIndex = nStrLen - 1;
		else
			nIndex = nStart - 1;

		do{
			if(sz[nIndex] == chFind)
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}




/////////////////////////////////////////////////
// CFilePathSegmentW
/////////////////////////////////////////////////

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(void)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(const CFilePathSegmentW &src)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(CFilePathSegmentW &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PWSTR)g_szNullW;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(const CFilePathSegment8 &path)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(path);
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(const CStrW &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(const CStr8 &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(PCWSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(PCSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(const wchar_t ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CFilePathSegmentW::CFilePathSegmentW(const char ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CFilePathSegmentW::~CFilePathSegmentW(void)
{
	Dealloc();
}

//#################################################################################################
CFilePathSegmentW::operator PCWSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCWSTR CFilePathSegmentW::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CFilePathSegmentW::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CFilePathSegmentW::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen) * sizeof(wchar_t);
}

//#################################################################################################
void CFilePathSegmentW::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CFilePathSegmentW::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
bool CFilePathSegmentW::IsDrive(void) const noexcept
{
#ifdef _WIN32
	return (m_nStrLen == 2 && m_sz[1] == g_chColonW);
#else
	return false;
#endif
}

//#################################################################################################
bool CFilePathSegmentW::HasWildcard(void) const
{
	bool bWildcard = false;

	PCWSTR szScan = m_sz;
	while(*szScan != g_chNullW)
	{
		if(*szScan == g_chAsteriskW || *szScan == g_chQuestionMarkW)
		{
			bWildcard = true;
			break;
		}

		++szScan;
	}

	return bWildcard;
}

//#################################################################################################
CFilePathSegment8 CFilePathSegmentW::AsUtf8(void) const
{
	return CFilePathSegment8(*this);
}

//#################################################################################################
CFilePathSegmentW CFilePathSegmentW::AsWide(void) const
{
	return *this;
}

#ifdef _WIN32
//#################################################################################################
CFilePathSegmentW CFilePathSegmentW::AsNative(void) const
{
	return *this;
}
#else
//#################################################################################################
CFilePathSegment8 CFilePathSegmentW::AsNative(void) const
{
	return CFilePathSegment8(*this);
}
#endif

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(const CFilePathSegmentW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(CFilePathSegmentW &&src) noexcept
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
CFilePathSegmentW &CFilePathSegmentW::operator=(const CFilePathSegment8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CFilePathSegmentW &CFilePathSegmentW::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CFilePathSegmentW::Assign(const CFilePathSegmentW &path)
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
ERRCODE CFilePathSegmentW::Assign(const CFilePathSegment8 &path)
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
ERRCODE CFilePathSegmentW::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidSegment(str))
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
ERRCODE CFilePathSegmentW::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CFilePathSegment8::IsValidSegment(str))
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
ERRCODE CFilePathSegmentW::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidSegment(sz))
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
ERRCODE CFilePathSegmentW::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CFilePathSegment8::IsValidSegment(sz))
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
ERRCODE CFilePathSegmentW::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidSegment(ch))
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
ERRCODE CFilePathSegmentW::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CFilePathSegment8::IsValidSegment(ch))
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
std::wostream &operator<<(std::wostream &stream, const CFilePathSegmentW &path)
{
	stream << path.m_sz;
	return stream;
}

//#################################################################################################
bool CFilePathSegmentW::operator==(const CFilePathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator==(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator==(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator==(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) == 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator!=(const CFilePathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator!=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator!=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator!=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) != 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<(const CFilePathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<=(const CFilePathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator<=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>(const CFilePathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>=(const CFilePathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePathSegmentW::operator>=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
int CFilePathSegmentW::Compare(const CFilePathSegmentW &path, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegmentW::Compare(const CStrW &str, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegmentW::Compare(PCWSTR sz, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegmentW::Compare(const wchar_t ch, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
int CFilePathSegmentW::CompareBase(const CStrW &strBase, const bool bCaseInsensitive) const
{
	int nCompareRtn;

	size_t nThisLen = ReverseFind(m_sz, m_nStrLen, g_chPeriodW);
	if(nThisLen == not_found)
		nThisLen = m_nStrLen;

	size_t nThatLen = strBase.GetLength();
	if(strBase.GetLast() == g_chPeriodW)
		--nThatLen;

	nCompareRtn = StringCompareW(m_sz, nThisLen, strBase, nThatLen, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CFilePathSegmentW::CompareBase(PCWSTR szBase, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nThisLen = ReverseFind(m_sz, m_nStrLen, g_chPeriodW);
	if(nThisLen == not_found)
		nThisLen = m_nStrLen;

	size_t nStrLen = StringGetLength(szBase);
	if(nStrLen)
	{
		size_t nThatLen = nStrLen;
		if(szBase[nStrLen - 1] == g_chPeriodW)
			--nThatLen;

		nCompareRtn = StringCompareW(m_sz, nThisLen, szBase, nThatLen, bCaseInsensitive);
	}
	else if(nThisLen)
		nCompareRtn = 1;

	return nCompareRtn;
}

//#################################################################################################
int CFilePathSegmentW::CompareExtension(const CStrW &strExt, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nThisStart = ReverseFind(m_sz, m_nStrLen, g_chPeriodW);
	if(nThisStart != not_found)
	{
		size_t nThisLen = m_nStrLen - ++nThisStart;
		size_t nThatStart = 0;

		if(strExt.GetFirst() == g_chPeriodW)
			++nThatStart;
		size_t nThatLen = strExt.GetLength() - nThatStart;

		nCompareRtn = StringCompareW(m_sz + nThisStart, nThisLen, (PCWSTR)strExt + nThatStart, nThatLen, bCaseInsensitive);
	}
	else if(!strExt.IsEmpty())
		nCompareRtn = -1;

	return nCompareRtn;
}

//#################################################################################################
int CFilePathSegmentW::CompareExtension(PCWSTR szExt, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = StringGetLength(szExt);

	size_t nThisStart = ReverseFind(m_sz, m_nStrLen, g_chPeriodW);
	if(nThisStart != not_found)
	{
		size_t nThisLen = m_nStrLen - ++nThisStart;

		if(nStrLen)
		{
			size_t nThatStart = 0;
			if(szExt[0] == g_chPeriodW)
				++nThatStart;
			size_t nThatLen = nStrLen - nThatStart;

			nCompareRtn = StringCompareW(m_sz + nThisStart, nThisLen, szExt + nThatStart, nThatLen, bCaseInsensitive);
		}
		else if(nThisLen)
			nCompareRtn = 1;
	}
	else if(nStrLen)
		nCompareRtn = -1;

	return nCompareRtn;
}

//#################################################################################################
CStrW CFilePathSegmentW::GetBase(void) const
{
	CStrW str;

	if(m_nStrLen)
	{
		str.Assign(m_sz);

		// Remove the extension if there is one
		size_t nPos = str.ReverseFind(g_chPeriodW);
		if(nPos != CStrW::not_found)
			str.Delete(nPos, CStrW::end);
	}

	return str;
}

//#################################################################################################
CStrW CFilePathSegmentW::GetExtension(void) const
{
	CStrW str;

	if(m_nStrLen)
	{
		size_t nPos = ReverseFind(m_sz, m_nStrLen, g_chPeriodW, m_nStrLen);
		if(nPos != not_found)
			str.Assign(m_sz + nPos + 1);
	}

	return str;
}

//#################################################################################################
bool CFilePathSegmentW::SetBase(const CStrW &strBase)
{
	bool bSet = false;

	size_t nExtLen = 0;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriodW, m_nStrLen);
	if(nExtPos != not_found)
		nExtLen = m_nStrLen - nExtPos;

	size_t nNewLen = strBase.GetLength() + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<wchar_t[]>(nNewLen + 1);
		if(!strBase.IsEmpty())
			std::wmemcpy(szTemp.get(), strBase, strBase.GetLength());
		if(nExtLen)
			std::wmemcpy(szTemp.get() + strBase.GetLength(), m_sz + nExtPos, nExtLen);
		szTemp[nNewLen] = g_chNullW;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::wmemcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegmentW::SetBase(PCWSTR szBase)
{
	bool bSet = false;

	size_t nExtLen = 0;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriodW, m_nStrLen);
	if(nExtPos != not_found)
		nExtLen = m_nStrLen - nExtPos;

	size_t nBaseLen = StringGetLength(szBase);
	size_t nNewLen = nBaseLen + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<wchar_t[]>(nNewLen + 1);
		if(nBaseLen)
			std::wmemcpy(szTemp.get(), szBase, nBaseLen);
		if(nExtLen)
			std::wmemcpy(szTemp.get() + nBaseLen, m_sz + nExtPos, nExtLen);
		szTemp[nNewLen] = g_chNullW;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::wmemcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegmentW::SetExtension(const CStrW &strExt)
{
	bool bSet = false;

	size_t nBaseLen = m_nStrLen;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriodW, m_nStrLen);
	if(nExtPos != not_found)
		nBaseLen = nExtPos;

	size_t nExtLen = strExt.GetLength();
	if(nExtLen && strExt[0] != g_chPeriodW)
		++nExtLen;

	size_t nNewLen = nBaseLen + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<wchar_t[]>(nNewLen + 1);
		if(nBaseLen)
			std::wmemcpy(szTemp.get(), m_sz, nBaseLen);
		if(nExtLen)
		{
			if(strExt[0] != g_chPeriodW)
			{
				szTemp[nBaseLen] = g_chPeriodW;
				std::wmemcpy(szTemp.get() + nBaseLen + 1, strExt, strExt.GetLength());
			}
			else
				std::wmemcpy(szTemp.get() + nBaseLen, strExt, strExt.GetLength());
		}
		szTemp[nNewLen] = g_chNullW;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::wmemcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegmentW::SetExtension(PCWSTR szExt)
{
	bool bSet = false;

	size_t nBaseLen = m_nStrLen;
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriodW, m_nStrLen);
	if(nExtPos != not_found)
		nBaseLen = nExtPos;

	size_t nExtLen = StringGetLength(szExt);
	if(nExtLen && szExt[0] != g_chPeriodW)
		++nExtLen;

	size_t nNewLen = nBaseLen + nExtLen;
	if(nNewLen)
	{
		auto szTemp = std::make_unique<wchar_t[]>(nNewLen + 1);
		if(nBaseLen)
			std::wmemcpy(szTemp.get(), m_sz, nBaseLen);
		if(nExtLen)
		{
			if(szExt[0] != g_chPeriodW)
			{
				szTemp[nBaseLen] = g_chPeriodW;
				std::wmemcpy(szTemp.get() + nBaseLen + 1, szExt, nExtLen - 1);
			}
			else
				std::wmemcpy(szTemp.get() + nBaseLen, szExt, nExtLen);
		}
		szTemp[nNewLen] = g_chNullW;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::wmemcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegmentW::AddExtension(const CStrW &strExt)
{
	bool bSet = false;

	size_t nExtLen = strExt.GetLength();
	if(nExtLen)
	{
		if(strExt[0] != g_chPeriodW)
			++nExtLen;

		size_t nNewLen = m_nStrLen + nExtLen;
		auto szTemp = std::make_unique<wchar_t[]>(nNewLen + 1);
		if(m_nStrLen)
			std::wmemcpy(szTemp.get(), m_sz, m_nStrLen);
		if(strExt[0] != g_chPeriodW)
		{
			szTemp[m_nStrLen] = g_chPeriodW;
			std::wmemcpy(szTemp.get() + m_nStrLen + 1, strExt, strExt.GetLength());
		}
		else
			std::wmemcpy(szTemp.get() + m_nStrLen, strExt, strExt.GetLength());
		szTemp[nNewLen] = g_chNullW;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::wmemcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
bool CFilePathSegmentW::AddExtension(PCWSTR szExt)
{
	bool bSet = false;

	size_t nExtLen = StringGetLength(szExt);
	if(nExtLen)
	{
		if(szExt[0] != g_chPeriodW)
			++nExtLen;

		size_t nNewLen = m_nStrLen + nExtLen;
		auto szTemp = std::make_unique<wchar_t[]>(nNewLen + 1);
		if(m_nStrLen)
			std::wmemcpy(szTemp.get(), m_sz, m_nStrLen);
		if(szExt[0] != g_chPeriodW)
		{
			szTemp[m_nStrLen] = g_chPeriodW;
			std::wmemcpy(szTemp.get() + m_nStrLen + 1, szExt, nExtLen - 1);
		}
		else
			std::wmemcpy(szTemp.get() + m_nStrLen, szExt, nExtLen);
		szTemp[nNewLen] = g_chNullW;

		if(IsValidSegment(szTemp.get()))
		{
			if(Alloc(nNewLen + 1))
			{
				std::wmemcpy(m_sz, szTemp.get(), nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
				bSet = true;
			}
		}
	}

	return bSet;
}

//#################################################################################################
void CFilePathSegmentW::RemoveExtension(void)
{
	size_t nExtPos = ReverseFind(m_sz, m_nStrLen, g_chPeriodW, m_nStrLen);
	if(nExtPos != not_found)
	{
		m_sz[nExtPos] = g_chNullW;
		m_nStrLen = nExtPos;
	}
}

//#################################################################################################
bool CFilePathSegmentW::IsValidSegment(const CStrW &strSegment)
{	// Valid Windows path segments are of length 1 through MAX_PATH,
	// and can contain any character except the following:
	//   " \ / | * ? < >
	// It can only contain a ':' if it is X:
	// Note: Allow * and ? as they are wildcards.
	// Valid Linux/MacOS path segments are of length 1 through MAX_PATH,
	// and can contain any character except '/'

	// Length between 1 and MAX_PATH
	size_t nStrLen = strSegment.GetLength();
	if(nStrLen == 0 || nStrLen > MAX_PATH)
		return false;

#ifdef _WIN32
	// Does it contain an invalid character or a colon out of place?
	if(FindFirstOf(strSegment, nStrLen, g_szInvalidSegmentW, g_nInvalidSegmentLen) || (std::wmemchr(strSegment, g_chColonW, nStrLen) && (nStrLen != 2 || strSegment[0] == g_chColonW)))
		return false;
#else
	// Does it contain an invalid character?
	if(std::wmemchr(strSegment, g_chDelimiterW, nStrLen))
		return false;
#endif

	return true;
}

//#################################################################################################
bool CFilePathSegmentW::IsValidSegment(PCWSTR szSegment)
{	// Valid Windows path segments are of length 1 through MAX_PATH,
	// and can contain any character except the following:
	//   " \ / | * ? < >
	// It can only contain a ':' if it is X:
	// Note: Allow * and ? as they are wildcards.
	// Valid Linux/MacOS path segments are of length 1 through MAX_PATH,
	// and can contain any character except '/'

	// Cannot be nullptr
	if(szSegment == nullptr)
		return false;

	// Length between 1 and MAX_PATH
	size_t nStrLen = StringGetLength(szSegment);
	if(nStrLen == 0 || nStrLen > MAX_PATH)
		return false;

#ifdef _WIN32
	// Does it contain an invalid character or a colon out of place?
	if(FindFirstOf(szSegment, nStrLen, g_szInvalidSegmentW, g_nInvalidSegmentLen) || (std::wmemchr(szSegment, g_chColonW, nStrLen) && (nStrLen != 2 || *szSegment == g_chColonW)))
		return false;
#else
	// Does it contain an invalid character?
	if(std::wmemchr(szSegment, g_chDelimiterW, nStrLen))
		return false;
#endif

	return true;
}

//#################################################################################################
bool CFilePathSegmentW::IsValidSegment(const wchar_t chSegment)
{	// Valid Windows path segments are of length 1 through MAX_PATH,
	// and can contain any character except the following:
	//   " \ / | * ? < >
	// It can only contain a ':' if it is X:
	// Note: Allow * and ? as they are wildcards.
	// Valid Linux/MacOS path segments are of length 1 through MAX_PATH,
	// and can contain any character except '/'

#ifdef _WIN32
	// Cannot be nullptr, ':', or an invalid character
	if(chSegment == g_chNullW || chSegment == g_chColonW || FindFirstOf(&chSegment, 1, g_szInvalidSegmentW, g_nInvalidSegmentLen))
		return false;
#else
	// Cannot be nullptr or '/'
	if(chSegment == g_chNullW || chSegment == g_chDelimiterW)
		return false;
#endif

	return true;
}

//#################################################################################################
bool CFilePathSegmentW::Alloc(const size_t nBufLen)
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
void CFilePathSegmentW::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PWSTR)g_szNullW;
	m_nStrLen = m_nBufLen = 0;
}

//#################################################################################################
PCWSTR CFilePathSegmentW::FindFirstOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	PCWSTR szResult = nullptr;
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if((szResult = (PCWSTR)std::wmemchr(szFind, sz[nIndex], nFindLen)) != nullptr)
			return szResult;
	}

	return szResult;
}

//#################################################################################################
size_t CFilePathSegmentW::ReverseFind(PCWSTR sz, const size_t nStrLen, const wchar_t chFind, const size_t nStart)
{
	if(nStrLen && nStart)
	{
		size_t nIndex;
		if(nStart >= nStrLen)
			nIndex = nStrLen - 1;
		else
			nIndex = nStart - 1;

		do{
			if(sz[nIndex] == chFind)
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}




/////////////////////////////////////////////////
// CMachineName8
/////////////////////////////////////////////////

//#################################################################################################
CMachineName8::CMachineName8(CMachineName8 &&src) noexcept
	: CFilePathSegment8(std::move(src))
{
}

//#################################################################################################
CMachineName8::CMachineName8(const CMachineNameW &machine)
{
	Assign(machine);
}

//#################################################################################################
CMachineName8::CMachineName8(const CStr8 &str)
{
	Assign(str);
}

//#################################################################################################
CMachineName8::CMachineName8(const CStrW &str)
{
	Assign(str);
}

//#################################################################################################
CMachineName8::CMachineName8(PCSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CMachineName8::CMachineName8(PCWSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CMachineName8::CMachineName8(const char ch)
{
	Assign(ch);
}

//#################################################################################################
CMachineName8::CMachineName8(const wchar_t ch)
{
	Assign(ch);
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(const CMachineName8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(const CMachineNameW &machine)
{
	Assign(machine);
	return *this;
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CMachineName8 &CMachineName8::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CMachineName8::Assign(const CMachineName8 &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &machine)
	{
		if(machine.m_nStrLen)
		{
			if(Alloc(machine.m_nStrLen + 1))
			{
				std::memcpy(m_sz, machine.m_sz, machine.m_nStrLen);
				m_nStrLen = machine.m_nStrLen;
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
ERRCODE CMachineName8::Assign(const CMachineNameW &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(machine.m_nStrLen)
	{
		if(Alloc(WideToUtf8(machine.m_sz, machine.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(machine.m_sz, machine.m_nStrLen, m_sz, m_nBufLen);
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
ERRCODE CMachineName8::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidMachineName(str))
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
ERRCODE CMachineName8::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CMachineNameW::IsValidMachineName(str))
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
ERRCODE CMachineName8::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidMachineName(sz))
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
ERRCODE CMachineName8::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CMachineNameW::IsValidMachineName(sz))
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
ERRCODE CMachineName8::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidMachineName(ch))
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
ERRCODE CMachineName8::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CMachineNameW::IsValidMachineName(ch))
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
bool CMachineName8::IsValidMachineName(const CStr8 &strMachineName)
{	// Valid machine names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	// Length between 1 and 63
	size_t nStrLen = strMachineName.GetLength();
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(strMachineName.GetFirst() == g_chHyphen8 || strMachineName.GetFirst() == g_chPeriod8 || strMachineName.GetLast() == g_chHyphen8 || strMachineName.GetLast() == g_chPeriod8)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(strMachineName, nStrLen, g_szDoublePeriod8, 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(strMachineName, nStrLen, g_szValidMachineName8, g_nValidMachineNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CMachineName8::IsValidMachineName(PCSTR szMachineName)
{	// Valid machine names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	// Cannot be nullptr
	if(szMachineName == nullptr)
		return false;

	// Length between 1 and 63
	size_t nStrLen = StringGetLength(szMachineName);
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(szMachineName[0] == g_chHyphen8 || szMachineName[0] == g_chPeriod8 || szMachineName[nStrLen - 1] == g_chHyphen8 || szMachineName[nStrLen - 1] == g_chPeriod8)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(szMachineName, nStrLen, g_szDoublePeriod8, 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(szMachineName, nStrLen, g_szValidMachineName8, g_nValidMachineNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CMachineName8::IsValidMachineName(const char chMachineName)
{	// Valid machine names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	return IsAlphaDigitChar8(chMachineName);
}

//#################################################################################################
PCSTR CMachineName8::FindStrPtr(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
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
PCSTR CMachineName8::FindFirstNotOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if(!std::memchr(szFind, sz[nIndex], nFindLen))
			return sz + nIndex;
	}

	return nullptr;
}




/////////////////////////////////////////////////
// CMachineNameW
/////////////////////////////////////////////////

//#################################################################################################
CMachineNameW::CMachineNameW(CMachineNameW &&src) noexcept
	: CFilePathSegmentW(std::move(src))
{
}

//#################################################################################################
CMachineNameW::CMachineNameW(const CMachineName8 &machine)
{
	Assign(machine);
}

//#################################################################################################
CMachineNameW::CMachineNameW(const CStrW &str)
{
	Assign(str);
}

//#################################################################################################
CMachineNameW::CMachineNameW(const CStr8 &str)
{
	Assign(str);
}

//#################################################################################################
CMachineNameW::CMachineNameW(PCWSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CMachineNameW::CMachineNameW(PCSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CMachineNameW::CMachineNameW(const wchar_t ch)
{
	Assign(ch);
}

//#################################################################################################
CMachineNameW::CMachineNameW(const char ch)
{
	Assign(ch);
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(const CMachineNameW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(const CMachineName8 &machine)
{
	Assign(machine);
	return *this;
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CMachineNameW &CMachineNameW::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CMachineNameW::Assign(const CMachineNameW &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &machine)
	{
		if(machine.m_nStrLen)
		{
			if(Alloc(machine.m_nStrLen + 1))
			{
				std::wmemcpy(m_sz, machine.m_sz, machine.m_nStrLen);
				m_nStrLen = machine.m_nStrLen;
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
ERRCODE CMachineNameW::Assign(const CMachineName8 &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(machine.m_nStrLen)
	{
		if(Alloc(Utf8ToWide(machine.m_sz, machine.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(machine.m_sz, machine.m_nStrLen, m_sz, m_nBufLen);
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
ERRCODE CMachineNameW::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidMachineName(str))
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
ERRCODE CMachineNameW::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CMachineName8::IsValidMachineName(str))
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
ERRCODE CMachineNameW::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidMachineName(sz))
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
ERRCODE CMachineNameW::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CMachineName8::IsValidMachineName(sz))
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
ERRCODE CMachineNameW::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidMachineName(ch))
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
ERRCODE CMachineNameW::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CMachineName8::IsValidMachineName(ch))
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
bool CMachineNameW::IsValidMachineName(const CStrW &strMachineName)
{	// Valid machine names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	// Length between 1 and 63
	size_t nStrLen = strMachineName.GetLength();
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(strMachineName.GetFirst() == g_chHyphenW || strMachineName.GetFirst() == g_chPeriodW || strMachineName.GetLast() == g_chHyphenW || strMachineName.GetLast() == g_chPeriodW)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(strMachineName, nStrLen, g_szDoublePeriodW, 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(strMachineName, nStrLen, g_szValidMachineNameW, g_nValidMachineNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CMachineNameW::IsValidMachineName(PCWSTR szMachineName)
{	// Valid machine names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	// Cannot be nullptr
	if(szMachineName == nullptr)
		return false;

	// Length between 1 and 63
	size_t nStrLen = StringGetLength(szMachineName);
	if(nStrLen == 0 || nStrLen > 63)
		return false;

	// Does it start or end with a hyphen or period?
	if(szMachineName[0] == g_chHyphenW || szMachineName[0] == g_chPeriodW || szMachineName[nStrLen - 1] == g_chHyphenW || szMachineName[nStrLen - 1] == g_chPeriodW)
		return false;

	// Does it contain double periods?
	if(FindStrPtr(szMachineName, nStrLen, g_szDoublePeriodW, 2))
		return false;

	// Does it contain an invalid character?
	if(FindFirstNotOf(szMachineName, nStrLen, g_szValidMachineNameW, g_nValidMachineNameLen))
		return false;

	return true;
}

//#################################################################################################
bool CMachineNameW::IsValidMachineName(const wchar_t chMachineName)
{	// Valid machine names are of length 1 through 63,
	// and can only contain letters (A-Z, a-z), numbers (0-9), hyphens, and periods.
	// Also, first and last character cannot be a hyphen or period.

	return IsAlphaDigitCharW(chMachineName);
}

//#################################################################################################
PCWSTR CMachineNameW::FindStrPtr(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
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
PCWSTR CMachineNameW::FindFirstNotOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	for(size_t nIndex = 0; nIndex < nStrLen; ++nIndex)
	{
		if(!std::wmemchr(szFind, sz[nIndex], nFindLen))
			return sz + nIndex;
	}

	return nullptr;
}




/////////////////////////////////////////////////
// CFilePath8
/////////////////////////////////////////////////

//#################################################################################################
CFilePath8::CFilePath8(void)
	: m_eType(EPT_Empty)
{
}

//#################################################################################################
CFilePath8::CFilePath8(const CFilePath8 &src)
	: m_eType(EPT_Empty)
{
	Assign(src);
}

//#################################################################################################
CFilePath8::CFilePath8(CFilePath8 &&src) noexcept
	: m_eType(src.m_eType),
	m_vecSegments(std::move(src.m_vecSegments))
{
	src.m_eType = EPT_Empty;
}

//#################################################################################################
CFilePath8::CFilePath8(const CFilePathW &path)
	: m_eType(EPT_Empty)
{
	Assign(path);
}

//#################################################################################################
CFilePath8::CFilePath8(const CFilePathSegment8 &path)
	: m_eType(EPT_Empty)
{
	Assign(path);
}

//#################################################################################################
CFilePath8::CFilePath8(const CFilePathSegmentW &path)
	: m_eType(EPT_Empty)
{
	Assign(path);
}

//#################################################################################################
CFilePath8::CFilePath8(const CMachineName8 &machine)
	: m_eType(EPT_Empty)
{
	Assign(machine);
}

//#################################################################################################
CFilePath8::CFilePath8(const CMachineNameW &machine)
	: m_eType(EPT_Empty)
{
	Assign(machine);
}

//#################################################################################################
CFilePath8::CFilePath8(const CStr8 &str)
	: m_eType(EPT_Empty)
{
	ParsePath(str);
}

//#################################################################################################
CFilePath8::CFilePath8(const CStrW &str)
	: m_eType(EPT_Empty)
{
	ParsePath(str.AsUtf8());
}

//#################################################################################################
CFilePath8::CFilePath8(PCSTR sz)
	: m_eType(EPT_Empty)
{
	ParsePath(CStr8(sz));
}

//#################################################################################################
CFilePath8::CFilePath8(PCWSTR sz)
	: m_eType(EPT_Empty)
{
	ParsePath(CStr8(sz));
}

//#################################################################################################
CFilePath8::CFilePath8(const char ch)
	: m_eType(EPT_Empty)
{
	ParsePath(CStr8(ch));
}

//#################################################################################################
CFilePath8::CFilePath8(const wchar_t ch)
	: m_eType(EPT_Empty)
{
	ParsePath(CStr8(ch));
}

//#################################################################################################
CFilePath8::operator CStr8(void) const
{
	return Get();
}

//#################################################################################################
#ifdef _WIN32
CStr8 CFilePath8::Get(const EKernelPath eKernelPath) const
#else
CStr8 CFilePath8::Get(void) const
#endif
{	// Convert the vector of path segments into a single string
	CStr8 str;

	if(!IsEmpty())
	{
		if(IsUNC())
			str.Assign(g_szDoubleDelimiter8);
		else if(m_eType == EPT_CurrentDriveRelative)
			str.Assign(g_chDelimiter8);
#ifndef _WIN32
		else if(m_eType == EPT_FullyRooted)
			str.Assign(g_chDelimiter8);
#endif

		if(!m_vecSegments.empty())
		{
			size_t nCount = 0;
			auto itr = m_vecSegments.begin();
			str.Append((*itr)->Get());

			while(++itr != m_vecSegments.end())
			{
				if(m_eType != EPT_DriveRelative || nCount != 0)
					str.Append(g_chDelimiter8);
				str.Append((*itr)->Get());
				++nCount;
			}

			// Add the trailing backslash for rooted drive paths (i.e. "X:\")
			if(IsRoot())
				str.Append(g_chDelimiter8);
		}

#ifdef _WIN32
		if(eKernelPath == EKP_Always || (eKernelPath == EKP_Auto && str.GetLength() > INTERNAL_MAX_PATH))
		{
			if(IsDrive())
				str.Prepend(g_szKernelPrefix8);
			else if(IsUNC())
				str.Insert(2, g_szUNCKernelPartialPrefix8);
		}
#endif
	}

	return str;
}

//#################################################################################################
#ifdef _WIN32
size_t CFilePath8::GetLength(const bool bIncludeNullTerm, const EKernelPath eKernelPath) const
#else
size_t CFilePath8::GetLength(const bool bIncludeNullTerm) const
#endif
{	// Length in characters
	size_t nStrLen = 0;

	if(!IsEmpty())
	{
		if(IsUNC())
			nStrLen = 2;
		else if(m_eType == EPT_CurrentDriveRelative)
			nStrLen = 1;
#ifndef _WIN32
		else if(m_eType == EPT_FullyRooted)
			nStrLen = 1;
#endif

		if(!m_vecSegments.empty())
		{
			size_t nCount = 0;
			auto itr = m_vecSegments.begin();
			nStrLen += (*itr)->GetLength();

			while(++itr != m_vecSegments.end())
			{
				if(m_eType != EPT_DriveRelative || nCount != 0)
					++nStrLen;
				nStrLen += (*itr)->GetLength();
				++nCount;
			}

			// Add the trailing backslash for rooted drive paths (i.e. "X:\")
			if(IsRoot())
				++nStrLen;
		}

#ifdef _WIN32
		if(eKernelPath == EKP_Always || (eKernelPath == EKP_Auto && nStrLen > INTERNAL_MAX_PATH))
		{
			if(IsDrive())
				nStrLen += g_nKernelPrefixLen;
			else if(IsUNC())
				nStrLen += g_nUNCKernelPrefixLen - 2;
		}
#endif

		if(bIncludeNullTerm)
			++nStrLen;
	}

	return nStrLen;
}

//#################################################################################################
#ifdef _WIN32
size_t CFilePath8::GetSize(const bool bIncludeNullTerm, const EKernelPath eKernelPath) const
#else
size_t CFilePath8::GetSize(const bool bIncludeNullTerm) const
#endif
{	// Size in bytes
#ifdef _WIN32
	return GetLength(bIncludeNullTerm, eKernelPath);
#else
	return GetLength(bIncludeNullTerm);
#endif
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CFilePath8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(CFilePath8 &&src) noexcept
{
	if(this != &src)
	{
		m_eType = src.m_eType;
		m_vecSegments = std::move(src.m_vecSegments);

		src.m_eType = EPT_Empty;
	}

	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CFilePathW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CFilePathSegment8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CFilePathSegmentW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CMachineName8 &machine)
{
	Assign(machine);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CMachineNameW &machine)
{
	Assign(machine);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CStr8 &str)
{
	ParsePath(str);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const CStrW &str)
{
	ParsePath(str.AsUtf8());
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(PCSTR sz)
{
	ParsePath(CStr8(sz));
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(PCWSTR sz)
{
	ParsePath(CStr8(sz));
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const char ch)
{
	ParsePath(CStr8(ch));
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator=(const wchar_t ch)
{
	ParsePath(CStr8(ch));
	return *this;
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CFilePath8 &path)
{
	if(this != &path)
	{
		Empty();

		m_eType = path.m_eType;

		// Add segments to the path
		auto itr = path.m_vecSegments.begin();
		if(path.m_eType == EPT_UNC)
		{
			m_vecSegments.push_back(std::make_unique<CMachineName8>(**itr));
			++itr;
		}

		while(itr != path.m_vecSegments.end())
		{
			m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(**itr));
			++itr;
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CFilePathW &path)
{
	Empty();

	m_eType = (EPathType)path.m_eType;

	// Add segments to the path
	auto itr = path.m_vecSegments.begin();
	if(path.m_eType == CFilePathW::EPT_UNC)
	{
		m_vecSegments.push_back(std::make_unique<CMachineName8>(**itr));
		++itr;
	}

	while(itr != path.m_vecSegments.end())
	{
		m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(**itr));
		++itr;
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CFilePathSegment8 &path)
{
	Empty();

	if(!path.IsEmpty())
	{	// Is it a drive path?
		if(path.IsDrive())
			m_eType = EPT_FullyRooted;
		else
			m_eType = EPT_Relative;

		m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(path));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CFilePathSegmentW &path)
{
	Empty();

	if(!path.IsEmpty())
	{	// Is it a drive path?
		if(path.IsDrive())
			m_eType = EPT_FullyRooted;
		else
			m_eType = EPT_Relative;

		m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(path));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CMachineName8 &machine)
{
	Empty();

	if(!machine.IsEmpty())
	{
		m_eType = EPT_UNC;
		m_vecSegments.push_back(std::make_unique<CMachineName8>(machine));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CMachineNameW &machine)
{
	Empty();

	if(!machine.IsEmpty())
	{
		m_eType = EPT_UNC;
		m_vecSegments.push_back(std::make_unique<CMachineName8>(machine));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CStr8 &str)
{
	return ParsePath(str);
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const CStrW &str)
{
	return ParsePath(str.AsUtf8());
}

//#################################################################################################
ERRCODE CFilePath8::Assign(PCSTR sz)
{
	return ParsePath(CStr8(sz));
}

//#################################################################################################
ERRCODE CFilePath8::Assign(PCWSTR sz)
{
	return ParsePath(CStr8(sz));
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const char ch)
{
	return ParsePath(CStr8(ch));
}

//#################################################################################################
ERRCODE CFilePath8::Assign(const wchar_t ch)
{
	return ParsePath(CStr8(ch));
}

//#################################################################################################
CFilePath8 &CFilePath8::operator+=(const CFilePath8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator+=(const CFilePathW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator+=(const CFilePathSegment8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator+=(const CFilePathSegmentW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator+=(const CMachineName8 &machine)
{
	Append(machine);
	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::operator+=(const CMachineNameW &machine)
{
	Append(machine);
	return *this;
}

//#################################################################################################
ERRCODE CFilePath8::Prepend(const CFilePath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else if(HasLeadingDots())
		{
			if(!SafeDotAppend(path, *this))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegment8>(**itr));

			m_eType = path.m_eType;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Prepend(const CFilePathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else if(HasLeadingDots())
		{
			if(!SafeDotAppend(CFilePath8(path), *this))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegment8>(**itr));

			m_eType = (EPathType)path.m_eType;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Prepend(const CFilePathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else if(*m_vecSegments.front() == g_szDoublePeriod8)
		{
			m_vecSegments.erase(m_vecSegments.begin());
			if(m_vecSegments.empty())
				m_eType = EPT_Empty;
		}
		else
		{
			if(*m_vecSegments.front() == g_chPeriod8)
				m_vecSegments.erase(m_vecSegments.begin());

			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegment8>(path));
			m_eType = path.IsDrive() ? EPT_FullyRooted : EPT_Relative;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Prepend(const CFilePathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else if(*m_vecSegments.front() == g_szDoublePeriod8)
		{
			m_vecSegments.erase(m_vecSegments.begin());
			if(m_vecSegments.empty())
				m_eType = EPT_Empty;
		}
		else
		{
			if(*m_vecSegments.front() == g_chPeriod8)
				m_vecSegments.erase(m_vecSegments.begin());

			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegment8>(path));
			m_eType = path.IsDrive() ? EPT_FullyRooted : EPT_Relative;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Prepend(const CMachineName8 &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineName8>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Prepend(const CMachineNameW &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineName8>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Append(const CFilePath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
			Assign(path);
		else if(path.IsDrive() || path.IsUNC())		// Cannot append a drive or UNC path to an existing path
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path.HasLeadingDots())
		{
			if(!SafeDotAppend(*this, path))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(*segment));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Append(const CFilePathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
			Assign(path);
		else if(path.IsDrive() || path.IsUNC())		// Cannot append a drive or UNC path to an existing path
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path.HasLeadingDots())
		{
			if(!SafeDotAppend(*this, path))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(*segment));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Append(const CFilePathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
		{	// Is it a drive path?
			if(path.IsDrive())
				m_eType = EPT_FullyRooted;
			else
				m_eType = EPT_Relative;

			m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(path));
		}
		else if(path.IsDrive())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path == g_szDoublePeriod8)
		{
			if((IsDrive() && m_vecSegments.size() == 1) || (IsUNC() && m_vecSegments.size() == 2) || m_vecSegments.empty())
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{
				m_vecSegments.pop_back();
				if(m_vecSegments.empty())
					m_eType = EPT_Empty;
			}
		}
		else if(path != g_chPeriod8)
			m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Append(const CFilePathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
		{	// Is it a drive path?
			if(path.IsDrive())
				m_eType = EPT_FullyRooted;
			else
				m_eType = EPT_Relative;

			m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(path));
		}
		else if(path.IsDrive())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path == g_szDoublePeriodW)
		{
			if((IsDrive() && m_vecSegments.size() == 1) || (IsUNC() && m_vecSegments.size() == 2) || m_vecSegments.empty())
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{
				m_vecSegments.pop_back();
				if(m_vecSegments.empty())
					m_eType = EPT_Empty;
			}
		}
		else if(path != g_chPeriodW)
			m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Append(const CMachineName8 &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(!IsEmpty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineName8>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Append(const CMachineNameW &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(!IsEmpty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineName8>(machine));
		}
	}

	return nErrorCode;
}

#ifndef _WIN32
//#################################################################################################
CFilePath8 operator+(const CFilePath8 &path1, const CFilePath8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePath8 &path1, const CFilePathW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathW &path1, const CFilePath8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathW &path1, const CFilePathW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePath8 &path1, const CFilePathSegment8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePath8 &path1, const CFilePathSegmentW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathW &path1, const CFilePathSegment8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathW &path1, const CFilePathSegmentW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePath8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePathW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePath8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePathW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePathSegment8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePathSegmentW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePathSegment8 &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePathSegmentW &path2)
{
	CFilePath8 pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineName8 &machine, const CFilePath8 &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineName8 &machine, const CFilePathW &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineNameW &machine, const CFilePath8 &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineNameW &machine, const CFilePathW &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineName8 &machine, const CFilePathSegment8 &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineName8 &machine, const CFilePathSegmentW &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineNameW &machine, const CFilePathSegment8 &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePath8 operator+(const CMachineNameW &machine, const CFilePathSegmentW &path)
{
	CFilePath8 pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}
#endif

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CFilePath8 &path)
{
#ifdef _WIN32
	stream << path.Get(CFilePath8::EKP_Never);
#else
	stream << path.Get();
#endif
	return stream;
}

//#################################################################################################
bool CFilePath8::operator==(const CFilePath8 &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_eType != path.m_eType)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if(**itr1 != **itr2)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePath8::operator==(const CFilePathSegment8 &path) const
{	// If the path is empty we have nothing to compare against
	if(IsEmpty())
		return path.IsEmpty();

	// Are they the same size?
	if(m_vecSegments.size() != 1)
		return false;

	// We need to compare the segment
	if(path != *m_vecSegments.front())
		return false;

	return true;
}

//#################################################################################################
bool CFilePath8::operator!=(const CFilePath8 &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_eType != path.m_eType)
		return true;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if(**itr1 != **itr2)
			return true;
	}

	return false;
}

//#################################################################################################
bool CFilePath8::operator!=(const CFilePathSegment8 &path) const
{	// If the path is empty we have nothing to compare against
	if(IsEmpty())
		return !path.IsEmpty();

	// Are they the same size?
	if(m_vecSegments.size() != 1)
		return true;

	// We need to compare the segment
	if(path != *m_vecSegments.front())
		return true;

	return false;
}

//#################################################################################################
bool CFilePath8::operator<(const CFilePath8 &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePath8::operator<=(const CFilePath8 &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePath8::operator>(const CFilePath8 &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePath8::operator>=(const CFilePath8 &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePath8::Compare(const CFilePath8 &path, const bool bCaseInsensitive) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_eType != path.m_eType)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePath8::Compare(const CFilePathSegment8 &path, const bool bCaseInsensitive) const
{	// If the path is empty we have nothing to compare against
	if(IsEmpty())
		return path.IsEmpty();

	// Are they the same size?
	if(m_vecSegments.size() != 1)
		return false;

	// We need to compare the segment
	if(path.Compare(*m_vecSegments.front(), bCaseInsensitive) != 0)
		return false;

	return true;
}

//#################################################################################################
bool CFilePath8::CompareLeft(const CFilePath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// Are they the same type of path?
	if(m_eType != path.m_eType)
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); nCount-- != 0 && itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePath8::CompareRight(const CFilePath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	for(auto itr1 = m_vecSegments.rbegin(), itr2 = path.m_vecSegments.rbegin(); nCount-- != 0 && itr1 != m_vecSegments.rend() && itr2 != path.m_vecSegments.rend(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePath8::Contains(const CFilePath8 &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty() && m_vecSegments.size() >= path.m_vecSegments.size())
	{
		CFilePath8 pathMid;
		for(size_t nSegment = 0; nSegment <= m_vecSegments.size() - path.m_vecSegments.size(); ++nSegment)
		{
			pathMid.Assign(GetMid(nSegment, path.m_vecSegments.size()));
			if(pathMid.Compare(path, bCaseInsensitive))
			{
				bFound = true;
				break;
			}
		}
	}

	return bFound;
}

//#################################################################################################
bool CFilePath8::Contains(const CFilePathSegment8 &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty())
		bFound = std::any_of(m_vecSegments.begin(), m_vecSegments.end(), [&](const std::unique_ptr<CFilePathSegment8> &segment){return (segment->Compare(path, bCaseInsensitive) == 0);});

	return bFound;
}

//#################################################################################################
bool CFilePath8::CompareBase(const CFilePath8 &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2) && !path.IsEmpty() && (!path.IsDrive() || path.m_vecSegments.size() > 1) && (!path.IsUNC() || path.m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(*path.m_vecSegments.back(), bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePath8::CompareBase(const CFilePathSegment8 &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(path, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePath8::CompareBase(const CStr8 &strBase, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(strBase, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePath8::CompareBase(PCSTR szBase, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(szBase, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePath8::CompareExtension(const CFilePath8 &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2) && !path.IsEmpty() && (!path.IsDrive() || path.m_vecSegments.size() > 1) && (!path.IsUNC() || path.m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(*path.m_vecSegments.back(), bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePath8::CompareExtension(const CFilePathSegment8 &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(path, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePath8::CompareExtension(const CStr8 &strExt, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(strExt, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePath8::CompareExtension(PCSTR szExt, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(szExt, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
size_t CFilePath8::CountCompare(const CFilePath8 &path, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	// Compare each segment in the path until a difference is found
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			break;
		++nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CFilePath8::ReverseCountCompare(const CFilePath8 &path, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	// Compare each segment in the path until a difference is found
	for(auto itr1 = m_vecSegments.rbegin(), itr2 = path.m_vecSegments.rbegin(); itr1 != m_vecSegments.rend() && itr2 != path.m_vecSegments.rend(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			break;
		++nCount;
	}

	return nCount;
}

//#################################################################################################
void CFilePath8::Empty(void)
{
	m_eType = EPT_Empty;
	m_vecSegments.clear();
}

//#################################################################################################
bool CFilePath8::SetAnchor(const bool bAnchor)
{
	bool bReturn = false;

#ifdef _WIN32
	if(bAnchor && m_eType == EPT_DriveRelative)
	{
		m_eType = EPT_FullyRooted;
		bReturn = true;
	}
	else if(bAnchor && m_eType == EPT_Relative)
	{
		m_eType = EPT_CurrentDriveRelative;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_FullyRooted)
	{
		m_eType = EPT_DriveRelative;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_CurrentDriveRelative)
	{
		m_eType = EPT_Relative;
		bReturn = true;
	}
#else
	if(bAnchor && m_eType == EPT_Relative && !HasLeadingDots())
	{
		m_eType = EPT_FullyRooted;
		bReturn = true;
	}
	else if(bAnchor && m_eType == EPT_Empty)
	{
		m_eType = EPT_FullyRooted;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_FullyRooted && !m_vecSegments.empty())
	{
		m_eType = EPT_Relative;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_FullyRooted)
	{
		m_eType = EPT_Empty;
		bReturn = true;
	}
#endif

	return bReturn;
}

//#################################################################################################
bool CFilePath8::SimplifyDots(void)
{
	bool bSimplified = false;

	if(!IsEmpty())
	{
		CFilePath8 pathTmp(*this);

		size_t nSegment = 0;
		auto itr = pathTmp.m_vecSegments.begin();
		while(itr != pathTmp.m_vecSegments.end())
		{
			if(**itr == g_chPeriod8)
			{
				itr = pathTmp.m_vecSegments.erase(itr);
				bSimplified = true;
				continue;
			}
			else if(**itr == g_szDoublePeriod8)
			{
				if(itr == pathTmp.m_vecSegments.begin() || (IsDrive() && nSegment <= 1) || (IsUNC() && nSegment <= 2))
				{
					bSimplified = false;
					break;
				}

				bSimplified = true;
				auto itrPrev = itr;
				itr = pathTmp.m_vecSegments.erase(--itrPrev, ++itr);
				--nSegment;
				continue;
			}

			++itr;
			++nSegment;
		}

		if(bSimplified)
		{
			if(pathTmp.IsEmpty())
				Empty();
			else
				Assign(pathTmp);
		}
	}

	return bSimplified;
}

//#################################################################################################
bool CFilePath8::IsEmpty(void) const noexcept
{
	return (m_eType == EPT_Empty);
}

//#################################################################################################
bool CFilePath8::IsDrive(void) const noexcept
{
	return (m_eType == EPT_FullyRooted || m_eType == EPT_DriveRelative);
}

//#################################################################################################
bool CFilePath8::IsUNC(void) const noexcept
{
	return (m_eType == EPT_UNC);
}

//#################################################################################################
bool CFilePath8::IsFullyRooted(void) const noexcept
{
	return (m_eType == EPT_FullyRooted);
}

//#################################################################################################
bool CFilePath8::IsRelative(void) const noexcept
{
	return (m_eType == EPT_Relative || m_eType == EPT_DriveRelative || m_eType == EPT_CurrentDriveRelative);
}

//#################################################################################################
bool CFilePath8::IsRoot(void) const noexcept
{
#ifdef _WIN32
	return (m_eType == EPT_FullyRooted && m_vecSegments.size() == 1);
#else
	return (m_eType == EPT_FullyRooted && m_vecSegments.empty());
#endif
}

//#################################################################################################
bool CFilePath8::IsAnchored(void) const noexcept
{
	return (m_eType == EPT_FullyRooted || m_eType == EPT_CurrentDriveRelative);
}

//#################################################################################################
bool CFilePath8::HasLeadingDots(void) const
{
	return (!m_vecSegments.empty() && (*m_vecSegments.front() == g_chPeriod8 || *m_vecSegments.front() == g_szDoublePeriod8));
}

//#################################################################################################
bool CFilePath8::HasExtension(void) const
{
	bool bExt = false;

	if(!m_vecSegments.empty())
	{
		CStr8 str(m_vecSegments.back()->Get());
		if(str.Contains(g_chPeriod8))
			bExt = true;
	}

	return bExt;
}

//#################################################################################################
bool CFilePath8::HasWildcard(void) const
{
	return std::any_of(m_vecSegments.rbegin(), m_vecSegments.rend(), [](const std::unique_ptr<CFilePathSegment8> &segment){return segment->HasWildcard();});
}

//#################################################################################################
size_t CFilePath8::GetSegmentCount(void) const noexcept
{
	return m_vecSegments.size();
}

//#################################################################################################
CFilePathSegment8 CFilePath8::GetSegment(const size_t nSegment) const
{
	CFilePathSegment8 path;

	if(nSegment < m_vecSegments.size())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(nCount-- != 0)
			++itr;

		path.Assign(**itr);
	}

	return path;
}

//#################################################################################################
CFilePathSegment8 CFilePath8::GetFirstSegment(void) const
{
	return !m_vecSegments.empty() ? CFilePathSegment8(*m_vecSegments.front()) : CFilePathSegment8();
}

//#################################################################################################
CFilePathSegment8 CFilePath8::GetLastSegment(void) const
{
	return !m_vecSegments.empty() ? CFilePathSegment8(*m_vecSegments.back()) : CFilePathSegment8();
}

//#################################################################################################
CFilePath8 CFilePath8::GetLeft(const size_t nSegmentCount) const
{
	CFilePath8 path(*this);
	path.CropLeft(nSegmentCount);

	return path;
}

//#################################################################################################
CFilePath8 CFilePath8::GetRight(const size_t nSegmentCount) const
{
	CFilePath8 path(*this);
	path.CropRight(nSegmentCount);

	return path;
}

//#################################################################################################
CFilePath8 CFilePath8::GetMid(const size_t nSegment, const size_t nSegmentCount) const
{
	CFilePath8 path;

	if(nSegment < m_vecSegments.size())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(itr != m_vecSegments.end() && nCount-- != 0)
			++itr;

		nCount = nSegmentCount;
		while(itr != m_vecSegments.end() && nCount-- != 0)
		{
			path.Append(**itr);
			++itr;
		}

		if(nSegment == 0 && nSegmentCount)
			path.m_eType = m_eType;
	}

	return path;
}

//#################################################################################################
CFilePath8 &CFilePath8::TrimLeft(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{	// If it was a drive, UNC, or non-relative path it no longer is because we are removing the first segment
			m_eType = EPT_Relative;

			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::TrimRight(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecSegments.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::CropLeft(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecSegments.size())
	{
		if(nSegmentCount == 0)
			Empty();
		else
		{
			size_t nToRemove = m_vecSegments.size() - nSegmentCount;
			while(nToRemove--)
				m_vecSegments.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CFilePath8 &CFilePath8::CropRight(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecSegments.size())
	{
		if(nSegmentCount == 0)
			Empty();
		else
		{	// If it was a drive, UNC, or non-relative path it no longer is because we are removing the first segment
			m_eType = EPT_Relative;

			size_t nToRemove = m_vecSegments.size() - nSegmentCount;
			while(nToRemove--)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CFilePath8::Insert(const size_t nSegment, const CFilePath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if((nSegment == 0 && (IsDrive() || IsUNC())) || (nSegment && (path.IsDrive() || path.IsUNC())))
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.insert(itr, std::make_unique<CFilePathSegment8>(*segment));

			if(nSegment == 0)
				m_eType = path.m_eType;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Insert(const size_t nSegment, const CFilePathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if((nSegment == 0 && (IsDrive() || IsUNC())) || (nSegment && path.IsDrive()))
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			m_vecSegments.insert(itr, std::make_unique<CFilePathSegment8>(path));

			if(nSegment == 0)
				m_eType = path.IsDrive() ? EPT_FullyRooted : EPT_Relative;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePath8::Insert(const CMachineName8 &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineName8>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
CFilePath8 &CFilePath8::Delete(const size_t nSegment, const size_t nSegmentCount)
{
	if(nSegment < m_vecSegments.size() && nSegmentCount != 0)
	{
		if(nSegment == 0 && nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			nCount = nSegmentCount;
			while(itr != m_vecSegments.end() && nCount-- != 0)
				itr = m_vecSegments.erase(itr);

			if(nSegment == 0)
				m_eType = EPT_Relative;
		}
	}

	return *this;
}

//#################################################################################################
size_t CFilePath8::Remove(const CFilePathSegment8 &path, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecSegments.begin();
	while(itr != m_vecSegments.end())
	{
		if((*itr)->Compare(path, bCaseInsensitive) == 0)
		{
			if(itr == m_vecSegments.begin())
				m_eType = EPT_Relative;

			itr = m_vecSegments.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	if(nCount && m_vecSegments.empty())
		m_eType = EPT_Empty;

	return nCount;
}

//#################################################################################################
size_t CFilePath8::Replace(const CFilePathSegment8 &pathOld, const CFilePathSegment8 &pathNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!pathOld.IsEmpty() && !pathNew.IsEmpty())
	{
		for(auto itr = m_vecSegments.begin(); itr != m_vecSegments.end(); ++itr)
		{
			if((*itr)->Compare(pathOld, bCaseInsensitive) == 0)
			{
				if(itr == m_vecSegments.begin())
				{
					if(pathNew.IsDrive())
						m_eType = EPT_FullyRooted;
					else if(pathOld.IsDrive())
						m_eType = EPT_Relative;

					*itr = std::make_unique<CFilePathSegment8>(pathNew);
					++nCount;
				}
				else if(!pathNew.IsDrive())
				{
					*itr = std::make_unique<CFilePathSegment8>(pathNew);
					++nCount;
				}
			}
		}
	}

	return nCount;
}

//#################################################################################################
CFilePath8 &CFilePath8::Modify(const size_t nSegment, const CFilePathSegment8 &path)
{
	if(nSegment < m_vecSegments.size() && !path.IsEmpty())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(nCount-- != 0)
			++itr;

		*itr = std::make_unique<CFilePathSegment8>(path);
		if(nSegment == 0)
		{
			if(path.IsDrive())
				m_eType = EPT_FullyRooted;
			else if(IsDrive())
				m_eType = EPT_Relative;
		}
	}

	return *this;
}

//#################################################################################################
CMachineName8 CFilePath8::GetMachine(void) const
{
	return IsUNC() ? CMachineName8(*m_vecSegments.front()) : CMachineName8();
}

//#################################################################################################
CFilePathSegment8 CFilePath8::GetShare(void) const
{
	CFilePathSegment8 path;

	if(IsUNC() && m_vecSegments.size() >= 2)
	{
		auto itr = m_vecSegments.begin();
		++itr;

		path.Assign(**itr);
	}

	return path;
}

//#################################################################################################
CFilePath8 CFilePath8::GetMachineAndShare(void) const
{
	CFilePath8 path;

	if(IsUNC() && m_vecSegments.size() >= 2)
	{
		auto itr = m_vecSegments.begin();
		path.m_vecSegments.push_back(std::make_unique<CMachineName8>(**itr));
		++itr;
		path.m_vecSegments.push_back(std::make_unique<CFilePathSegment8>(**itr));
		path.m_eType = EPT_UNC;
	}

	return path;
}

//#################################################################################################
CFilePathSegment8 CFilePath8::GetDrive(void) const
{
	return IsDrive() ? CFilePathSegment8(*m_vecSegments.front()) : CFilePathSegment8();
}

//#################################################################################################
CFilePath8 CFilePath8::GetPath(void) const
{
	CFilePath8 path(*this);

	if((!path.IsDrive() || path.m_vecSegments.size() > 1) && (!path.IsUNC() || path.m_vecSegments.size() > 2))
		path.TrimRight();

	return path;
}

//#################################################################################################
CFilePath8 CFilePath8::GetRelativePath(void) const
{
	CFilePath8 path(*this);

	if(path.IsDrive())
		path.TrimLeft();
	else if(path.IsUNC())
		path.TrimLeft(2);

	return path;
}

//#################################################################################################
CFilePathSegment8 CFilePath8::GetFilename(void) const
{	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	return ((IsDrive() && m_vecSegments.size() > 1) || (IsUNC() && m_vecSegments.size() > 2) || (!IsDrive() && !IsUNC() && !m_vecSegments.empty())) ? CFilePathSegment8(*m_vecSegments.back()) : CFilePathSegment8();
}

//#################################################################################################
CStr8 CFilePath8::GetBase(void) const
{
	CStr8 str;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		str.Assign(m_vecSegments.back()->GetBase());

	return str;
}

//#################################################################################################
CStr8 CFilePath8::GetExtension(void) const
{
	CStr8 str;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		str.Assign(m_vecSegments.back()->GetExtension());

	return str;
}

//#################################################################################################
bool CFilePath8::SetBase(const CStr8 &strBase)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetBase(strBase);

	return bSet;
}

//#################################################################################################
bool CFilePath8::SetBase(PCSTR szBase)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetBase(szBase);

	return bSet;
}

//#################################################################################################
bool CFilePath8::SetExtension(const CStr8 &strExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetExtension(strExt);

	return bSet;
}

//#################################################################################################
bool CFilePath8::SetExtension(PCSTR szExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetExtension(szExt);

	return bSet;
}

//#################################################################################################
bool CFilePath8::AddExtension(const CStr8 &strExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->AddExtension(strExt);

	return bSet;
}

//#################################################################################################
bool CFilePath8::AddExtension(PCSTR szExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->AddExtension(szExt);

	return bSet;
}

//#################################################################################################
void CFilePath8::RemoveExtension(void)
{	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		m_vecSegments.back()->RemoveExtension();
}

//#################################################################################################
void CFilePath8::DebugPrint(const bool bAppendNewline) const
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

#ifdef _WIN32
//#################################################################################################
bool CFilePath8::SplitDrivePath(PCSTR szPath, CStr8 &strDrive, CStr8 &strPath, CStr8 &strFileBase, CStr8 &strFileExt)
{
	bool bValid = false;

	strDrive.Empty();
	strPath.Empty();
	strFileBase.Empty();
	strFileExt.Empty();

	CFilePath8 path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strDrive.Assign(path.GetDrive());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFileBase.Assign(path.GetBase());
		strFileExt.Assign(path.GetExtension());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePath8::SplitDrivePath(PCSTR szPath, CStr8 &strDrive, CStr8 &strPath, CStr8 &strFilename)
{
	bool bValid = false;

	strDrive.Empty();
	strPath.Empty();
	strFilename.Empty();

	CFilePath8 path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strDrive.Assign(path.GetDrive());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFilename.Assign(path.GetFilename());
		bValid = true;
	}

	return bValid;
}
#endif

//#################################################################################################
bool CFilePath8::SplitUNCPath(PCSTR szPath, CStr8 &strMachine, CStr8 &strShare, CStr8 &strPath, CStr8 &strFileBase, CStr8 &strFileExt)
{
	bool bValid = false;

	strMachine.Empty();
	strShare.Empty();
	strPath.Empty();
	strFileBase.Empty();
	strFileExt.Empty();

	CFilePath8 path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsUNC() || path.IsRelative()))
	{
		strMachine.Assign(path.GetMachine());
		strShare.Assign(path.GetShare());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFileBase.Assign(path.GetBase());
		strFileExt.Assign(path.GetExtension());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePath8::SplitUNCPath(PCSTR szPath, CStr8 &strMachine, CStr8 &strShare, CStr8 &strPath, CStr8 &strFilename)
{
	bool bValid = false;

	strMachine.Empty();
	strShare.Empty();
	strPath.Empty();
	strFilename.Empty();

	CFilePath8 path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsUNC() || path.IsRelative()))
	{
		strMachine.Assign(path.GetMachine());
		strShare.Assign(path.GetShare());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFilename.Assign(path.GetFilename());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePath8::SplitPath(PCSTR szPath, CStr8 &strPath, CStr8 &strFileBase, CStr8 &strFileExt)
{
	bool bValid = false;

	strPath.Empty();
	strFileBase.Empty();
	strFileExt.Empty();

	CFilePath8 path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strPath.Assign(path.GetPath().TrimRight());
		strFileBase.Assign(path.GetBase());
		strFileExt.Assign(path.GetExtension());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePath8::SplitPath(PCSTR szPath, CStr8 &strPath, CStr8 &strFilename)
{
	bool bValid = false;

	strPath.Empty();
	strFilename.Empty();

	CFilePath8 path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strPath.Assign(path.GetPath().TrimRight());
		strFilename.Assign(path.GetFilename());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePath8::IsValidPath(const CStr8 &strPath)
{
	bool bValid = true;

	CFilePath8 path;
	if(path.Assign(strPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CFilePath8::IsValidPath(PCSTR szPath)
{
	bool bValid = true;

	CFilePath8 path;
	if(path.Assign(szPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CFilePath8::IsValidPath(const char chPath)
{
	bool bValid = true;

	CFilePath8 path;
	if(path.Assign(chPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
ERRCODE CFilePath8::ParsePath(const CStr8 &strPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Empty();

	if(strPath.IsEmpty())
		return nErrorCode;

	PCSTR szPath = strPath;
	PCSTR szLastDelim = szPath;
	PCSTR szNextDelim;
	size_t nDelimLen = strPath.GetLength();

#ifdef _WIN32
	// Is it a kernel UNC path?
	if(strPath.CompareLeft(g_szUNCKernelPrefix8, g_nUNCKernelPrefixLen, true) == 0)
	{
		m_eType = EPT_UNC;
		szLastDelim += g_nUNCKernelPrefixLen;
		nDelimLen -= g_nUNCKernelPrefixLen;
	}
	else if(strPath.CompareLeft(g_szKernelPrefix8, g_nKernelPrefixLen) == 0)
	{	// Is it a kernel path?
		if(strPath.GetLength() >= g_nKernelPrefixLen + 2 && szPath[g_nKernelPrefixLen + 1] == g_chColon8)
		{
			if(strPath.GetLength() == g_nKernelPrefixLen + 2 || szPath[g_nKernelPrefixLen + 2] != g_chDelimiter8)
				m_eType = EPT_DriveRelative;
			else
				m_eType = EPT_FullyRooted;

			szLastDelim += g_nKernelPrefixLen;
			nDelimLen -= g_nKernelPrefixLen;
		}
		else
			return FW_ERROR_INVALID_DATA;
	}
	else if(szPath[1] == g_chColon8)
	{	// Is it a drive path?
		if(strPath.GetLength() == 2 || szPath[2] != g_chDelimiter8)
			m_eType = EPT_DriveRelative;
		else
			m_eType = EPT_FullyRooted;
	}
	else
#endif
	{
		if(szPath[0] == g_chDelimiter8 && szPath[1] == g_chDelimiter8)
		{	// Is it a UNC path?
			m_eType = EPT_UNC;
			szLastDelim += 2;
			nDelimLen -= 2;
		}
		else if(szPath[0] == g_chDelimiter8)
		{
#ifdef _WIN32
			m_eType = EPT_CurrentDriveRelative;
#else
			m_eType = EPT_FullyRooted;
#endif
			++szLastDelim;
			--nDelimLen;
		}
		else
			m_eType = EPT_Relative;
	}

	do{
		szNextDelim = (PCSTR)std::memchr(szLastDelim, g_chDelimiter8, nDelimLen);
		if(szNextDelim == nullptr)
		{	// No more delimiters, the rest of the string is the last segment
			if(IsUNC() && m_vecSegments.empty())
			{
				auto pMachine = std::make_unique<CMachineName8>(szLastDelim);
				if(pMachine->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pMachine));
			}
#ifdef _WIN32
			else if(m_vecSegments.empty() && nDelimLen >= 3 && szLastDelim[1] == g_chColon8 && szLastDelim[2] != g_chDelimiter8)
			{	// Special case paths like "c:temp" - this is actually two segments without a delimiter
				auto pPath = std::make_unique<CFilePathSegment8>(strPath.GetMid((size_t)(szLastDelim - szPath), 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));

				pPath = std::make_unique<CFilePathSegment8>(strPath.GetMid((size_t)(szLastDelim - szPath) + 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}
#endif
			else if(*szLastDelim == g_chNull8)
				break;
			else
			{
				auto pPath = std::make_unique<CFilePathSegment8>(szLastDelim);
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}
		}
		else
		{
			if(IsUNC() && m_vecSegments.empty())
			{
				auto pMachine = std::make_unique<CMachineName8>(strPath.GetMid((size_t)(szLastDelim - szPath), (size_t)(szNextDelim - szLastDelim)));
				if(pMachine->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pMachine));
			}
#ifdef _WIN32
			else if(m_vecSegments.empty() && nDelimLen >= 3 && szLastDelim[1] == g_chColon8 && szLastDelim[2] != g_chDelimiter8)
			{	// Special case paths like "c:temp" - this is actually two segments without a delimiter
				auto pPath = std::make_unique<CFilePathSegment8>(strPath.GetMid((size_t)(szLastDelim - szPath), 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));

				pPath = std::make_unique<CFilePathSegment8>(strPath.GetMid((size_t)(szLastDelim - szPath) + 2, (size_t)(szNextDelim - szLastDelim) - 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}
#endif
			else
			{
				auto pPath = std::make_unique<CFilePathSegment8>(strPath.GetMid((size_t)(szLastDelim - szPath), (size_t)(szNextDelim - szLastDelim)));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}

			nDelimLen -= (size_t)(szNextDelim - szLastDelim) + 1;
			szLastDelim = szNextDelim + 1;
		}
	}while(szNextDelim && *szLastDelim != g_chNull8);

	return nErrorCode;
}

//#################################################################################################
bool CFilePath8::SafeDotAppend(const CFilePath8 &path1, const CFilePath8 &path2)
{
	bool bAppended = false;

	bool bValidAppend = true;
	CFilePath8 pathTmp1(path1);
	CFilePath8 pathTmp2(path2);

	while(!pathTmp2.m_vecSegments.empty())
	{
		const CFilePathSegment8 pathSeg(*pathTmp2.m_vecSegments.front());
		if(pathSeg == g_chPeriod8)
			pathTmp2.TrimLeft();
		else if(pathSeg == g_szDoublePeriod8)
		{
			if((pathTmp1.IsDrive() && pathTmp1.m_vecSegments.size() == 1) || (pathTmp1.IsUNC() && pathTmp1.m_vecSegments.size() == 2) || pathTmp1.IsEmpty())
			{
				bValidAppend = false;
				break;
			}

			pathTmp1.TrimRight();
			pathTmp2.TrimLeft();
		}
		else
			break;
	}

	if(bValidAppend)
	{
		Assign(pathTmp1);
		Append(pathTmp2);
		bAppended = true;
	}

	return bAppended;
}

//#################################################################################################
bool CFilePath8::SafeDotAppend(const CFilePath8 &path1, const CFilePathW &path2)
{
	bool bAppended = false;

	bool bValidAppend = true;
	CFilePath8 pathTmp1(path1);
	CFilePathW pathTmp2(path2);

	while(!pathTmp2.m_vecSegments.empty())
	{
		const CFilePathSegmentW pathSeg(*pathTmp2.m_vecSegments.front());
		if(pathSeg == g_chPeriodW)
			pathTmp2.TrimLeft();
		else if(pathSeg == g_szDoublePeriodW)
		{
			if((pathTmp1.IsDrive() && pathTmp1.m_vecSegments.size() == 1) || (pathTmp1.IsUNC() && pathTmp1.m_vecSegments.size() == 2) || pathTmp1.IsEmpty())
			{
				bValidAppend = false;
				break;
			}

			pathTmp1.TrimRight();
			pathTmp2.TrimLeft();
		}
		else
			break;
	}

	if(bValidAppend)
	{
		Assign(pathTmp1);
		Append(pathTmp2);
		bAppended = true;
	}

	return bAppended;
}




/////////////////////////////////////////////////
// CFilePathW
/////////////////////////////////////////////////

//#################################################################################################
CFilePathW::CFilePathW(void)
	: m_eType(EPT_Empty)
{
}

//#################################################################################################
CFilePathW::CFilePathW(const CFilePathW &src)
	: m_eType(EPT_Empty)
{
	Assign(src);
}

//#################################################################################################
CFilePathW::CFilePathW(CFilePathW &&src) noexcept
	: m_eType(src.m_eType),
	m_vecSegments(std::move(src.m_vecSegments))
{
	src.m_eType = EPT_Empty;
}

//#################################################################################################
CFilePathW::CFilePathW(const CFilePath8 &path)
	: m_eType(EPT_Empty)
{
	Assign(path);
}

//#################################################################################################
CFilePathW::CFilePathW(const CFilePathSegmentW &path)
	: m_eType(EPT_Empty)
{
	Assign(path);
}

//#################################################################################################
CFilePathW::CFilePathW(const CFilePathSegment8 &path)
	: m_eType(EPT_Empty)
{
	Assign(path);
}

//#################################################################################################
CFilePathW::CFilePathW(const CMachineNameW &machine)
	: m_eType(EPT_Empty)
{
	Assign(machine);
}

//#################################################################################################
CFilePathW::CFilePathW(const CMachineName8 &machine)
	: m_eType(EPT_Empty)
{
	Assign(machine);
}

//#################################################################################################
CFilePathW::CFilePathW(const CStrW &str)
	: m_eType(EPT_Empty)
{
	ParsePath(str);
}

//#################################################################################################
CFilePathW::CFilePathW(const CStr8 &str)
	: m_eType(EPT_Empty)
{
	ParsePath(str.AsWide());
}

//#################################################################################################
CFilePathW::CFilePathW(PCWSTR sz)
	: m_eType(EPT_Empty)
{
	ParsePath(CStrW(sz));
}

//#################################################################################################
CFilePathW::CFilePathW(PCSTR sz)
	: m_eType(EPT_Empty)
{
	ParsePath(CStrW(sz));
}

//#################################################################################################
CFilePathW::CFilePathW(const wchar_t ch)
	: m_eType(EPT_Empty)
{
	ParsePath(CStrW(ch));
}

//#################################################################################################
CFilePathW::CFilePathW(const char ch)
	: m_eType(EPT_Empty)
{
	ParsePath(CStrW(ch));
}

//#################################################################################################
CFilePathW::operator CStrW(void) const
{
	return Get();
}

//#################################################################################################
#ifdef _WIN32
CStrW CFilePathW::Get(const EKernelPath eKernelPath) const
#else
CStrW CFilePathW::Get(void) const
#endif
{	// Convert the vector of path segments into a single string
	CStrW str;

	if(!IsEmpty())
	{
		if(IsUNC())
			str.Assign(g_szDoubleDelimiterW);
		else if(m_eType == EPT_CurrentDriveRelative)
			str.Assign(g_chDelimiterW);
#ifndef _WIN32
		else if(m_eType == EPT_FullyRooted)
			str.Assign(g_chDelimiterW);
#endif

		if(!m_vecSegments.empty())
		{
			size_t nCount = 0;
			auto itr = m_vecSegments.begin();
			str.Append((*itr)->Get());

			while(++itr != m_vecSegments.end())
			{
				if(m_eType != EPT_DriveRelative || nCount != 0)
					str.Append(g_chDelimiterW);
				str.Append((*itr)->Get());
				++nCount;
			}

			// Add the trailing backslash for rooted drive paths (i.e. "X:\")
			if(IsRoot())
				str.Append(g_chDelimiterW);
		}

#ifdef _WIN32
		if(eKernelPath == EKP_Always || (eKernelPath == EKP_Auto && str.GetLength() > INTERNAL_MAX_PATH))
		{
			if(IsDrive())
				str.Prepend(g_szKernelPrefixW);
			else if(IsUNC())
				str.Insert(2, g_szUNCKernelPartialPrefixW);
		}
#endif
	}

	return str;
}

//#################################################################################################
#ifdef _WIN32
size_t CFilePathW::GetLength(const bool bIncludeNullTerm, const EKernelPath eKernelPath) const
#else
size_t CFilePathW::GetLength(const bool bIncludeNullTerm) const
#endif
{	// Length in characters
	size_t nStrLen = 0;

	if(!IsEmpty())
	{
		if(IsUNC())
			nStrLen = 2;
		else if(m_eType == EPT_CurrentDriveRelative)
			nStrLen = 1;
#ifndef _WIN32
		else if(m_eType == EPT_FullyRooted)
			nStrLen = 1;
#endif

		if(!m_vecSegments.empty())
		{
			size_t nCount = 0;
			auto itr = m_vecSegments.begin();
			nStrLen += (*itr)->GetLength();

			while(++itr != m_vecSegments.end())
			{
				if(m_eType != EPT_DriveRelative || nCount != 0)
					++nStrLen;
				nStrLen += (*itr)->GetLength();
				++nCount;
			}

			// Add the trailing backslash for rooted drive paths (i.e. "X:\")
			if(IsRoot())
				++nStrLen;
		}

#ifdef _WIN32
		if(eKernelPath == EKP_Always || (eKernelPath == EKP_Auto && nStrLen > INTERNAL_MAX_PATH))
		{
			if(IsDrive())
				nStrLen += g_nKernelPrefixLen;
			else if(IsUNC())
				nStrLen += g_nUNCKernelPrefixLen - 2;
		}
#endif

		if(bIncludeNullTerm)
			++nStrLen;
	}

	return nStrLen;
}

//#################################################################################################
#ifdef _WIN32
size_t CFilePathW::GetSize(const bool bIncludeNullTerm, const EKernelPath eKernelPath) const
#else
size_t CFilePathW::GetSize(const bool bIncludeNullTerm) const
#endif
{	// Size in bytes
#ifdef _WIN32
	return GetLength(bIncludeNullTerm, eKernelPath) * sizeof(wchar_t);
#else
	return GetLength(bIncludeNullTerm) * sizeof(wchar_t);
#endif
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CFilePathW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(CFilePathW &&src) noexcept
{
	if(this != &src)
	{
		m_eType = src.m_eType;
		m_vecSegments = std::move(src.m_vecSegments);

		src.m_eType = EPT_Empty;
	}

	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CFilePath8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CFilePathSegmentW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CFilePathSegment8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CMachineNameW &machine)
{
	Assign(machine);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CMachineName8 &machine)
{
	Assign(machine);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CStrW &str)
{
	ParsePath(str);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const CStr8 &str)
{
	ParsePath(str.AsWide());
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(PCWSTR sz)
{
	ParsePath(CStrW(sz));
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(PCSTR sz)
{
	ParsePath(CStrW(sz));
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const wchar_t ch)
{
	ParsePath(CStrW(ch));
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator=(const char ch)
{
	ParsePath(CStrW(ch));
	return *this;
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CFilePathW &path)
{
	if(this != &path)
	{
		Empty();

		m_eType = path.m_eType;

		// Add segments to the path
		auto itr = path.m_vecSegments.begin();
		if(path.m_eType == EPT_UNC)
		{
			m_vecSegments.push_back(std::make_unique<CMachineNameW>(**itr));
			++itr;
		}

		while(itr != path.m_vecSegments.end())
		{
			m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(**itr));
			++itr;
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CFilePath8 &path)
{
	Empty();

	m_eType = (EPathType)path.m_eType;

	// Add segments to the path
	auto itr = path.m_vecSegments.begin();
	if(path.m_eType == CFilePath8::EPT_UNC)
	{
		m_vecSegments.push_back(std::make_unique<CMachineNameW>(**itr));
		++itr;
	}

	while(itr != path.m_vecSegments.end())
	{
		m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(**itr));
		++itr;
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CFilePathSegmentW &path)
{
	Empty();

	if(!path.IsEmpty())
	{	// Is it a drive path?
		if(path.IsDrive())
			m_eType = EPT_FullyRooted;
		else
			m_eType = EPT_Relative;

		m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(path));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CFilePathSegment8 &path)
{
	Empty();

	if(!path.IsEmpty())
	{	// Is it a drive path?
		if(path.IsDrive())
			m_eType = EPT_FullyRooted;
		else
			m_eType = EPT_Relative;

		m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(path));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CMachineNameW &machine)
{
	Empty();

	if(!machine.IsEmpty())
	{
		m_eType = EPT_UNC;
		m_vecSegments.push_back(std::make_unique<CMachineNameW>(machine));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CMachineName8 &machine)
{
	Empty();

	if(!machine.IsEmpty())
	{
		m_eType = EPT_UNC;
		m_vecSegments.push_back(std::make_unique<CMachineNameW>(machine));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CStrW &str)
{
	return ParsePath(str);
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const CStr8 &str)
{
	return ParsePath(str.AsWide());
}

//#################################################################################################
ERRCODE CFilePathW::Assign(PCWSTR sz)
{
	return ParsePath(CStrW(sz));
}

//#################################################################################################
ERRCODE CFilePathW::Assign(PCSTR sz)
{
	return ParsePath(CStrW(sz));
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const wchar_t ch)
{
	return ParsePath(CStrW(ch));
}

//#################################################################################################
ERRCODE CFilePathW::Assign(const char ch)
{
	return ParsePath(CStrW(ch));
}

//#################################################################################################
CFilePathW &CFilePathW::operator+=(const CFilePathW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator+=(const CFilePath8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator+=(const CFilePathSegmentW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator+=(const CFilePathSegment8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator+=(const CMachineNameW &machine)
{
	Append(machine);
	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::operator+=(const CMachineName8 &machine)
{
	Append(machine);
	return *this;
}

//#################################################################################################
ERRCODE CFilePathW::Prepend(const CFilePathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else if(HasLeadingDots())
		{
			if(!SafeDotAppend(path, *this))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegmentW>(**itr));

			m_eType = path.m_eType;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Prepend(const CFilePath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else if(HasLeadingDots())
		{
			if(!SafeDotAppend(CFilePathW(path), *this))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegmentW>(**itr));

			m_eType = (EPathType)path.m_eType;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Prepend(const CFilePathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else if(*m_vecSegments.front() == g_szDoublePeriodW)
		{
			m_vecSegments.erase(m_vecSegments.begin());
			if(m_vecSegments.empty())
				m_eType = EPT_Empty;
		}
		else
		{
			if(*m_vecSegments.front() == g_chPeriodW)
				m_vecSegments.erase(m_vecSegments.begin());

			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegmentW>(path));
			m_eType = path.IsDrive() ? EPT_FullyRooted : EPT_Relative;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Prepend(const CFilePathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else if(*m_vecSegments.front() == g_szDoublePeriodW)
		{
			m_vecSegments.erase(m_vecSegments.begin());
			if(m_vecSegments.empty())
				m_eType = EPT_Empty;
		}
		else
		{
			if(*m_vecSegments.front() == g_chPeriodW)
				m_vecSegments.erase(m_vecSegments.begin());

			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CFilePathSegmentW>(path));
			m_eType = path.IsDrive() ? EPT_FullyRooted : EPT_Relative;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Prepend(const CMachineNameW &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineNameW>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Prepend(const CMachineName8 &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineNameW>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Append(const CFilePathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
			Assign(path);
		else if(path.IsDrive() || path.IsUNC())		// Cannot append a drive or UNC path to an existing path
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path.HasLeadingDots())
		{
			if(!SafeDotAppend(*this, path))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(*segment));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Append(const CFilePath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
			Assign(path);
		else if(path.IsDrive() || path.IsUNC())		// Cannot append a drive or UNC path to an existing path
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path.HasLeadingDots())
		{
			if(!SafeDotAppend(*this, path))
				nErrorCode = FW_ERROR_INVALID_DATA;
		}
		else
		{	// Add segments to the path
			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(*segment));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Append(const CFilePathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
		{	// Is it a drive path?
			if(path.IsDrive())
				m_eType = EPT_FullyRooted;
			else
				m_eType = EPT_Relative;

			m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(path));
		}
		else if(path.IsDrive())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path == g_szDoublePeriodW)
		{
			if((IsDrive() && m_vecSegments.size() == 1) || (IsUNC() && m_vecSegments.size() == 2) || m_vecSegments.empty())
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{
				m_vecSegments.pop_back();
				if(m_vecSegments.empty())
					m_eType = EPT_Empty;
			}
		}
		else if(path != g_chPeriodW)
			m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Append(const CFilePathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(IsEmpty())
		{	// Is it a drive path?
			if(path.IsDrive())
				m_eType = EPT_FullyRooted;
			else
				m_eType = EPT_Relative;

			m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(path));
		}
		else if(path.IsDrive())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(path == g_szDoublePeriod8)
		{
			if((IsDrive() && m_vecSegments.size() == 1) || (IsUNC() && m_vecSegments.size() == 2) || m_vecSegments.empty())
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{
				m_vecSegments.pop_back();
				if(m_vecSegments.empty())
					m_eType = EPT_Empty;
			}
		}
		else if(path != g_chPeriod8)
			m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Append(const CMachineNameW &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(!IsEmpty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineNameW>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Append(const CMachineName8 &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(!IsEmpty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineNameW>(machine));
		}
	}

	return nErrorCode;
}

#ifdef _WIN32
//#################################################################################################
CFilePathW operator+(const CFilePathW &path1, const CFilePathW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathW &path1, const CFilePath8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePath8 &path1, const CFilePathW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePath8 &path1, const CFilePath8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathW &path1, const CFilePathSegmentW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathW &path1, const CFilePathSegment8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePath8 &path1, const CFilePathSegmentW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePath8 &path1, const CFilePathSegment8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePathW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePath8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePathW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePath8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePathSegmentW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePathSegment8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePathSegmentW &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePathSegment8 &path2)
{
	CFilePathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineNameW &machine, const CFilePathW &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineNameW &machine, const CFilePath8 &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineName8 &machine, const CFilePathW &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineName8 &machine, const CFilePath8 &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineNameW &machine, const CFilePathSegmentW &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineNameW &machine, const CFilePathSegment8 &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineName8 &machine, const CFilePathSegmentW &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CFilePathW operator+(const CMachineName8 &machine, const CFilePathSegment8 &path)
{
	CFilePathW pathRtn(machine);
	pathRtn.Append(path);
	return pathRtn;
}
#endif

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CFilePathW &path)
{
#ifdef _WIN32
	stream << path.Get(CFilePathW::EKP_Never);
#else
	stream << path.Get();
#endif
	return stream;
}

//#################################################################################################
bool CFilePathW::operator==(const CFilePathW &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_eType != path.m_eType)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if(**itr1 != **itr2)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePathW::operator==(const CFilePathSegmentW &path) const
{	// If the path is empty we have nothing to compare against
	if(IsEmpty())
		return path.IsEmpty();

	// Are they the same size?
	if(m_vecSegments.size() != 1)
		return false;

	// We need to compare the segment
	if(path != *m_vecSegments.front())
		return false;

	return true;
}

//#################################################################################################
bool CFilePathW::operator!=(const CFilePathW &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_eType != path.m_eType)
		return true;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if(**itr1 != **itr2)
			return true;
	}

	return false;
}

//#################################################################################################
bool CFilePathW::operator!=(const CFilePathSegmentW &path) const
{	// If the path is empty we have nothing to compare against
	if(IsEmpty())
		return !path.IsEmpty();

	// Are they the same size?
	if(m_vecSegments.size() != 1)
		return true;

	// We need to compare the segment
	if(path != *m_vecSegments.front())
		return true;

	return false;
}

//#################################################################################################
bool CFilePathW::operator<(const CFilePathW &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) < 0);
}

//#################################################################################################
bool CFilePathW::operator<=(const CFilePathW &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) <= 0);
}

//#################################################################################################
bool CFilePathW::operator>(const CFilePathW &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) > 0);
}

//#################################################################################################
bool CFilePathW::operator>=(const CFilePathW &path) const
{
	return (Get().Compare(path.Get(), FILE_DEFAULT_CMP) >= 0);
}

//#################################################################################################
bool CFilePathW::Compare(const CFilePathW &path, const bool bCaseInsensitive) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_eType != path.m_eType)
		return false;

	// We need to compare each segment in the path
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePathW::Compare(const CFilePathSegmentW &path, const bool bCaseInsensitive) const
{	// If the path is empty we have nothing to compare against
	if(IsEmpty())
		return path.IsEmpty();

	// Are they the same size?
	if(m_vecSegments.size() != 1)
		return false;

	// We need to compare the segment
	if(path.Compare(*m_vecSegments.front(), bCaseInsensitive) != 0)
		return false;

	return true;
}

//#################################################################################################
bool CFilePathW::CompareLeft(const CFilePathW &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// Are they the same type of path?
	if(m_eType != path.m_eType)
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); nCount-- != 0 && itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePathW::CompareRight(const CFilePathW &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	for(auto itr1 = m_vecSegments.rbegin(), itr2 = path.m_vecSegments.rbegin(); nCount-- != 0 && itr1 != m_vecSegments.rend() && itr2 != path.m_vecSegments.rend(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;
	}

	return true;
}

//#################################################################################################
bool CFilePathW::Contains(const CFilePathW &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty() && m_vecSegments.size() >= path.m_vecSegments.size())
	{
		CFilePathW pathMid;
		for(size_t nSegment = 0; nSegment <= m_vecSegments.size() - path.m_vecSegments.size(); ++nSegment)
		{
			pathMid.Assign(GetMid(nSegment, path.m_vecSegments.size()));
			if(pathMid.Compare(path, bCaseInsensitive))
			{
				bFound = true;
				break;
			}
		}
	}

	return bFound;
}

//#################################################################################################
bool CFilePathW::Contains(const CFilePathSegmentW &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty())
		bFound = std::any_of(m_vecSegments.begin(), m_vecSegments.end(), [&](const std::unique_ptr<CFilePathSegmentW> &segment) {return (segment->Compare(path, bCaseInsensitive) == 0);});

	return bFound;
}

//#################################################################################################
bool CFilePathW::CompareBase(const CFilePathW &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2) && !path.IsEmpty() && (!path.IsDrive() || path.m_vecSegments.size() > 1) && (!path.IsUNC() || path.m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(*path.m_vecSegments.back(), bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePathW::CompareBase(const CFilePathSegmentW &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(path, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePathW::CompareBase(const CStrW &strBase, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(strBase, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePathW::CompareBase(PCWSTR szBase, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareBase(szBase, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePathW::CompareExtension(const CFilePathW &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2) && !path.IsEmpty() && (!path.IsDrive() || path.m_vecSegments.size() > 1) && (!path.IsUNC() || path.m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(*path.m_vecSegments.back(), bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePathW::CompareExtension(const CFilePathSegmentW &path, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(path, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePathW::CompareExtension(const CStrW &strExt, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(strExt, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
bool CFilePathW::CompareExtension(PCWSTR szExt, const bool bCaseInsensitive) const
{
	bool bCompareRtn = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bCompareRtn = (m_vecSegments.back()->CompareExtension(szExt, bCaseInsensitive) == 0);

	return bCompareRtn;
}

//#################################################################################################
size_t CFilePathW::CountCompare(const CFilePathW &path, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	// Compare each segment in the path until a difference is found
	for(auto itr1 = m_vecSegments.begin(), itr2 = path.m_vecSegments.begin(); itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			break;
		++nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CFilePathW::ReverseCountCompare(const CFilePathW &path, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	// Compare each segment in the path until a difference is found
	for(auto itr1 = m_vecSegments.rbegin(), itr2 = path.m_vecSegments.rbegin(); itr1 != m_vecSegments.rend() && itr2 != path.m_vecSegments.rend(); ++itr1, ++itr2)
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			break;
		++nCount;
	}

	return nCount;
}

//#################################################################################################
void CFilePathW::Empty(void)
{
	m_eType = EPT_Empty;
	m_vecSegments.clear();
}

//#################################################################################################
bool CFilePathW::SetAnchor(const bool bAnchor)
{
	bool bReturn = false;

#ifdef _WIN32
	if(bAnchor && m_eType == EPT_DriveRelative)
	{
		m_eType = EPT_FullyRooted;
		bReturn = true;
	}
	else if(bAnchor && m_eType == EPT_Relative)
	{
		m_eType = EPT_CurrentDriveRelative;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_FullyRooted)
	{
		m_eType = EPT_DriveRelative;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_CurrentDriveRelative)
	{
		m_eType = EPT_Relative;
		bReturn = true;
	}
#else
	if(bAnchor && m_eType == EPT_Relative && !HasLeadingDots())
	{
		m_eType = EPT_FullyRooted;
		bReturn = true;
	}
	else if(bAnchor && m_eType == EPT_Empty)
	{
		m_eType = EPT_FullyRooted;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_FullyRooted && !m_vecSegments.empty())
	{
		m_eType = EPT_Relative;
		bReturn = true;
	}
	else if(!bAnchor && m_eType == EPT_FullyRooted)
	{
		m_eType = EPT_Empty;
		bReturn = true;
	}
#endif

	return bReturn;
}

//#################################################################################################
bool CFilePathW::SimplifyDots(void)
{
	bool bSimplified = false;

	if(!IsEmpty())
	{
		CFilePathW pathTmp(*this);

		size_t nSegment = 0;
		auto itr = pathTmp.m_vecSegments.begin();
		while(itr != pathTmp.m_vecSegments.end())
		{
			if(**itr == g_chPeriodW)
			{
				itr = pathTmp.m_vecSegments.erase(itr);
				bSimplified = true;
				continue;
			}
			else if(**itr == g_szDoublePeriodW)
			{
				if(itr == pathTmp.m_vecSegments.begin() || (IsDrive() && nSegment <= 1) || (IsUNC() && nSegment <= 2))
				{
					bSimplified = false;
					break;
				}

				bSimplified = true;
				auto itrPrev = itr;
				itr = pathTmp.m_vecSegments.erase(--itrPrev, ++itr);
				--nSegment;
				continue;
			}

			++itr;
			++nSegment;
		}

		if(bSimplified)
		{
			if(pathTmp.IsEmpty())
				Empty();
			else
				Assign(pathTmp);
		}
	}

	return bSimplified;
}

//#################################################################################################
bool CFilePathW::IsEmpty(void) const noexcept
{
	return (m_eType == EPT_Empty);
}

//#################################################################################################
bool CFilePathW::IsDrive(void) const noexcept
{
	return (m_eType == EPT_FullyRooted || m_eType == EPT_DriveRelative);
}

//#################################################################################################
bool CFilePathW::IsUNC(void) const noexcept
{
	return (m_eType == EPT_UNC);
}

//#################################################################################################
bool CFilePathW::IsFullyRooted(void) const noexcept
{
	return (m_eType == EPT_FullyRooted);
}

//#################################################################################################
bool CFilePathW::IsRelative(void) const noexcept
{
	return (m_eType == EPT_Relative || m_eType == EPT_DriveRelative || m_eType == EPT_CurrentDriveRelative);
}

//#################################################################################################
bool CFilePathW::IsRoot(void) const noexcept
{
#ifdef _WIN32
	return (m_eType == EPT_FullyRooted && m_vecSegments.size() == 1);
#else
	return (m_eType == EPT_FullyRooted && m_vecSegments.empty());
#endif
}

//#################################################################################################
bool CFilePathW::IsAnchored(void) const noexcept
{
	return (m_eType == EPT_FullyRooted || m_eType == EPT_CurrentDriveRelative);
}

//#################################################################################################
bool CFilePathW::HasLeadingDots(void) const
{
	return (!m_vecSegments.empty() && (*m_vecSegments.front() == g_chPeriodW || *m_vecSegments.front() == g_szDoublePeriodW));
}

//#################################################################################################
bool CFilePathW::HasExtension(void) const
{
	bool bExt = false;

	if(!m_vecSegments.empty())
	{
		CStrW str(m_vecSegments.back()->Get());
		if(str.Contains(g_chPeriodW))
			bExt = true;
	}

	return bExt;
}

//#################################################################################################
bool CFilePathW::HasWildcard(void) const
{
	return std::any_of(m_vecSegments.rbegin(), m_vecSegments.rend(), [](const std::unique_ptr<CFilePathSegmentW> &segment){return segment->HasWildcard();});
}

//#################################################################################################
size_t CFilePathW::GetSegmentCount(void) const noexcept
{
	return m_vecSegments.size();
}

//#################################################################################################
CFilePathSegmentW CFilePathW::GetSegment(const size_t nSegment) const
{
	CFilePathSegmentW path;

	if(nSegment < m_vecSegments.size())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(nCount-- != 0)
			++itr;

		path.Assign(**itr);
	}

	return path;
}

//#################################################################################################
CFilePathSegmentW CFilePathW::GetFirstSegment(void) const
{
	return !m_vecSegments.empty() ? CFilePathSegmentW(*m_vecSegments.front()) : CFilePathSegmentW();
}

//#################################################################################################
CFilePathSegmentW CFilePathW::GetLastSegment(void) const
{
	return !m_vecSegments.empty() ? CFilePathSegmentW(*m_vecSegments.back()) : CFilePathSegmentW();
}

//#################################################################################################
CFilePathW CFilePathW::GetLeft(const size_t nSegmentCount) const
{
	CFilePathW path(*this);
	path.CropLeft(nSegmentCount);

	return path;
}

//#################################################################################################
CFilePathW CFilePathW::GetRight(const size_t nSegmentCount) const
{
	CFilePathW path(*this);
	path.CropRight(nSegmentCount);

	return path;
}

//#################################################################################################
CFilePathW CFilePathW::GetMid(const size_t nSegment, const size_t nSegmentCount) const
{
	CFilePathW path;

	if(nSegment < m_vecSegments.size())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(itr != m_vecSegments.end() && nCount-- != 0)
			++itr;

		nCount = nSegmentCount;
		while(itr != m_vecSegments.end() && nCount-- != 0)
		{
			path.Append(**itr);
			++itr;
		}

		if(nSegment == 0 && nSegmentCount)
			path.m_eType = m_eType;
	}

	return path;
}

//#################################################################################################
CFilePathW &CFilePathW::TrimLeft(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{	// If it was a drive, UNC, or non-relative path it no longer is because we are removing the first segment
			m_eType = EPT_Relative;

			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::TrimRight(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{
			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecSegments.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::CropLeft(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecSegments.size())
	{
		if(nSegmentCount == 0)
			Empty();
		else
		{
			size_t nToRemove = m_vecSegments.size() - nSegmentCount;
			while(nToRemove--)
				m_vecSegments.pop_back();
		}
	}

	return *this;
}

//#################################################################################################
CFilePathW &CFilePathW::CropRight(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecSegments.size())
	{
		if(nSegmentCount == 0)
			Empty();
		else
		{	// If it was a drive, UNC, or non-relative path it no longer is because we are removing the first segment
			m_eType = EPT_Relative;

			size_t nToRemove = m_vecSegments.size() - nSegmentCount;
			while(nToRemove--)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CFilePathW::Insert(const size_t nSegment, const CFilePathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if((nSegment == 0 && (IsDrive() || IsUNC())) || (nSegment && (path.IsDrive() || path.IsUNC())))
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.insert(itr, std::make_unique<CFilePathSegmentW>(*segment));

			if(nSegment == 0)
				m_eType = path.m_eType;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Insert(const size_t nSegment, const CFilePathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if((nSegment == 0 && (IsDrive() || IsUNC())) || (nSegment && path.IsDrive()))
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(IsEmpty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			m_vecSegments.insert(itr, std::make_unique<CFilePathSegmentW>(path));

			if(nSegment == 0)
				m_eType = path.IsDrive() ? EPT_FullyRooted : EPT_Relative;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CFilePathW::Insert(const CMachineNameW &machine)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!machine.IsEmpty())
	{
		if(IsDrive() || IsUNC())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_eType = EPT_UNC;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CMachineNameW>(machine));
		}
	}

	return nErrorCode;
}

//#################################################################################################
CFilePathW &CFilePathW::Delete(const size_t nSegment, const size_t nSegmentCount)
{
	if(nSegment < m_vecSegments.size() && nSegmentCount != 0)
	{
		if(nSegment == 0 && nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			nCount = nSegmentCount;
			while(itr != m_vecSegments.end() && nCount-- != 0)
				itr = m_vecSegments.erase(itr);

			if(nSegment == 0)
				m_eType = EPT_Relative;
		}
	}

	return *this;
}

//#################################################################################################
size_t CFilePathW::Remove(const CFilePathSegmentW &path, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecSegments.begin();
	while(itr != m_vecSegments.end())
	{
		if((*itr)->Compare(path, bCaseInsensitive) == 0)
		{
			if(itr == m_vecSegments.begin())
				m_eType = EPT_Relative;

			itr = m_vecSegments.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	if(nCount && m_vecSegments.empty())
		m_eType = EPT_Empty;

	return nCount;
}

//#################################################################################################
size_t CFilePathW::Replace(const CFilePathSegmentW &pathOld, const CFilePathSegmentW &pathNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!pathOld.IsEmpty() && !pathNew.IsEmpty())
	{
		for(auto itr = m_vecSegments.begin(); itr != m_vecSegments.end(); ++itr)
		{
			if((*itr)->Compare(pathOld, bCaseInsensitive) == 0)
			{
				if(itr == m_vecSegments.begin())
				{
					if(pathNew.IsDrive())
						m_eType = EPT_FullyRooted;
					else if(pathOld.IsDrive())
						m_eType = EPT_Relative;

					*itr = std::make_unique<CFilePathSegmentW>(pathNew);
					++nCount;
				}
				else if(!pathNew.IsDrive())
				{
					*itr = std::make_unique<CFilePathSegmentW>(pathNew);
					++nCount;
				}
			}
		}
	}

	return nCount;
}

//#################################################################################################
CFilePathW &CFilePathW::Modify(const size_t nSegment, const CFilePathSegmentW &path)
{
	if(nSegment < m_vecSegments.size() && !path.IsEmpty())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(nCount-- != 0)
			++itr;

		*itr = std::make_unique<CFilePathSegmentW>(path);
		if(nSegment == 0)
		{
			if(path.IsDrive())
				m_eType = EPT_FullyRooted;
			else if(IsDrive())
				m_eType = EPT_Relative;
		}
	}

	return *this;
}

//#################################################################################################
CMachineNameW CFilePathW::GetMachine(void) const
{
	return IsUNC() ? CMachineNameW(*m_vecSegments.front()) : CMachineNameW();
}

//#################################################################################################
CFilePathSegmentW CFilePathW::GetShare(void) const
{
	CFilePathSegmentW path;

	if(IsUNC() && m_vecSegments.size() >= 2)
	{
		auto itr = m_vecSegments.begin();
		++itr;

		path.Assign(**itr);
	}

	return path;
}

//#################################################################################################
CFilePathW CFilePathW::GetMachineAndShare(void) const
{
	CFilePathW path;

	if(IsUNC() && m_vecSegments.size() >= 2)
	{
		auto itr = m_vecSegments.begin();
		path.m_vecSegments.push_back(std::make_unique<CMachineNameW>(**itr));
		++itr;
		path.m_vecSegments.push_back(std::make_unique<CFilePathSegmentW>(**itr));
		path.m_eType = EPT_UNC;
	}

	return path;
}

//#################################################################################################
CFilePathSegmentW CFilePathW::GetDrive(void) const
{
	return IsDrive() ? CFilePathSegmentW(*m_vecSegments.front()) : CFilePathSegmentW();
}

//#################################################################################################
CFilePathW CFilePathW::GetPath(void) const
{
	CFilePathW path(*this);

	if((!path.IsDrive() || path.m_vecSegments.size() > 1) && (!path.IsUNC() || path.m_vecSegments.size() > 2))
		path.TrimRight();

	return path;
}

//#################################################################################################
CFilePathW CFilePathW::GetRelativePath(void) const
{
	CFilePathW path(*this);

	if(path.IsDrive())
		path.TrimLeft();
	else if(path.IsUNC())
		path.TrimLeft(2);

	return path;
}

//#################################################################################################
CFilePathSegmentW CFilePathW::GetFilename(void) const
{	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	return ((IsDrive() && m_vecSegments.size() > 1) || (IsUNC() && m_vecSegments.size() > 2) || (!IsDrive() && !IsUNC() && !m_vecSegments.empty())) ? CFilePathSegmentW(*m_vecSegments.back()) : CFilePathSegmentW();
}

//#################################################################################################
CStrW CFilePathW::GetBase(void) const
{
	CStrW str;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		str.Assign(m_vecSegments.back()->GetBase());

	return str;
}

//#################################################################################################
CStrW CFilePathW::GetExtension(void) const
{
	CStrW str;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		str.Assign(m_vecSegments.back()->GetExtension());

	return str;
}

//#################################################################################################
bool CFilePathW::SetBase(const CStrW &strBase)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetBase(strBase);

	return bSet;
}

//#################################################################################################
bool CFilePathW::SetBase(PCWSTR szBase)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetBase(szBase);

	return bSet;
}

//#################################################################################################
bool CFilePathW::SetExtension(const CStrW &strExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetExtension(strExt);

	return bSet;
}

//#################################################################################################
bool CFilePathW::SetExtension(PCWSTR szExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->SetExtension(szExt);

	return bSet;
}

//#################################################################################################
bool CFilePathW::AddExtension(const CStrW &strExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->AddExtension(strExt);

	return bSet;
}

//#################################################################################################
bool CFilePathW::AddExtension(PCWSTR szExt)
{
	bool bSet = false;

	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		bSet = m_vecSegments.back()->AddExtension(szExt);

	return bSet;
}

//#################################################################################################
void CFilePathW::RemoveExtension(void)
{	// If it is a drive path there needs to be at least 1 segment and UNC paths there needs to be at least 2 segments.
	if(!m_vecSegments.empty() && (!IsDrive() || m_vecSegments.size() > 1) && (!IsUNC() || m_vecSegments.size() > 2))
		m_vecSegments.back()->RemoveExtension();
}

//#################################################################################################
void CFilePathW::DebugPrint(const bool bAppendNewline) const
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

#ifdef _WIN32
//#################################################################################################
bool CFilePathW::SplitDrivePath(PCWSTR szPath, CStrW &strDrive, CStrW &strPath, CStrW &strFileBase, CStrW &strFileExt)
{
	bool bValid = false;

	strDrive.Empty();
	strPath.Empty();
	strFileBase.Empty();
	strFileExt.Empty();

	CFilePathW path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strDrive.Assign(path.GetDrive());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFileBase.Assign(path.GetBase());
		strFileExt.Assign(path.GetExtension());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePathW::SplitDrivePath(PCWSTR szPath, CStrW &strDrive, CStrW &strPath, CStrW &strFilename)
{
	bool bValid = false;

	strDrive.Empty();
	strPath.Empty();
	strFilename.Empty();

	CFilePathW path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strDrive.Assign(path.GetDrive());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFilename.Assign(path.GetFilename());
		bValid = true;
	}

	return bValid;
}
#endif

//#################################################################################################
bool CFilePathW::SplitUNCPath(PCWSTR szPath, CStrW &strMachine, CStrW &strShare, CStrW &strPath, CStrW &strFileBase, CStrW &strFileExt)
{
	bool bValid = false;

	strMachine.Empty();
	strShare.Empty();
	strPath.Empty();
	strFileBase.Empty();
	strFileExt.Empty();

	CFilePathW path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsUNC() || path.IsRelative()))
	{
		strMachine.Assign(path.GetMachine());
		strShare.Assign(path.GetShare());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFileBase.Assign(path.GetBase());
		strFileExt.Assign(path.GetExtension());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePathW::SplitUNCPath(PCWSTR szPath, CStrW &strMachine, CStrW &strShare, CStrW &strPath, CStrW &strFilename)
{
	bool bValid = false;

	strMachine.Empty();
	strShare.Empty();
	strPath.Empty();
	strFilename.Empty();

	CFilePathW path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsUNC() || path.IsRelative()))
	{
		strMachine.Assign(path.GetMachine());
		strShare.Assign(path.GetShare());
		strPath.Assign(path.GetRelativePath().TrimRight());
		strFilename.Assign(path.GetFilename());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePathW::SplitPath(PCWSTR szPath, CStrW &strPath, CStrW &strFileBase, CStrW &strFileExt)
{
	bool bValid = false;

	strPath.Empty();
	strFileBase.Empty();
	strFileExt.Empty();

	CFilePathW path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strPath.Assign(path.GetPath().TrimRight());
		strFileBase.Assign(path.GetBase());
		strFileExt.Assign(path.GetExtension());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePathW::SplitPath(PCWSTR szPath, CStrW &strPath, CStrW &strFilename)
{
	bool bValid = false;

	strPath.Empty();
	strFilename.Empty();

	CFilePathW path;
	if(path.Assign(szPath) == FW_NO_ERROR && (path.IsDrive() || path.IsRelative()))
	{
		strPath.Assign(path.GetPath().TrimRight());
		strFilename.Assign(path.GetFilename());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CFilePathW::IsValidPath(const CStrW &strPath)
{
	bool bValid = true;

	CFilePathW path;
	if(path.Assign(strPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CFilePathW::IsValidPath(PCWSTR szPath)
{
	bool bValid = true;

	CFilePathW path;
	if(path.Assign(szPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CFilePathW::IsValidPath(const wchar_t chPath)
{
	bool bValid = true;

	CFilePathW path;
	if(path.Assign(chPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
ERRCODE CFilePathW::ParsePath(const CStrW &strPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Empty();

	if(strPath.IsEmpty())
		return nErrorCode;

	PCWSTR szPath = strPath;
	PCWSTR szLastDelim = szPath;
	PCWSTR szNextDelim;
	size_t nDelimLen = strPath.GetLength();

#ifdef _WIN32
	// Is it a kernel UNC path?
	if(strPath.CompareLeft(g_szUNCKernelPrefixW, g_nUNCKernelPrefixLen, true) == 0)
	{
		m_eType = EPT_UNC;
		szLastDelim += g_nUNCKernelPrefixLen;
		nDelimLen -= g_nUNCKernelPrefixLen;
	}
	else if(strPath.CompareLeft(g_szKernelPrefixW, g_nKernelPrefixLen) == 0)
	{	// Is it a kernel path?
		if(strPath.GetLength() >= g_nKernelPrefixLen + 2 && szPath[g_nKernelPrefixLen + 1] == g_chColonW)
		{
			if(strPath.GetLength() == g_nKernelPrefixLen + 2 || szPath[g_nKernelPrefixLen + 2] != g_chDelimiterW)
				m_eType = EPT_DriveRelative;
			else
				m_eType = EPT_FullyRooted;

			szLastDelim += g_nKernelPrefixLen;
			nDelimLen -= g_nKernelPrefixLen;
		}
		else
			return FW_ERROR_INVALID_DATA;
	}
	else if(szPath[1] == g_chColonW)
	{	// Is it a drive path?
		if(strPath.GetLength() == 2 || szPath[2] != g_chDelimiterW)
			m_eType = EPT_DriveRelative;
		else
			m_eType = EPT_FullyRooted;
	}
	else
#endif
	{
		if(szPath[0] == g_chDelimiterW && szPath[1] == g_chDelimiterW)
		{	// Is it a UNC path?
			m_eType = EPT_UNC;
			szLastDelim += 2;
			nDelimLen -= 2;
		}
		else if(szPath[0] == g_chDelimiterW)
		{
#ifdef _WIN32
			m_eType = EPT_CurrentDriveRelative;
#else
			m_eType = EPT_FullyRooted;
#endif
			++szLastDelim;
			--nDelimLen;
		}
		else
			m_eType = EPT_Relative;
	}

	do{
		szNextDelim = (PCWSTR)std::wmemchr(szLastDelim, g_chDelimiterW, nDelimLen);
		if(szNextDelim == nullptr)
		{	// No more delimiters, the rest of the string is the last segment
			if(IsUNC() && m_vecSegments.empty())
			{
				auto pMachine = std::make_unique<CMachineNameW>(szLastDelim);
				if(pMachine->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pMachine));
			}
#ifdef _WIN32
			else if(m_vecSegments.empty() && nDelimLen >= 3 && szLastDelim[1] == g_chColonW && szLastDelim[2] != g_chDelimiterW)
			{	// Special case paths like "c:temp" - this is actually two segments without a delimiter
				auto pPath = std::make_unique<CFilePathSegmentW>(strPath.GetMid((size_t)(szLastDelim - szPath), 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));

				pPath = std::make_unique<CFilePathSegmentW>(strPath.GetMid((size_t)(szLastDelim - szPath) + 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}
#endif
			else if(*szLastDelim == g_chNullW)
				break;
			else
			{
				auto pPath = std::make_unique<CFilePathSegmentW>(szLastDelim);
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}
		}
		else
		{
			if(IsUNC() && m_vecSegments.empty())
			{
				auto pMachine = std::make_unique<CMachineNameW>(strPath.GetMid((size_t)(szLastDelim - szPath), (size_t)(szNextDelim - szLastDelim)));
				if(pMachine->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pMachine));
			}
#ifdef _WIN32
			else if(m_vecSegments.empty() && nDelimLen >= 3 && szLastDelim[1] == g_chColonW && szLastDelim[2] != g_chDelimiterW)
			{	// Special case paths like "c:temp" - this is actually two segments without a delimiter
				auto pPath = std::make_unique<CFilePathSegmentW>(strPath.GetMid((size_t)(szLastDelim - szPath), 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));

				pPath = std::make_unique<CFilePathSegmentW>(strPath.GetMid((size_t)(szLastDelim - szPath) + 2, (size_t)(szNextDelim - szLastDelim) - 2));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}
#endif
			else
			{
				auto pPath = std::make_unique<CFilePathSegmentW>(strPath.GetMid((size_t)(szLastDelim - szPath), (size_t)(szNextDelim - szLastDelim)));
				if(pPath->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pPath));
			}

			nDelimLen -= (size_t)(szNextDelim - szLastDelim) + 1;
			szLastDelim = szNextDelim + 1;
		}
	}while(szNextDelim && *szLastDelim != g_chNullW);

	return nErrorCode;
}

//#################################################################################################
bool CFilePathW::SafeDotAppend(const CFilePathW &path1, const CFilePathW &path2)
{
	bool bAppended = false;

	bool bValidAppend = true;
	CFilePathW pathTmp1(path1);
	CFilePathW pathTmp2(path2);

	while(!pathTmp2.m_vecSegments.empty())
	{
		const CFilePathSegmentW pathSeg(*pathTmp2.m_vecSegments.front());
		if(pathSeg == g_chPeriodW)
			pathTmp2.TrimLeft();
		else if(pathSeg == g_szDoublePeriodW)
		{
			if((pathTmp1.IsDrive() && pathTmp1.m_vecSegments.size() == 1) || (pathTmp1.IsUNC() && pathTmp1.m_vecSegments.size() == 2) || pathTmp1.IsEmpty())
			{
				bValidAppend = false;
				break;
			}

			pathTmp1.TrimRight();
			pathTmp2.TrimLeft();
		}
		else
			break;
	}

	if(bValidAppend)
	{
		Assign(pathTmp1);
		Append(pathTmp2);
		bAppended = true;
	}

	return bAppended;
}

//#################################################################################################
bool CFilePathW::SafeDotAppend(const CFilePathW &path1, const CFilePath8 &path2)
{
	bool bAppended = false;

	bool bValidAppend = true;
	CFilePathW pathTmp1(path1);
	CFilePath8 pathTmp2(path2);

	while(!pathTmp2.m_vecSegments.empty())
	{
		const CFilePathSegment8 pathSeg(*pathTmp2.m_vecSegments.front());
		if(pathSeg == g_chPeriod8)
			pathTmp2.TrimLeft();
		else if(pathSeg == g_szDoublePeriod8)
		{
			if((pathTmp1.IsDrive() && pathTmp1.m_vecSegments.size() == 1) || (pathTmp1.IsUNC() && pathTmp1.m_vecSegments.size() == 2) || pathTmp1.IsEmpty())
			{
				bValidAppend = false;
				break;
			}

			pathTmp1.TrimRight();
			pathTmp2.TrimLeft();
		}
		else
			break;
	}

	if(bValidAppend)
	{
		Assign(pathTmp1);
		Append(pathTmp2);
		bAppended = true;
	}

	return bAppended;
}

NS_END

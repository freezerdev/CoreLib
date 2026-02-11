#include "Base.h"
#include "RegPath.h"

NS_BEGIN

static constexpr char g_chBackSlash8 = '\\';
static constexpr wchar_t g_chBackSlashW = L'\\';

static PCSTR g_szHKCR8 = "HKEY_CLASSES_ROOT";
static PCWSTR g_szHKCRW = L"HKEY_CLASSES_ROOT";
static constexpr size_t g_nHKCRLen = 17;
static PCSTR g_szHKCRShort8 = "HKCR";
static PCWSTR g_szHKCRShortW = L"HKCR";
static constexpr size_t g_nHKCRShortLen = 4;
static PCSTR g_szHKCU8 = "HKEY_CURRENT_USER";
static PCWSTR g_szHKCUW = L"HKEY_CURRENT_USER";
static constexpr size_t g_nHKCULen = 17;
static PCSTR g_szHKCUShort8 = "HKCU";
static PCWSTR g_szHKCUShortW = L"HKCU";
static constexpr size_t g_nHKCUShortLen = 4;
static PCSTR g_szHKLM8 = "HKEY_LOCAL_MACHINE";
static PCWSTR g_szHKLMW = L"HKEY_LOCAL_MACHINE";
static constexpr size_t g_nHKLMLen = 18;
static PCSTR g_szHKLMShort8 = "HKLM";
static PCWSTR g_szHKLMShortW = L"HKLM";
static constexpr size_t g_nHKLMShortLen = 4;
static PCSTR g_szHKU8 = "HKEY_USERS";
static PCWSTR g_szHKUW = L"HKEY_USERS";
static constexpr size_t g_nHKULen = 10;
static PCSTR g_szHKUShort8 = "HKU";
static PCWSTR g_szHKUShortW = L"HKU";
static constexpr size_t g_nHKUShortLen = 3;
static PCSTR g_szHKCC8 = "HKEY_CURRENT_CONFIG";
static PCWSTR g_szHKCCW = L"HKEY_CURRENT_CONFIG";
static constexpr size_t g_nHKCCLen = 19;
static PCSTR g_szHKCCShort8 = "HKCC";
static PCWSTR g_szHKCCShortW = L"HKCC";
static constexpr size_t g_nHKCCShortLen = 4;
static PCSTR g_szRegKernelMachine8 = "\\REGISTRY\\MACHINE";
static PCWSTR g_szRegKernelMachineW = L"\\REGISTRY\\MACHINE";
static constexpr size_t g_nKernelMachineLen = 17;
static PCSTR g_szRegKernelUser8 = "\\REGISTRY\\USER";
static PCWSTR g_szRegKernelUserW = L"\\REGISTRY\\USER";
static constexpr size_t g_nKernelUserLen = 14;


/////////////////////////////////////////////////
// CRegPathSegment8
/////////////////////////////////////////////////

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(void)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(const CRegPathSegment8 &src)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(CRegPathSegment8 &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PSTR)g_szNull8;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(const CRegPathSegmentW &path)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(path);
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(const CStr8 &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(const CStrW &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(PCSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(PCWSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(const char ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CRegPathSegment8::CRegPathSegment8(const wchar_t ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CRegPathSegment8::~CRegPathSegment8(void)
{
	Dealloc();
}

//#################################################################################################
CRegPathSegment8::operator PCSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCSTR CRegPathSegment8::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CRegPathSegment8::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CRegPathSegment8::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
void CRegPathSegment8::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CRegPathSegment8::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
CRegPathSegment8 CRegPathSegment8::AsUtf8(void) const
{
	return *this;
}

//#################################################################################################
CRegPathSegmentW CRegPathSegment8::AsWide(void) const
{
	return CRegPathSegmentW(*this);
}

//#################################################################################################
CRegPathSegmentW CRegPathSegment8::AsNative(void) const
{
	return CRegPathSegmentW(*this);
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(const CRegPathSegment8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(CRegPathSegment8 &&src) noexcept
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
CRegPathSegment8 &CRegPathSegment8::operator=(const CRegPathSegmentW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CRegPathSegment8 &CRegPathSegment8::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CRegPathSegment8::Assign(const CRegPathSegment8 &path)
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
ERRCODE CRegPathSegment8::Assign(const CRegPathSegmentW &path)
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
ERRCODE CRegPathSegment8::Assign(const CStr8 &str)
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
ERRCODE CRegPathSegment8::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CRegPathSegmentW::IsValidSegment(str))
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
ERRCODE CRegPathSegment8::Assign(PCSTR sz)
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
ERRCODE CRegPathSegment8::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CRegPathSegmentW::IsValidSegment(sz))
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
ERRCODE CRegPathSegment8::Assign(const char ch)
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
ERRCODE CRegPathSegment8::Assign(const wchar_t ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CRegPathSegmentW::IsValidSegment(ch))
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
std::ostream &operator<<(std::ostream &stream, const CRegPathSegment8 &path)
{
	stream << path.m_sz;
	return stream;
}

//#################################################################################################
bool CRegPathSegment8::operator==(const CRegPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) == 0);
}

//#################################################################################################
bool CRegPathSegment8::operator==(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) == 0);
}

//#################################################################################################
bool CRegPathSegment8::operator==(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) == 0);
}

//#################################################################################################
bool CRegPathSegment8::operator==(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) == 0);
}

//#################################################################################################
bool CRegPathSegment8::operator!=(const CRegPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) != 0);
}

//#################################################################################################
bool CRegPathSegment8::operator!=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) != 0);
}

//#################################################################################################
bool CRegPathSegment8::operator!=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) != 0);
}

//#################################################################################################
bool CRegPathSegment8::operator!=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) != 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<(const CRegPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) < 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) < 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) < 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) < 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<=(const CRegPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) <= 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) <= 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) <= 0);
}

//#################################################################################################
bool CRegPathSegment8::operator<=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) <= 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>(const CRegPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) > 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) > 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) > 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) > 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>=(const CRegPathSegment8 &path) const
{
	return (StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) >= 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true) >= 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), true) >= 0);
}

//#################################################################################################
bool CRegPathSegment8::operator>=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1, true) >= 0);
}

//#################################################################################################
int CRegPathSegment8::Compare(const CRegPathSegment8 &path, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CRegPathSegment8::Compare(const CStr8 &str, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CRegPathSegment8::Compare(PCSTR sz, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CRegPathSegment8::Compare(const char ch, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
bool CRegPathSegment8::IsValidSegment(const CStr8 &str)
{	// Valid Registry path segments are of length 1 or greater and can contain any character except '\'
	return (!str.IsEmpty() && !str.Contains(g_chBackSlash8));
}

//#################################################################################################
bool CRegPathSegment8::IsValidSegment(PCSTR szSegment)
{	// Valid Registry path segments are of length 1 or greater and can contain any character except '\'

	// Cannot be nullptr
	if(szSegment == nullptr)
		return false;

	// Length greater than 0
	size_t nStrLen = StringGetLength(szSegment);
	if(nStrLen == 0)
		return false;

	// Does it contain an invalid character?
	if(std::memchr(szSegment, g_chBackSlash8, nStrLen))
		return false;

	return true;
}

//#################################################################################################
bool CRegPathSegment8::IsValidSegment(const char chSegment)
{	// Valid Registry path segments are of length 1 or greater and can contain any character except '\'
	// Cannot be nullptr or '\'
	return (chSegment != g_chNull8 && chSegment != g_chBackSlash8);
}

//#################################################################################################
bool CRegPathSegment8::Alloc(const size_t nBufLen)
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
void CRegPathSegment8::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PSTR)g_szNull8;
	m_nStrLen = m_nBufLen = 0;
}




/////////////////////////////////////////////////
// CRegPathSegmentW
/////////////////////////////////////////////////

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(void)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(const CRegPathSegmentW &src)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(CRegPathSegmentW &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PWSTR)g_szNullW;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(const CRegPathSegment8 &path)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(path);
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(const CStrW &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(const CStr8 &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(PCWSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(PCSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(const wchar_t ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CRegPathSegmentW::CRegPathSegmentW(const char ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CRegPathSegmentW::~CRegPathSegmentW(void)
{
	Dealloc();
}

//#################################################################################################
CRegPathSegmentW::operator PCWSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
PCWSTR CRegPathSegmentW::Get(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
size_t CRegPathSegmentW::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CRegPathSegmentW::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen) * sizeof(wchar_t);
}

//#################################################################################################
void CRegPathSegmentW::Empty(void)
{
	Dealloc();
}

//#################################################################################################
bool CRegPathSegmentW::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
CRegPathSegment8 CRegPathSegmentW::AsUtf8(void) const
{
	return CRegPathSegment8(*this);
}

//#################################################################################################
CRegPathSegmentW CRegPathSegmentW::AsWide(void) const
{
	return *this;
}

//#################################################################################################
CRegPathSegmentW CRegPathSegmentW::AsNative(void) const
{
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(const CRegPathSegmentW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(CRegPathSegmentW &&src) noexcept
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
CRegPathSegmentW &CRegPathSegmentW::operator=(const CRegPathSegment8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CRegPathSegmentW &CRegPathSegmentW::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
ERRCODE CRegPathSegmentW::Assign(const CRegPathSegmentW &path)
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
ERRCODE CRegPathSegmentW::Assign(const CRegPathSegment8 &path)
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
ERRCODE CRegPathSegmentW::Assign(const CStrW &str)
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
ERRCODE CRegPathSegmentW::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CRegPathSegment8::IsValidSegment(str))
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
ERRCODE CRegPathSegmentW::Assign(PCWSTR sz)
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
ERRCODE CRegPathSegmentW::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CRegPathSegment8::IsValidSegment(sz))
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
ERRCODE CRegPathSegmentW::Assign(const wchar_t ch)
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
ERRCODE CRegPathSegmentW::Assign(const char ch)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CRegPathSegment8::IsValidSegment(ch))
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
std::wostream &operator<<(std::wostream &stream, const CRegPathSegmentW &path)
{
	stream << path.m_sz;
	return stream;
}

//#################################################################################################
bool CRegPathSegmentW::operator==(const CRegPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) == 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator==(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) == 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator==(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) == 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator==(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) == 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator!=(const CRegPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) != 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator!=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) != 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator!=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) != 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator!=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) != 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<(const CRegPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) < 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) < 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) < 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) < 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<=(const CRegPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) <= 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) <= 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) <= 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator<=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) <= 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>(const CRegPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) > 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) > 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) > 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) > 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>=(const CRegPathSegmentW &path) const
{
	return (StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, true) >= 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), true) >= 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), true) >= 0);
}

//#################################################################################################
bool CRegPathSegmentW::operator>=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1, true) >= 0);
}

//#################################################################################################
int CRegPathSegmentW::Compare(const CRegPathSegmentW &path, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, path.m_sz, path.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CRegPathSegmentW::Compare(const CStrW &str, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, str, str.GetLength(), bCaseInsensitive);
}

//#################################################################################################
int CRegPathSegmentW::Compare(PCWSTR sz, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CRegPathSegmentW::Compare(const wchar_t ch, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
bool CRegPathSegmentW::IsValidSegment(const CStrW &str)
{	// Valid Registry path segments are of length 1 or greater and can contain any character except '\'
	return (!str.IsEmpty() && !str.Contains(g_chBackSlashW));
}

//#################################################################################################
bool CRegPathSegmentW::IsValidSegment(PCWSTR szSegment)
{	// Valid Registry path segments are of length 1 or greater and can contain any character except '\'

	// Cannot be nullptr
	if(szSegment == nullptr)
		return false;

	// Length greater than 0
	size_t nStrLen = StringGetLength(szSegment);
	if(nStrLen == 0)
		return false;

	// Does it contain an invalid character?
	if(std::wmemchr(szSegment, g_chBackSlashW, nStrLen))
		return false;

	return true;
}

//#################################################################################################
bool CRegPathSegmentW::IsValidSegment(const wchar_t chSegment)
{	// Valid Registry path segments are of length 1 or greater and can contain any character except '\'
	// Cannot be nullptr or '\'
	return (chSegment != g_chNullW && chSegment != g_chBackSlashW);
}

//#################################################################################################
bool CRegPathSegmentW::Alloc(const size_t nBufLen)
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
void CRegPathSegmentW::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PWSTR)g_szNullW;
	m_nStrLen = m_nBufLen = 0;
}




/////////////////////////////////////////////////
// CHiveName8
/////////////////////////////////////////////////

//#################################################################################################
CHiveName8::CHiveName8(const CHiveNameW &hive)
{
	Assign(hive);
}

//#################################################################################################
CHiveName8::CHiveName8(const CStr8 &str)
{
	Assign(str);
}

//#################################################################################################
CHiveName8::CHiveName8(const CStrW &str)
{
	Assign(str);
}

//#################################################################################################
CHiveName8::CHiveName8(PCSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CHiveName8::CHiveName8(PCWSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CHiveName8::CHiveName8(const HKEY hHive, const bool bShort)
{
	Assign(hHive, bShort);
}

//#################################################################################################
CHiveName8::operator HKEY(void) const
{
	return GetHive();
}

//#################################################################################################
PCSTR CHiveName8::GetLongName(void) const
{
	PCSTR szHive = nullptr;

	if(StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCR8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCU8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLM8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKU8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCC8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetShortName(void) const
{
	PCSTR szHive = nullptr;

	if(StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShort8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShort8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShort8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShort8;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShort8;

	return szHive;
}

//#################################################################################################
HKEY CHiveName8::GetHive(void) const
{
	HKEY hHive = nullptr;

	if(StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		hHive = HKEY_CLASSES_ROOT;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		hHive = HKEY_CURRENT_USER;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		hHive = HKEY_LOCAL_MACHINE;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		hHive = HKEY_USERS;
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		hHive = HKEY_CURRENT_CONFIG;

	return hHive;
}

//#################################################################################################
CHiveName8 &CHiveName8::operator=(const CHiveNameW &hive)
{
	Assign(hive);
	return *this;
}

//#################################################################################################
CHiveName8 &CHiveName8::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CHiveName8 &CHiveName8::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CHiveName8 &CHiveName8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CHiveName8 &CHiveName8::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CHiveName8 &CHiveName8::operator=(const HKEY hHive)
{
	Assign(hHive);
	return *this;
}

//#################################################################################################
ERRCODE CHiveName8::Assign(const CHiveName8 &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &hive)
	{
		if(hive.m_nStrLen)
		{
			if(Alloc(hive.m_nStrLen + 1))
			{
				std::memcpy(m_sz, hive.m_sz, hive.m_nStrLen);
				m_nStrLen = hive.m_nStrLen;
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
ERRCODE CHiveName8::Assign(const CHiveNameW &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(hive.m_nStrLen)
	{
		if(Alloc(WideToUtf8(hive.m_sz, hive.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(hive.m_sz, hive.m_nStrLen, m_sz, m_nBufLen);
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
ERRCODE CHiveName8::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidHiveName(str))
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
ERRCODE CHiveName8::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CHiveNameW::IsValidHiveName(str))
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
ERRCODE CHiveName8::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidHiveName(sz))
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
ERRCODE CHiveName8::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CHiveNameW::IsValidHiveName(sz))
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
ERRCODE CHiveName8::Assign(const HKEY hHive, const bool bShort)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	PCSTR szHive = nullptr;
	size_t nHiveLen = 0;

	if(hHive == HKEY_CLASSES_ROOT)
	{
		if(bShort)
		{
			szHive = g_szHKCRShort8;
			nHiveLen = g_nHKCRShortLen;
		}
		else
		{
			szHive = g_szHKCR8;
			nHiveLen = g_nHKCRLen;
		}
	}
	else if(hHive == HKEY_CURRENT_USER)
	{
		if(bShort)
		{
			szHive = g_szHKCUShort8;
			nHiveLen = g_nHKCUShortLen;
		}
		else
		{
			szHive = g_szHKCU8;
			nHiveLen = g_nHKCULen;
		}
	}
	else if(hHive == HKEY_LOCAL_MACHINE)
	{
		if(bShort)
		{
			szHive = g_szHKLMShort8;
			nHiveLen = g_nHKLMShortLen;
		}
		else
		{
			szHive = g_szHKLM8;
			nHiveLen = g_nHKLMLen;
		}
	}
	else if(hHive == HKEY_USERS)
	{
		if(bShort)
		{
			szHive = g_szHKUShort8;
			nHiveLen = g_nHKUShortLen;
		}
		else
		{
			szHive = g_szHKU8;
			nHiveLen = g_nHKULen;
		}
	}
	else if(hHive == HKEY_CURRENT_CONFIG)
	{
		if(bShort)
		{
			szHive = g_szHKCCShort8;
			nHiveLen = g_nHKCCShortLen;
		}
		else
		{
			szHive = g_szHKCC8;
			nHiveLen = g_nHKCCLen;
		}
	}

	if(szHive)
	{
		if(Alloc(nHiveLen + 1))
		{
			std::memcpy(m_sz, szHive, nHiveLen);
			m_nStrLen = nHiveLen;
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
bool CHiveName8::operator==(const CHiveName8 &hive) const
{
	return (Compare(hive) == 0);
}

//#################################################################################################
bool CHiveName8::operator==(const CStr8 &str) const
{
	return (Compare(str) == 0);
}

//#################################################################################################
bool CHiveName8::operator==(PCSTR sz) const
{
	return (Compare(sz) == 0);
}

//#################################################################################################
bool CHiveName8::operator==(const HKEY hHive) const
{
	return (Compare(hHive) == 0);
}

//#################################################################################################
bool CHiveName8::operator!=(const CHiveName8 &hive) const
{
	return (Compare(hive) != 0);
}

//#################################################################################################
bool CHiveName8::operator!=(const CStr8 &str) const
{
	return (Compare(str) != 0);
}

//#################################################################################################
bool CHiveName8::operator!=(PCSTR sz) const
{
	return (Compare(sz) != 0);
}

//#################################################################################################
bool CHiveName8::operator!=(const HKEY hHive) const
{
	return (Compare(hHive) != 0);
}

//#################################################################################################
bool CHiveName8::operator<(const CHiveName8 &hive) const
{
	return (Compare(hive) < 0);
}

//#################################################################################################
bool CHiveName8::operator<(const CStr8 &str) const
{
	return (Compare(str) < 0);
}

//#################################################################################################
bool CHiveName8::operator<(PCSTR sz) const
{
	return (Compare(sz) < 0);
}

//#################################################################################################
bool CHiveName8::operator<(const HKEY hHive) const
{
	return (Compare(hHive) < 0);
}

//#################################################################################################
bool CHiveName8::operator<=(const CHiveName8 &hive) const
{
	return (Compare(hive) <= 0);
}

//#################################################################################################
bool CHiveName8::operator<=(const CStr8 &str) const
{
	return (Compare(str) <= 0);
}

//#################################################################################################
bool CHiveName8::operator<=(PCSTR sz) const
{
	return (Compare(sz) <= 0);
}

//#################################################################################################
bool CHiveName8::operator<=(const HKEY hHive) const
{
	return (Compare(hHive) <= 0);
}

//#################################################################################################
bool CHiveName8::operator>(const CHiveName8 &hive) const
{
	return (Compare(hive) > 0);
}

//#################################################################################################
bool CHiveName8::operator>(const CStr8 &str) const
{
	return (Compare(str) > 0);
}

//#################################################################################################
bool CHiveName8::operator>(PCSTR sz) const
{
	return (Compare(sz) > 0);
}

//#################################################################################################
bool CHiveName8::operator>(const HKEY hHive) const
{
	return (Compare(hHive) > 0);
}

//#################################################################################################
bool CHiveName8::operator>=(const CHiveName8 &hive) const
{
	return (Compare(hive) >= 0);
}

//#################################################################################################
bool CHiveName8::operator>=(const CStr8 &str) const
{
	return (Compare(str) >= 0);
}

//#################################################################################################
bool CHiveName8::operator>=(PCSTR sz) const
{
	return (Compare(sz) >= 0);
}

//#################################################################################################
bool CHiveName8::operator>=(const HKEY hHive) const
{
	return (Compare(hHive) >= 0);
}

//#################################################################################################
int CHiveName8::Compare(const CHiveName8 &hive) const
{
	int nCompareRtn = 0;

	int n;

	if(StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0)
	{
		if((n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCR8, g_nHKCRLen, true)) != 0 && StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
	{
		if(StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCR8, g_nHKCRLen, true) != 0 && (n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true) == 0)
	{
		if((n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCU8, g_nHKCULen, true)) != 0 && StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
	{
		if(StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCU8, g_nHKCULen, true) != 0 && (n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0)
	{
		if((n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKLM8, g_nHKLMLen, true)) != 0 && StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
	{
		if(StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKLM8, g_nHKLMLen, true) != 0 && (n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true) == 0)
	{
		if((n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKU8, g_nHKULen, true)) != 0 && StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
	{
		if(StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKU8, g_nHKULen, true) != 0 && (n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0)
	{
		if((n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCC8, g_nHKCCLen, true)) != 0 && StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
	{
		if(StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCC8, g_nHKCCLen, true) != 0 && (n = StringCompare8(hive.m_sz, hive.m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else
		nCompareRtn = StringCompare8(m_sz, m_nStrLen, hive.m_sz, hive.m_nStrLen, true);

	return nCompareRtn;
}

//#################################################################################################
int CHiveName8::Compare(const CStr8 &str) const
{
	int nCompareRtn = 0;

	int n;

	if(StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0)
	{
		if((n = StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true)) != 0 && StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
	{
		if(StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true) != 0 && (n = StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true) == 0)
	{
		if((n = StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true)) != 0 && StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
	{
		if(StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true) != 0 && (n = StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0)
	{
		if((n = StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true)) != 0 && StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
	{
		if(StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true) != 0 && (n = StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true) == 0)
	{
		if((n = StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true)) != 0 && StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
	{
		if(StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true) != 0 && (n = StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0)
	{
		if((n = StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true)) != 0 && StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
	{
		if(StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true) != 0 && (n = StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else
		nCompareRtn = StringCompare8(m_sz, m_nStrLen, str, str.GetLength(), true);

	return nCompareRtn;
}

//#################################################################################################
int CHiveName8::Compare(PCSTR sz) const
{
	int nCompareRtn = 0;

	size_t nStrLen = StringGetLength(sz);
	int n;

	if(StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0)
	{
		if((n = StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true)) != 0 && StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
	{
		if(StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true) != 0 && (n = StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true) == 0)
	{
		if((n = StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true)) != 0 && StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
	{
		if(StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true) != 0 && (n = StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0)
	{
		if((n = StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true)) != 0 && StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
	{
		if(StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true) != 0 && (n = StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true) == 0)
	{
		if((n = StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true)) != 0 && StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
	{
		if(StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true) != 0 && (n = StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0)
	{
		if((n = StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true)) != 0 && StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
	{
		if(StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true) != 0 && (n = StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else
		nCompareRtn = StringCompare8(m_sz, m_nStrLen, sz, nStrLen, true);

	return nCompareRtn;
}

//#################################################################################################
int CHiveName8::Compare(const HKEY hHive) const
{
	int nCompareRtn = 0;

	if(StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0)
	{
		if(hHive != HKEY_CLASSES_ROOT)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKCR8, g_nHKCRLen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
	{
		if(hHive != HKEY_CLASSES_ROOT)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_USER)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKCU8, g_nHKCULen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_USER)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0)
	{
		if(hHive != HKEY_LOCAL_MACHINE)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKLM8, g_nHKLMLen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
	{
		if(hHive != HKEY_LOCAL_MACHINE)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true) == 0)
	{
		if(hHive != HKEY_USERS)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKU8, g_nHKULen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
	{
		if(hHive != HKEY_USERS)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKUShort8, g_nHKUShortLen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_CONFIG)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKCC8, g_nHKCCLen, true);
	}
	else if(StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_CONFIG)
			nCompareRtn = StringCompare8(m_sz, m_nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true);
	}
	else
	{
		if(hHive)
			nCompareRtn = -1;
	}

	return nCompareRtn;
}

//#################################################################################################
PCSTR CHiveName8::GetLongName(const CStr8 &str)
{
	PCSTR szHive = nullptr;

	if(StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCR8;
	else if(StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCU8;
	else if(StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLM8;
	else if(StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKU8;
	else if(StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCC8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetLongName(const CStrW &str)
{
	PCSTR szHive = nullptr;

	if(StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCR8;
	else if(StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCU8;
	else if(StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLM8;
	else if(StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKU8;
	else if(StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCC8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetLongName(PCSTR sz)
{
	PCSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCR8;
	else if(StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCU8;
	else if(StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLM8;
	else if(StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKU8;
	else if(StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCC8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetLongName(PCWSTR sz)
{
	PCSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCR8;
	else if(StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCU8;
	else if(StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLM8;
	else if(StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKU8;
	else if(StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCC8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetLongName(const HKEY hHive)
{
	PCSTR szHive = nullptr;

	if(hHive == HKEY_CLASSES_ROOT)
		szHive = g_szHKCR8;
	else if(hHive == HKEY_CURRENT_USER)
		szHive = g_szHKCU8;
	else if(hHive == HKEY_LOCAL_MACHINE)
		szHive = g_szHKLM8;
	else if(hHive == HKEY_USERS)
		szHive = g_szHKU8;
	else if(hHive == HKEY_CURRENT_CONFIG)
		szHive = g_szHKCC8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetShortName(const CStr8 &str)
{
	PCSTR szHive = nullptr;

	if(StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShort8;
	else if(StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShort8;
	else if(StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShort8;
	else if(StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShort8;
	else if(StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShort8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetShortName(const CStrW &str)
{
	PCSTR szHive = nullptr;

	if(StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShort8;
	else if(StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShort8;
	else if(StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShort8;
	else if(StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShort8;
	else if(StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShort8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetShortName(PCSTR sz)
{
	PCSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShort8;
	else if(StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShort8;
	else if(StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShort8;
	else if(StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShort8;
	else if(StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShort8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetShortName(PCWSTR sz)
{
	PCSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShort8;
	else if(StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShort8;
	else if(StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShort8;
	else if(StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShort8;
	else if(StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShort8;

	return szHive;
}

//#################################################################################################
PCSTR CHiveName8::GetShortName(const HKEY hHive)
{
	PCSTR szHive = nullptr;

	if(hHive == HKEY_CLASSES_ROOT)
		szHive = g_szHKCRShort8;
	else if(hHive == HKEY_CURRENT_USER)
		szHive = g_szHKCUShort8;
	else if(hHive == HKEY_LOCAL_MACHINE)
		szHive = g_szHKLMShort8;
	else if(hHive == HKEY_USERS)
		szHive = g_szHKUShort8;
	else if(hHive == HKEY_CURRENT_CONFIG)
		szHive = g_szHKCCShort8;

	return szHive;
}

//#################################################################################################
HKEY CHiveName8::GetHive(const CStr8 &str)
{
	HKEY hHive = nullptr;

	if(StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		hHive = HKEY_CLASSES_ROOT;
	else if(StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		hHive = HKEY_CURRENT_USER;
	else if(StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		hHive = HKEY_LOCAL_MACHINE;
	else if(StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true) == 0)
		hHive = HKEY_USERS;
	else if(StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		hHive = HKEY_CURRENT_CONFIG;

	return hHive;
}

//#################################################################################################
HKEY CHiveName8::GetHive(PCSTR sz)
{
	HKEY hHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		hHive = HKEY_CLASSES_ROOT;
	else if(StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		hHive = HKEY_CURRENT_USER;
	else if(StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		hHive = HKEY_LOCAL_MACHINE;
	else if(StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		hHive = HKEY_USERS;
	else if(StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		hHive = HKEY_CURRENT_CONFIG;

	return hHive;
}

//#################################################################################################
bool CHiveName8::IsValidHiveName(const CStr8 &str)
{
	return (StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true) == 0 ||
		StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true) == 0 ||
		StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true) == 0 ||
		StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true) == 0 ||
		StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true) == 0);
}

//#################################################################################################
bool CHiveName8::IsValidHiveName(PCSTR sz)
{
	size_t nStrLen = StringGetLength(sz);
	return (StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0 ||
		StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0 ||
		StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0 ||
		StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0 ||
		StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0);
}




/////////////////////////////////////////////////
// CHiveNameW
/////////////////////////////////////////////////

//#################################################################################################
CHiveNameW::CHiveNameW(const CHiveName8 &hive)
{
	Assign(hive);
}

//#################################################################################################
CHiveNameW::CHiveNameW(const CStrW &str)
{
	Assign(str);
}

//#################################################################################################
CHiveNameW::CHiveNameW(const CStr8 &str)
{
	Assign(str);
}

//#################################################################################################
CHiveNameW::CHiveNameW(PCWSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CHiveNameW::CHiveNameW(PCSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CHiveNameW::CHiveNameW(const HKEY hHive, const bool bShort)
{
	Assign(hHive, bShort);
}

//#################################################################################################
CHiveNameW::operator HKEY(void) const
{
	return GetHive();
}

//#################################################################################################
PCWSTR CHiveNameW::GetLongName(void) const
{
	PCWSTR szHive = nullptr;

	if(StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetShortName(void) const
{
	PCWSTR szHive = nullptr;

	if(StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShortW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShortW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShortW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShortW;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShortW;

	return szHive;
}

//#################################################################################################
HKEY CHiveNameW::GetHive(void) const
{
	HKEY hHive = nullptr;

	if(StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		hHive = HKEY_CLASSES_ROOT;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		hHive = HKEY_CURRENT_USER;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		hHive = HKEY_LOCAL_MACHINE;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		hHive = HKEY_USERS;
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		hHive = HKEY_CURRENT_CONFIG;

	return hHive;
}

//#################################################################################################
CHiveNameW &CHiveNameW::operator=(const CHiveName8 &hive)
{
	Assign(hive);
	return *this;
}

//#################################################################################################
CHiveNameW &CHiveNameW::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CHiveNameW &CHiveNameW::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CHiveNameW &CHiveNameW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CHiveNameW &CHiveNameW::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CHiveNameW &CHiveNameW::operator=(const HKEY hHive)
{
	Assign(hHive);
	return *this;
}

//#################################################################################################
ERRCODE CHiveNameW::Assign(const CHiveNameW &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(this != &hive)
	{
		if(hive.m_nStrLen)
		{
			if(Alloc(hive.m_nStrLen + 1))
			{
				std::wmemcpy(m_sz, hive.m_sz, hive.m_nStrLen);
				m_nStrLen = hive.m_nStrLen;
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
ERRCODE CHiveNameW::Assign(const CHiveName8 &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(hive.m_nStrLen)
	{
		if(Alloc(Utf8ToWide(hive.m_sz, hive.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(hive.m_sz, hive.m_nStrLen, m_sz, m_nBufLen);
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
ERRCODE CHiveNameW::Assign(const CStrW &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidHiveName(str))
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
ERRCODE CHiveNameW::Assign(const CStr8 &str)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CHiveName8::IsValidHiveName(str))
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
ERRCODE CHiveNameW::Assign(PCWSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(IsValidHiveName(sz))
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
ERRCODE CHiveNameW::Assign(PCSTR sz)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(CHiveName8::IsValidHiveName(sz))
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
ERRCODE CHiveNameW::Assign(const HKEY hHive, const bool bShort)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	PCWSTR szHive = nullptr;
	size_t nHiveLen = 0;

	if(hHive == HKEY_CLASSES_ROOT)
	{
		if(bShort)
		{
			szHive = g_szHKCRShortW;
			nHiveLen = g_nHKCRShortLen;
		}
		else
		{
			szHive = g_szHKCRW;
			nHiveLen = g_nHKCRLen;
		}
	}
	else if(hHive == HKEY_CURRENT_USER)
	{
		if(bShort)
		{
			szHive = g_szHKCUShortW;
			nHiveLen = g_nHKCUShortLen;
		}
		else
		{
			szHive = g_szHKCUW;
			nHiveLen = g_nHKCULen;
		}
	}
	else if(hHive == HKEY_LOCAL_MACHINE)
	{
		if(bShort)
		{
			szHive = g_szHKLMShortW;
			nHiveLen = g_nHKLMShortLen;
		}
		else
		{
			szHive = g_szHKLMW;
			nHiveLen = g_nHKLMLen;
		}
	}
	else if(hHive == HKEY_USERS)
	{
		if(bShort)
		{
			szHive = g_szHKUShortW;
			nHiveLen = g_nHKUShortLen;
		}
		else
		{
			szHive = g_szHKUW;
			nHiveLen = g_nHKULen;
		}
	}
	else if(hHive == HKEY_CURRENT_CONFIG)
	{
		if(bShort)
		{
			szHive = g_szHKCCShortW;
			nHiveLen = g_nHKCCShortLen;
		}
		else
		{
			szHive = g_szHKCCW;
			nHiveLen = g_nHKCCLen;
		}
	}

	if(szHive)
	{
		if(Alloc(nHiveLen + 1))
		{
			std::wmemcpy(m_sz, szHive, nHiveLen);
			m_nStrLen = nHiveLen;
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
bool CHiveNameW::operator==(const CHiveNameW &hive) const
{
	return (Compare(hive) == 0);
}

//#################################################################################################
bool CHiveNameW::operator==(const CStrW &str) const
{
	return (Compare(str) == 0);
}

//#################################################################################################
bool CHiveNameW::operator==(PCWSTR sz) const
{
	return (Compare(sz) == 0);
}

//#################################################################################################
bool CHiveNameW::operator==(const HKEY hHive) const
{
	return (Compare(hHive) == 0);
}

//#################################################################################################
bool CHiveNameW::operator!=(const CHiveNameW &hive) const
{
	return (Compare(hive) != 0);
}

//#################################################################################################
bool CHiveNameW::operator!=(const CStrW &str) const
{
	return (Compare(str) != 0);
}

//#################################################################################################
bool CHiveNameW::operator!=(PCWSTR sz) const
{
	return (Compare(sz) != 0);
}

//#################################################################################################
bool CHiveNameW::operator!=(const HKEY hHive) const
{
	return (Compare(hHive) != 0);
}

//#################################################################################################
bool CHiveNameW::operator<(const CHiveNameW &hive) const
{
	return (Compare(hive) < 0);
}

//#################################################################################################
bool CHiveNameW::operator<(const CStrW &str) const
{
	return (Compare(str) < 0);
}

//#################################################################################################
bool CHiveNameW::operator<(PCWSTR sz) const
{
	return (Compare(sz) < 0);
}

//#################################################################################################
bool CHiveNameW::operator<(const HKEY hHive) const
{
	return (Compare(hHive) < 0);
}

//#################################################################################################
bool CHiveNameW::operator<=(const CHiveNameW &hive) const
{
	return (Compare(hive) <= 0);
}

//#################################################################################################
bool CHiveNameW::operator<=(const CStrW &str) const
{
	return (Compare(str) <= 0);
}

//#################################################################################################
bool CHiveNameW::operator<=(PCWSTR sz) const
{
	return (Compare(sz) <= 0);
}

//#################################################################################################
bool CHiveNameW::operator<=(const HKEY hHive) const
{
	return (Compare(hHive) <= 0);
}

//#################################################################################################
bool CHiveNameW::operator>(const CHiveNameW &hive) const
{
	return (Compare(hive) > 0);
}

//#################################################################################################
bool CHiveNameW::operator>(const CStrW &str) const
{
	return (Compare(str) > 0);
}

//#################################################################################################
bool CHiveNameW::operator>(PCWSTR sz) const
{
	return (Compare(sz) > 0);
}

//#################################################################################################
bool CHiveNameW::operator>(const HKEY hHive) const
{
	return (Compare(hHive) > 0);
}

//#################################################################################################
bool CHiveNameW::operator>=(const CHiveNameW &hive) const
{
	return (Compare(hive) >= 0);
}

//#################################################################################################
bool CHiveNameW::operator>=(const CStrW &str) const
{
	return (Compare(str) >= 0);
}

//#################################################################################################
bool CHiveNameW::operator>=(PCWSTR sz) const
{
	return (Compare(sz) >= 0);
}

//#################################################################################################
bool CHiveNameW::operator>=(const HKEY hHive) const
{
	return (Compare(hHive) >= 0);
}

//#################################################################################################
int CHiveNameW::Compare(const CHiveNameW &hive) const
{
	int nCompareRtn = 0;

	int n;

	if(StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0)
	{
		if((n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCRW, g_nHKCRLen, true)) != 0 && StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
	{
		if(StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCRW, g_nHKCRLen, true) != 0 && (n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true) == 0)
	{
		if((n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCUW, g_nHKCULen, true)) != 0 && StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
	{
		if(StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCUW, g_nHKCULen, true) != 0 && (n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0)
	{
		if((n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKLMW, g_nHKLMLen, true)) != 0 && StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
	{
		if(StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKLMW, g_nHKLMLen, true) != 0 && (n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true) == 0)
	{
		if((n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKUW, g_nHKULen, true)) != 0 && StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
	{
		if(StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKUW, g_nHKULen, true) != 0 && (n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0)
	{
		if((n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCCW, g_nHKCCLen, true)) != 0 && StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
	{
		if(StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCCW, g_nHKCCLen, true) != 0 && (n = StringCompareW(hive.m_sz, hive.m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else
		nCompareRtn = StringCompareW(m_sz, m_nStrLen, hive.m_sz, hive.m_nStrLen, true);

	return nCompareRtn;
}

//#################################################################################################
int CHiveNameW::Compare(const CStrW &str) const
{
	int nCompareRtn = 0;

	int n;

	if(StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0)
	{
		if((n = StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true)) != 0 && StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
	{
		if(StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true) != 0 && (n = StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true) == 0)
	{
		if((n = StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true)) != 0 && StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
	{
		if(StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true) != 0 && (n = StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0)
	{
		if((n = StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true)) != 0 && StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
	{
		if(StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true) != 0 && (n = StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true) == 0)
	{
		if((n = StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true)) != 0 && StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
	{
		if(StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true) != 0 && (n = StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0)
	{
		if((n = StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true)) != 0 && StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
	{
		if(StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true) != 0 && (n = StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else
		nCompareRtn = StringCompareW(m_sz, m_nStrLen, str, str.GetLength());

	return nCompareRtn;
}

//#################################################################################################
int CHiveNameW::Compare(PCWSTR sz) const
{
	int nCompareRtn = 0;

	size_t nStrLen = StringGetLength(sz);
	int n;

	if(StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0)
	{
		if((n = StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true)) != 0 && StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
	{
		if(StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true) != 0 && (n = StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true) == 0)
	{
		if((n = StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true)) != 0 && StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
	{
		if(StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true) != 0 && (n = StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0)
	{
		if((n = StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true)) != 0 && StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
	{
		if(StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true) != 0 && (n = StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true) == 0)
	{
		if((n = StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true)) != 0 && StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
	{
		if(StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true) != 0 && (n = StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0)
	{
		if((n = StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true)) != 0 && StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) != 0)
			nCompareRtn = n;
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
	{
		if(StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true) != 0 && (n = StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true)) != 0)
			nCompareRtn = n;
	}
	else
		nCompareRtn = StringCompareW(m_sz, m_nStrLen, sz, nStrLen, true);

	return nCompareRtn;
}

//#################################################################################################
int CHiveNameW::Compare(const HKEY hHive) const
{
	int nCompareRtn = 0;

	if(StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0)
	{
		if(hHive != HKEY_CLASSES_ROOT)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKCRW, g_nHKCRLen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
	{
		if(hHive != HKEY_CLASSES_ROOT)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_USER)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKCUW, g_nHKCULen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_USER)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0)
	{
		if(hHive != HKEY_LOCAL_MACHINE)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKLMW, g_nHKLMLen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
	{
		if(hHive != HKEY_LOCAL_MACHINE)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true) == 0)
	{
		if(hHive != HKEY_USERS)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKUW, g_nHKULen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
	{
		if(hHive != HKEY_USERS)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKUShortW, g_nHKUShortLen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_CONFIG)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKCCW, g_nHKCCLen, true);
	}
	else if(StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
	{
		if(hHive != HKEY_CURRENT_CONFIG)
			nCompareRtn = StringCompareW(m_sz, m_nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true);
	}
	else
	{
		if(hHive)
			nCompareRtn = -1;
	}

	return nCompareRtn;
}

//#################################################################################################
PCWSTR CHiveNameW::GetLongName(const CStrW &str)
{
	PCWSTR szHive = nullptr;

	if(StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRW;
	else if(StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUW;
	else if(StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMW;
	else if(StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUW;
	else if(StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetLongName(const CStr8 &str)
{
	PCWSTR szHive = nullptr;

	if(StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRW;
	else if(StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUW;
	else if(StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMW;
	else if(StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUW;
	else if(StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetLongName(PCWSTR sz)
{
	PCWSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRW;
	else if(StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUW;
	else if(StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMW;
	else if(StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUW;
	else if(StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetLongName(PCSTR sz)
{
	PCWSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRW;
	else if(StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUW;
	else if(StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMW;
	else if(StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUW;
	else if(StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetLongName(const HKEY hHive)
{
	PCWSTR szHive = nullptr;

	if(hHive == HKEY_CLASSES_ROOT)
		szHive = g_szHKCRW;
	else if(hHive == HKEY_CURRENT_USER)
		szHive = g_szHKCUW;
	else if(hHive == HKEY_LOCAL_MACHINE)
		szHive = g_szHKLMW;
	else if(hHive == HKEY_USERS)
		szHive = g_szHKUW;
	else if(hHive == HKEY_CURRENT_CONFIG)
		szHive = g_szHKCCW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetShortName(const CStrW &str)
{
	PCWSTR szHive = nullptr;

	if(StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShortW;
	else if(StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShortW;
	else if(StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShortW;
	else if(StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShortW;
	else if(StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShortW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetShortName(const CStr8 &str)
{
	PCWSTR szHive = nullptr;

	if(StringCompare8(str, str.GetLength(), g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShortW;
	else if(StringCompare8(str, str.GetLength(), g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShortW;
	else if(StringCompare8(str, str.GetLength(), g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShortW;
	else if(StringCompare8(str, str.GetLength(), g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShortW;
	else if(StringCompare8(str, str.GetLength(), g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(str, str.GetLength(), g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShortW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetShortName(PCWSTR sz)
{
	PCWSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShortW;
	else if(StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShortW;
	else if(StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShortW;
	else if(StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShortW;
	else if(StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShortW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetShortName(PCSTR sz)
{
	PCWSTR szHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompare8(sz, nStrLen, g_szHKCR8, g_nHKCRLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCRShort8, g_nHKCRShortLen, true) == 0)
		szHive = g_szHKCRShortW;
	else if(StringCompare8(sz, nStrLen, g_szHKCU8, g_nHKCULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCUShort8, g_nHKCUShortLen, true) == 0)
		szHive = g_szHKCUShortW;
	else if(StringCompare8(sz, nStrLen, g_szHKLM8, g_nHKLMLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKLMShort8, g_nHKLMShortLen, true) == 0)
		szHive = g_szHKLMShortW;
	else if(StringCompare8(sz, nStrLen, g_szHKU8, g_nHKULen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKUShort8, g_nHKUShortLen, true) == 0)
		szHive = g_szHKUShortW;
	else if(StringCompare8(sz, nStrLen, g_szHKCC8, g_nHKCCLen, true) == 0 || StringCompare8(sz, nStrLen, g_szHKCCShort8, g_nHKCCShortLen, true) == 0)
		szHive = g_szHKCCShortW;

	return szHive;
}

//#################################################################################################
PCWSTR CHiveNameW::GetShortName(const HKEY hHive)
{
	PCWSTR szHive = nullptr;

	if(hHive == HKEY_CLASSES_ROOT)
		szHive = g_szHKCRShortW;
	else if(hHive == HKEY_CURRENT_USER)
		szHive = g_szHKCUShortW;
	else if(hHive == HKEY_LOCAL_MACHINE)
		szHive = g_szHKLMShortW;
	else if(hHive == HKEY_USERS)
		szHive = g_szHKUShortW;
	else if(hHive == HKEY_CURRENT_CONFIG)
		szHive = g_szHKCCShortW;

	return szHive;
}

//#################################################################################################
HKEY CHiveNameW::GetHive(const CStrW &str)
{
	HKEY hHive = nullptr;

	if(StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		hHive = HKEY_CLASSES_ROOT;
	else if(StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		hHive = HKEY_CURRENT_USER;
	else if(StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		hHive = HKEY_LOCAL_MACHINE;
	else if(StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true) == 0)
		hHive = HKEY_USERS;
	else if(StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		hHive = HKEY_CURRENT_CONFIG;

	return hHive;
}

//#################################################################################################
HKEY CHiveNameW::GetHive(PCWSTR sz)
{
	HKEY hHive = nullptr;

	size_t nStrLen = StringGetLength(sz);
	if(StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0)
		hHive = HKEY_CLASSES_ROOT;
	else if(StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0)
		hHive = HKEY_CURRENT_USER;
	else if(StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0)
		hHive = HKEY_LOCAL_MACHINE;
	else if(StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0)
		hHive = HKEY_USERS;
	else if(StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0)
		hHive = HKEY_CURRENT_CONFIG;

	return hHive;
}

//#################################################################################################
bool CHiveNameW::IsValidHiveName(const CStrW &str)
{
	return (StringCompareW(str, str.GetLength(), g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCRShortW, g_nHKCRShortLen, true) == 0 ||
		StringCompareW(str, str.GetLength(), g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCUShortW, g_nHKCUShortLen, true) == 0 ||
		StringCompareW(str, str.GetLength(), g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKLMShortW, g_nHKLMShortLen, true) == 0 ||
		StringCompareW(str, str.GetLength(), g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKUShortW, g_nHKUShortLen, true) == 0 ||
		StringCompareW(str, str.GetLength(), g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(str, str.GetLength(), g_szHKCCShortW, g_nHKCCShortLen, true) == 0);
}

//#################################################################################################
bool CHiveNameW::IsValidHiveName(PCWSTR sz)
{
	size_t nStrLen = StringGetLength(sz);
	return (StringCompareW(sz, nStrLen, g_szHKCRW, g_nHKCRLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCRShortW, g_nHKCRShortLen, true) == 0 ||
		StringCompareW(sz, nStrLen, g_szHKCUW, g_nHKCULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCUShortW, g_nHKCUShortLen, true) == 0 ||
		StringCompareW(sz, nStrLen, g_szHKLMW, g_nHKLMLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKLMShortW, g_nHKLMShortLen, true) == 0 ||
		StringCompareW(sz, nStrLen, g_szHKUW, g_nHKULen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKUShortW, g_nHKUShortLen, true) == 0 ||
		StringCompareW(sz, nStrLen, g_szHKCCW, g_nHKCCLen, true) == 0 || StringCompareW(sz, nStrLen, g_szHKCCShortW, g_nHKCCShortLen, true) == 0);
}




/////////////////////////////////////////////////
// CRegPath8
/////////////////////////////////////////////////

//#################################################################################################
CRegPath8::CRegPath8(void)
	: m_bRooted(false)
{
}

//#################################################################################################
CRegPath8::CRegPath8(const CRegPath8 &src)
	: m_bRooted(false)
{
	Assign(src);
}

//#################################################################################################
CRegPath8::CRegPath8(CRegPath8 &&src) noexcept
	: m_vecSegments(std::move(src.m_vecSegments)),
	m_bRooted(src.m_bRooted)
{
	src.m_bRooted = false;
}

//#################################################################################################
CRegPath8::CRegPath8(const CRegPathW &path)
	: m_bRooted(false)
{
	Assign(path);
}

//#################################################################################################
CRegPath8::CRegPath8(const CRegPathSegment8 &path)
	: m_bRooted(false)
{
	Assign(path);
}

//#################################################################################################
CRegPath8::CRegPath8(const CRegPathSegmentW &path)
	: m_bRooted(false)
{
	Assign(path);
}

//#################################################################################################
CRegPath8::CRegPath8(const CHiveName8 &hive)
	: m_bRooted(false)
{
	Assign(hive);
}

//#################################################################################################
CRegPath8::CRegPath8(const CHiveNameW &hive)
	: m_bRooted(false)
{
	Assign(hive);
}

//#################################################################################################
CRegPath8::CRegPath8(const CStr8 &str)
	: m_bRooted(false)
{
	ParsePath(str);
}

//#################################################################################################
CRegPath8::CRegPath8(const CStrW &str)
	: m_bRooted(false)
{
	ParsePath(str.AsUtf8());
}

//#################################################################################################
CRegPath8::CRegPath8(PCSTR sz)
	: m_bRooted(false)
{
	ParsePath(CStr8(sz));
}

//#################################################################################################
CRegPath8::CRegPath8(PCWSTR sz)
	: m_bRooted(false)
{
	ParsePath(CStr8(sz));
}

//#################################################################################################
CRegPath8::CRegPath8(const char ch)
	: m_bRooted(false)
{
	ParsePath(CStr8(ch));
}

//#################################################################################################
CRegPath8::CRegPath8(const wchar_t ch)
	: m_bRooted(false)
{
	ParsePath(CStr8(ch));
}

//#################################################################################################
CRegPath8::operator CStr8(void) const
{
	return Get();
}

//#################################################################################################
CStr8 CRegPath8::Get(void) const
{	// Convert the vector of path segments into a single string
	CStr8 str;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		str.Assign((*itr)->Get());

		while(++itr != m_vecSegments.end())
		{
			str.Append(g_chBackSlash8);
			str.Append((*itr)->Get());
		}
	}

	return str;
}

//#################################################################################################
size_t CRegPath8::GetLength(const bool bIncludeNullTerm) const
{	// Length in characters
	size_t nStrLen = 0;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		nStrLen = (*itr)->GetLength();

		while(++itr != m_vecSegments.end())
			nStrLen += (*itr)->GetLength() + 1;	// Add one for the backslash

		if(bIncludeNullTerm)
			++nStrLen;
	}

	return nStrLen;
}

//#################################################################################################
size_t CRegPath8::GetSize(const bool bIncludeNullTerm) const
{	// Size in bytes
	return GetLength(bIncludeNullTerm);
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CRegPath8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(CRegPath8 &&src) noexcept
{
	if(this != &src)
	{
		m_vecSegments = std::move(src.m_vecSegments);
		m_bRooted = src.m_bRooted;

		src.m_bRooted = false;
	}

	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CRegPathW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CRegPathSegment8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CRegPathSegmentW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CHiveName8 &hive)
{
	Assign(hive);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CHiveNameW &hive)
{
	Assign(hive);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CStr8 &str)
{
	ParsePath(str);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const CStrW &str)
{
	ParsePath(str.AsUtf8());
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(PCSTR sz)
{
	ParsePath(CStr8(sz));
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(PCWSTR sz)
{
	ParsePath(CStr8(sz));
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const char ch)
{
	ParsePath(CStr8(ch));
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator=(const wchar_t ch)
{
	ParsePath(CStr8(ch));
	return *this;
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CRegPath8 &path)
{
	if(this != &path)
	{
		Empty();

		m_bRooted = path.m_bRooted;

		// Add segments to the path
		auto itr = path.m_vecSegments.begin();
		if(path.m_bRooted)
		{
			m_vecSegments.push_back(std::make_unique<CHiveName8>(**itr));
			++itr;
		}

		while(itr != path.m_vecSegments.end())
		{
			m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(**itr));
			++itr;
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CRegPathW &path)
{
	Empty();

	m_bRooted = path.m_bRooted;

	// Add segments to the path
	auto itr = path.m_vecSegments.begin();
	if(path.m_bRooted)
	{
		m_vecSegments.push_back(std::make_unique<CHiveName8>(**itr));
		++itr;
	}

	while(itr != path.m_vecSegments.end())
	{
		m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(**itr));
		++itr;
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CRegPathSegment8 &path)
{
	Empty();

	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CRegPathSegmentW &path)
{
	Empty();

	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CHiveName8 &hive)
{
	Empty();

	if(!hive.IsEmpty())
	{
		m_bRooted = true;
		m_vecSegments.push_back(std::make_unique<CHiveName8>(hive));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CHiveNameW &hive)
{
	Empty();

	if(!hive.IsEmpty())
	{
		m_bRooted = true;
		m_vecSegments.push_back(std::make_unique<CHiveName8>(hive));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CStr8 &str)
{
	return ParsePath(str);
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const CStrW &str)
{
	return ParsePath(str.AsUtf8());
}

//#################################################################################################
ERRCODE CRegPath8::Assign(PCSTR sz)
{
	return ParsePath(CStr8(sz));
}

//#################################################################################################
ERRCODE CRegPath8::Assign(PCWSTR sz)
{
	return ParsePath(CStr8(sz));
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const char ch)
{
	return ParsePath(CStr8(ch));
}

//#################################################################################################
ERRCODE CRegPath8::Assign(const wchar_t ch)
{
	return ParsePath(CStr8(ch));
}

//#################################################################################################
CRegPath8 &CRegPath8::operator+=(const CRegPath8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator+=(const CRegPathW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator+=(const CRegPathSegment8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator+=(const CRegPathSegmentW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator+=(const CHiveName8 &hive)
{
	Append(hive);
	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::operator+=(const CHiveNameW &hive)
{
	Append(hive);
	return *this;
}

//#################################################################################################
ERRCODE CRegPath8::Prepend(const CRegPath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Add segments to the path
			m_bRooted = path.m_bRooted;
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegment8>(**itr));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Prepend(const CRegPathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Add segments to the path
			m_bRooted = path.m_bRooted;
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegment8>(**itr));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Prepend(const CRegPathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegment8>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Prepend(const CRegPathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegment8>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Prepend(const CHiveName8 &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_bRooted)
		{
			m_bRooted = true;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CHiveName8>(hive));
		}
		else
			nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Prepend(const CHiveNameW &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_bRooted)
		{
			m_bRooted = true;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CHiveName8>(hive));
		}
		else
			nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Append(const CRegPath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Cannot append a rooted path to an existing path
			if(path.m_bRooted)
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{	// Add segments to the path
				for(const auto &segment : path.m_vecSegments)
					m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(*segment));
			}
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Append(const CRegPathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Cannot append a rooted path to an existing path
			if(path.m_bRooted)
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{	// Add segments to the path
				for(const auto &segment : path.m_vecSegments)
					m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(*segment));
			}
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Append(const CRegPathSegment8 &path)
{
	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Append(const CRegPathSegmentW &path)
{
	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegment8>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPath8::Append(const CHiveName8 &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_vecSegments.empty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_bRooted = true;
			m_vecSegments.push_back(std::make_unique<CHiveName8>(hive));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Append(const CHiveNameW &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_vecSegments.empty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_bRooted = true;
			m_vecSegments.push_back(std::make_unique<CHiveName8>(hive));
		}
	}

	return nErrorCode;
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CRegPath8 &path)
{
	stream << path.Get();
	return stream;
}

//#################################################################################################
bool CRegPath8::operator==(const CRegPath8 &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_bRooted != path.m_bRooted)
		return false;

	// We need to compare each segment in the path
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();

	if(m_bRooted)
	{
		if(*(CHiveName8*)itr1->get() != *(CHiveName8*)itr2->get())
			return false;

		++itr1;
		++itr2;
	}

	while(itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if(**itr1 != **itr2)
			return false;

		++itr1;
		++itr2;
	}

	return true;
}

//#################################################################################################
bool CRegPath8::operator==(const CRegPathSegment8 &path) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
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
bool CRegPath8::operator==(const CHiveName8 &hive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
		return hive.IsEmpty();

	// Are they the same size, and is the path rooted?
	if(m_vecSegments.size() != 1 || !m_bRooted)
		return false;

	// We need to compare the segment
	if(hive != *m_vecSegments.front())
		return false;

	return true;
}

//#################################################################################################
bool CRegPath8::operator!=(const CRegPath8 &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_bRooted != path.m_bRooted)
		return true;

	// We need to compare each segment in the path
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();

	if(m_bRooted)
	{
		if(*(CHiveName8*)itr1->get() != *(CHiveName8*)itr2->get())
			return true;

		++itr1;
		++itr2;
	}

	while(itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if(**itr1 != **itr2)
			return true;

		++itr1;
		++itr2;
	}

	return false;
}

//#################################################################################################
bool CRegPath8::operator!=(const CRegPathSegment8 &path) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
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
bool CRegPath8::operator!=(const CHiveName8 &hive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
		return !hive.IsEmpty();

	// Are they the same size, and is the path rooted?
	if(m_vecSegments.size() != 1 || !m_bRooted)
		return true;

	// We need to compare the segment
	if(hive != *m_vecSegments.front())
		return true;

	return false;
}

//#################################################################################################
bool CRegPath8::operator<(const CRegPath8 &path) const
{
	return (Get().Compare(path.Get(), true) < 0);
}

//#################################################################################################
bool CRegPath8::operator<=(const CRegPath8 &path) const
{
	return (Get().Compare(path.Get(), true) <= 0);
}

//#################################################################################################
bool CRegPath8::operator>(const CRegPath8 &path) const
{
	return (Get().Compare(path.Get(), true) > 0);
}

//#################################################################################################
bool CRegPath8::operator>=(const CRegPath8 &path) const
{
	return (Get().Compare(path.Get(), true) >= 0);
}

//#################################################################################################
bool CRegPath8::Compare(const CRegPath8 &path, const bool bCaseInsensitive) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_bRooted != path.m_bRooted)
		return false;

	// We need to compare each segment in the path
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();

	if(m_bRooted)
	{
		if(*(CHiveName8*)itr1->get() != *(CHiveName8*)itr2->get())
			return false;

		++itr1;
		++itr2;
	}

	while(itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;

		++itr1;
		++itr2;
	}

	return true;
}

//#################################################################################################
bool CRegPath8::Compare(const CRegPathSegment8 &path, const bool bCaseInsensitive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
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
bool CRegPath8::Compare(const CHiveName8 &hive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
		return hive.IsEmpty();

	// Are they the same size, and is the path rooted?
	if(m_vecSegments.size() != 1 || !m_bRooted)
		return false;

	// We need to compare the segment
	if(hive != *m_vecSegments.front())
		return false;

	return true;
}

//#################################################################################################
bool CRegPath8::CompareLeft(const CRegPath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// Are they the same type of path?
	if(m_bRooted != path.m_bRooted)
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();
	while(nCount != 0 && itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if(m_bRooted && (*itr1) == m_vecSegments.front())
		{
			if(((CHiveName8*)itr1->get())->Compare(*(CHiveName8*)itr2->get()) != 0)
				return false;
		}
		else
		{
			if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
				return false;
		}

		++itr1;
		++itr2;
		--nCount;
	}

	return true;
}

//#################################################################################################
bool CRegPath8::CompareRight(const CRegPath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	auto itr1 = m_vecSegments.rbegin();
	auto itr2 = path.m_vecSegments.rbegin();
	while(nCount != 0 && itr1 != m_vecSegments.rend() && itr2 != path.m_vecSegments.rend())
	{
		if(m_bRooted && (*itr1) == m_vecSegments.front())
		{
			if(((CHiveName8*)itr1->get())->Compare(*(CHiveName8*)itr2->get()) != 0)
				return false;
		}
		else
		{
			if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
				return false;
		}
		++itr1;
		++itr2;
		--nCount;
	}

	return true;
}

//#################################################################################################
bool CRegPath8::Contains(const CRegPath8 &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty() && m_vecSegments.size() >= path.m_vecSegments.size())
	{
		CRegPath8 pathMid;
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
bool CRegPath8::Contains(const CRegPathSegment8 &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty())
		bFound = std::any_of(m_vecSegments.begin(), m_vecSegments.end(), [&](const std::unique_ptr<CRegPathSegment8> &segment) {return (segment->Compare(path, bCaseInsensitive) == 0);});

	return bFound;
}

//#################################################################################################
bool CRegPath8::Contains(const CHiveName8 &hive) const
{
	bool bFound = false;

	// We need to compare the segment
	if(!hive.IsEmpty() && hive == *m_vecSegments.front())
		bFound = true;

	return bFound;
}

//#################################################################################################
void CRegPath8::Empty(void)
{
	m_bRooted = false;
	m_vecSegments.clear();
}

//#################################################################################################
bool CRegPath8::IsEmpty(void) const noexcept
{
	return m_vecSegments.empty();
}

//#################################################################################################
bool CRegPath8::IsRoot(void) const noexcept
{
	return (m_bRooted && m_vecSegments.size() == 1);
}

//#################################################################################################
bool CRegPath8::IsRelative(void) const noexcept
{
	return (!m_bRooted && !m_vecSegments.empty());
}

//#################################################################################################
size_t CRegPath8::GetSegmentCount(void) const noexcept
{
	return m_vecSegments.size();
}

//#################################################################################################
CRegPathSegment8 CRegPath8::GetSegment(const size_t nSegment) const
{
	CRegPathSegment8 path;

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
CRegPathSegment8 CRegPath8::GetFirstSegment(void) const
{
	return (!m_vecSegments.empty()) ? CRegPathSegment8(*m_vecSegments.front()) : CRegPathSegment8();
}

//#################################################################################################
CRegPathSegment8 CRegPath8::GetLastSegment(void) const
{
	return (!m_vecSegments.empty()) ? CRegPathSegment8(*m_vecSegments.back()) : CRegPathSegment8();
}

//#################################################################################################
CRegPath8 CRegPath8::GetLeft(const size_t nSegmentCount) const
{
	CRegPath8 path(*this);
	path.CropLeft(nSegmentCount);

	return path;
}

//#################################################################################################
CRegPath8 CRegPath8::GetRight(const size_t nSegmentCount) const
{
	CRegPath8 path(*this);
	path.CropRight(nSegmentCount);

	return path;
}

//#################################################################################################
CRegPath8 CRegPath8::GetMid(const size_t nSegment, const size_t nSegmentCount) const
{
	CRegPath8 path;

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
			path.m_bRooted = m_bRooted;
	}

	return path;
}

//#################################################################################################
CRegPath8 &CRegPath8::TrimLeft(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{	// If it was rooted it no longer is because we are removing the first segment
			m_bRooted = false;

			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
CRegPath8 &CRegPath8::TrimRight(const size_t nSegmentCount)
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
CRegPath8 &CRegPath8::CropLeft(const size_t nSegmentCount)
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
CRegPath8 &CRegPath8::CropRight(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecSegments.size())
	{
		if(nSegmentCount == 0)
			Empty();
		else
		{	// If it was rooted it no longer is because we are removing the first segment
			m_bRooted = false;

			size_t nToRemove = m_vecSegments.size() - nSegmentCount;
			while(nToRemove--)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CRegPath8::Insert(const size_t nSegment, const CRegPath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if((nSegment == 0 && m_bRooted) || (nSegment && path.m_bRooted))
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.insert(itr, std::make_unique<CRegPathSegment8>(*segment));

			if(nSegment == 0)
				m_bRooted = path.m_bRooted;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Insert(const size_t nSegment, const CRegPathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(nSegment == 0 && m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			m_vecSegments.insert(itr, std::make_unique<CRegPathSegment8>(path));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPath8::Insert(const CHiveName8 &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_bRooted = true;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CHiveName8>(hive));
		}
	}

	return nErrorCode;
}

//#################################################################################################
CRegPath8 &CRegPath8::Delete(const size_t nSegment, const size_t nSegmentCount)
{
	if(nSegment < m_vecSegments.size() && nSegmentCount != 0)
	{
		if(nSegment == 0)
			m_bRooted = false;

		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(itr != m_vecSegments.end() && nCount-- != 0)
			++itr;

		nCount = nSegmentCount;
		while(itr != m_vecSegments.end() && nCount-- != 0)
			itr = m_vecSegments.erase(itr);
	}

	return *this;
}

//#################################################################################################
size_t CRegPath8::Remove(const CRegPathSegment8 &path, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecSegments.begin();
	while(itr != m_vecSegments.end())
	{
		if((*itr)->Compare(path, bCaseInsensitive) == 0)
		{
			if(itr == m_vecSegments.begin())
				m_bRooted = false;

			itr = m_vecSegments.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	return nCount;
}

//#################################################################################################
size_t CRegPath8::Replace(const CRegPathSegment8 &pathOld, const CRegPathSegment8 &pathNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!pathNew.IsEmpty())
	{
		for(auto &itr : m_vecSegments)
		{
			if(itr->Compare(pathOld, bCaseInsensitive) == 0)
			{
				itr = std::make_unique<CRegPathSegment8>(pathNew);
				++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
CRegPath8 &CRegPath8::Modify(const size_t nSegment, const CRegPathSegment8 &path)
{
	if(nSegment < m_vecSegments.size() && !path.IsEmpty())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(nCount-- != 0)
			++itr;

		*itr = std::make_unique<CRegPathSegment8>(path);
	}

	return *this;
}

//#################################################################################################
CStr8 CRegPath8::GetLongPath(void) const
{	// Convert the vector of path segments into a single string
	CStr8 str;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		if(m_bRooted)
			str.Assign(((CHiveName8*)itr->get())->GetLongName());
		else
			str.Assign((*itr)->Get());

		while(++itr != m_vecSegments.end())
		{
			str.Append(g_chBackSlash8);
			str.Append((*itr)->Get());
		}
	}

	return str;
}

//#################################################################################################
CStr8 CRegPath8::GetShortPath(void) const
{	// Convert the vector of path segments into a single string
	CStr8 str;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		if(m_bRooted)
			str.Assign(((CHiveName8*)itr->get())->GetShortName());
		else
			str.Assign((*itr)->Get());

		while(++itr != m_vecSegments.end())
		{
			str.Append(g_chBackSlash8);
			str.Append((*itr)->Get());
		}
	}

	return str;
}

//#################################################################################################
CHiveName8 CRegPath8::GetHive(void) const
{
	return (m_bRooted && !m_vecSegments.empty()) ? CHiveName8(*m_vecSegments.front()) : CHiveName8();
}

//#################################################################################################
CRegPath8 CRegPath8::GetRelativePath(void) const
{
	CRegPath8 path(*this);

	if(path.m_bRooted)
		path.TrimLeft();

	return path;
}

//#################################################################################################
void CRegPath8::DebugPrint(const bool bAppendNewline) const
{
	CStr8 str(Get());
	if(bAppendNewline)
		str.Append(g_szEol8);
	OutputDebugStringA(str);
}

//#################################################################################################
ERRCODE CRegPath8::ParsePath(const CStr8 &strPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Empty();

	if(strPath.IsEmpty())
		return nErrorCode;

	CStr8 strSegment;
	PCSTR szPath = strPath;
	PCSTR szLastDelim = szPath;
	PCSTR szNextDelim;
	size_t nDelimLen = strPath.GetLength();

	// Is it a kernel path?
	if(strPath.CompareLeft(g_szRegKernelMachine8, g_nKernelMachineLen, true) == 0)
	{
		m_bRooted = true;
		auto pHive = std::make_unique<CHiveName8>(g_szHKLM8);
		if(pHive->IsEmpty())
			return FW_ERROR_INVALID_DATA;

		m_vecSegments.push_back(std::move(pHive));

		szLastDelim += g_nKernelMachineLen;
		nDelimLen -= g_nKernelMachineLen;
	}
	else if(strPath.CompareLeft(g_szRegKernelUser8, g_nKernelUserLen, true) == 0)
	{
		m_bRooted = true;
		auto pHive = std::make_unique<CHiveName8>(g_szHKU8);
		if(pHive->IsEmpty())
			return FW_ERROR_INVALID_DATA;

		m_vecSegments.push_back(std::move(pHive));

		szLastDelim += g_nKernelUserLen;
		nDelimLen -= g_nKernelUserLen;
	}

	// Find first non-delimiter character
	while(*szLastDelim == g_chBackSlash8)
		++szLastDelim, --nDelimLen;

	do{
		szNextDelim = (PCSTR)std::memchr(szLastDelim, g_chBackSlash8, nDelimLen);
		if(szNextDelim == nullptr)
		{	// No more delimiters, the rest of the string is the last segment
			if(m_vecSegments.empty() && CHiveName8::IsValidHiveName(szLastDelim))
			{
				m_bRooted = true;
				auto pHive = std::make_unique<CHiveName8>(szLastDelim);
				if(pHive->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pHive));
			}
			else
			{
				auto pPath = std::make_unique<CRegPathSegment8>(szLastDelim);
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
			strSegment.Assign(strPath, (size_t)(szLastDelim - szPath), (size_t)(szNextDelim - szLastDelim));
			if(m_vecSegments.empty() && CHiveName8::IsValidHiveName(strSegment))
			{
				m_bRooted = true;
				auto pHive = std::make_unique<CHiveName8>(strSegment);
				if(pHive->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pHive));
			}
			else
			{
				auto pPath = std::make_unique<CRegPathSegment8>(strSegment);
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
bool CRegPath8::SplitPath(PCSTR szPath, CStr8 &strHive, CStr8 &strPath)
{
	bool bValid = false;

	strHive.Empty();
	strPath.Empty();

	CRegPath8 path;
	if(path.Assign(szPath) == FW_NO_ERROR)
	{
		strHive.Assign(path.GetHive());
		strPath.Assign(path.GetRelativePath());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CRegPath8::IsValidPath(const CStr8 &strPath)
{
	bool bValid = true;

	CRegPath8 path;
	if(path.Assign(strPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CRegPath8::IsValidPath(PCSTR szPath)
{
	bool bValid = true;

	CRegPath8 path;
	if(path.Assign(szPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CRegPath8::IsValidPath(const char chPath)
{
	bool bValid = true;

	CRegPath8 path;
	if(path.Assign(chPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}




/////////////////////////////////////////////////
// CRegPathW
/////////////////////////////////////////////////

//#################################################################################################
CRegPathW::CRegPathW(void)
	: m_bRooted(false)
{
}

//#################################################################################################
CRegPathW::CRegPathW(const CRegPathW &src)
	: m_bRooted(false)
{
	Assign(src);
}

//#################################################################################################
CRegPathW::CRegPathW(CRegPathW &&src) noexcept
	: m_vecSegments(std::move(src.m_vecSegments)),
	m_bRooted(src.m_bRooted)
{
	src.m_bRooted = false;
}

//#################################################################################################
CRegPathW::CRegPathW(const CRegPath8 &path)
	: m_bRooted(false)
{
	Assign(path);
}

//#################################################################################################
CRegPathW::CRegPathW(const CRegPathSegmentW &path)
	: m_bRooted(false)
{
	Assign(path);
}

//#################################################################################################
CRegPathW::CRegPathW(const CRegPathSegment8 &path)
	: m_bRooted(false)
{
	Assign(path);
}

//#################################################################################################
CRegPathW::CRegPathW(const CHiveNameW &hive)
	: m_bRooted(false)
{
	Assign(hive);
}

//#################################################################################################
CRegPathW::CRegPathW(const CHiveName8 &hive)
	: m_bRooted(false)
{
	Assign(hive);
}

//#################################################################################################
CRegPathW::CRegPathW(const CStrW &str)
	: m_bRooted(false)
{
	ParsePath(str);
}

//#################################################################################################
CRegPathW::CRegPathW(const CStr8 &str)
	: m_bRooted(false)
{
	ParsePath(str.AsWide());
}

//#################################################################################################
CRegPathW::CRegPathW(PCWSTR sz)
	: m_bRooted(false)
{
	ParsePath(CStrW(sz));
}

//#################################################################################################
CRegPathW::CRegPathW(PCSTR sz)
	: m_bRooted(false)
{
	ParsePath(CStrW(sz));
}

//#################################################################################################
CRegPathW::CRegPathW(const wchar_t ch)
	: m_bRooted(false)
{
	ParsePath(CStrW(ch));
}

//#################################################################################################
CRegPathW::CRegPathW(const char ch)
	: m_bRooted(false)
{
	ParsePath(CStrW(ch));
}

//#################################################################################################
CRegPathW::operator CStrW(void) const
{
	return Get();
}

//#################################################################################################
CStrW CRegPathW::Get(void) const
{	// Convert the vector of path segments into a single string
	CStrW str;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		str.Assign((*itr)->Get());

		while(++itr != m_vecSegments.end())
		{
			str.Append(g_chBackSlashW);
			str.Append((*itr)->Get());
		}
	}

	return str;
}

//#################################################################################################
size_t CRegPathW::GetLength(const bool bIncludeNullTerm) const
{	// Length in characters
	size_t nStrLen = 0;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		nStrLen = (*itr)->GetLength();

		while(++itr != m_vecSegments.end())
			nStrLen += (*itr)->GetLength() + 1;	// Add one for the backslash

		if(bIncludeNullTerm)
			++nStrLen;
	}

	return nStrLen;
}

//#################################################################################################
size_t CRegPathW::GetSize(const bool bIncludeNullTerm) const
{	// Size in bytes
	return GetLength(bIncludeNullTerm) * sizeof(wchar_t);
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CRegPathW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(CRegPathW &&src) noexcept
{
	if(this != &src)
	{
		m_vecSegments = std::move(src.m_vecSegments);
		m_bRooted = src.m_bRooted;

		src.m_bRooted = false;
	}

	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CRegPath8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CRegPathSegmentW &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CRegPathSegment8 &path)
{
	Assign(path);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CHiveNameW &hive)
{
	Assign(hive);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CHiveName8 &hive)
{
	Assign(hive);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CStrW &str)
{
	ParsePath(str);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const CStr8 &str)
{
	ParsePath(str.AsWide());
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(PCWSTR sz)
{
	ParsePath(CStrW(sz));
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(PCSTR sz)
{
	ParsePath(CStrW(sz));
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const wchar_t ch)
{
	ParsePath(CStrW(ch));
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator=(const char ch)
{
	ParsePath(CStrW(ch));
	return *this;
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CRegPathW &path)
{
	if(this != &path)
	{
		Empty();

		m_bRooted = path.m_bRooted;

		// Add segments to the path
		auto itr = path.m_vecSegments.begin();
		if(path.m_bRooted)
		{
			m_vecSegments.push_back(std::make_unique<CHiveNameW>(**itr));
			++itr;
		}

		while(itr != path.m_vecSegments.end())
		{
			m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(**itr));
			++itr;
		}
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CRegPath8 &path)
{
	Empty();

	m_bRooted = path.m_bRooted;

	// Add segments to the path
	auto itr = path.m_vecSegments.begin();
	if(path.m_bRooted)
	{
		m_vecSegments.push_back(std::make_unique<CHiveNameW>(**itr));
		++itr;
	}

	while(itr != path.m_vecSegments.end())
	{
		m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(**itr));
		++itr;
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CRegPathSegmentW &path)
{
	Empty();

	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CRegPathSegment8 &path)
{
	Empty();

	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CHiveNameW &hive)
{
	Empty();

	if(!hive.IsEmpty())
	{
		m_bRooted = true;
		m_vecSegments.push_back(std::make_unique<CHiveNameW>(hive));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CHiveName8 &hive)
{
	Empty();

	if(!hive.IsEmpty())
	{
		m_bRooted = true;
		m_vecSegments.push_back(std::make_unique<CHiveNameW>(hive));
	}

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CStrW &str)
{
	return ParsePath(str);
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const CStr8 &str)
{
	return ParsePath(str.AsWide());
}

//#################################################################################################
ERRCODE CRegPathW::Assign(PCWSTR sz)
{
	return ParsePath(CStrW(sz));
}

//#################################################################################################
ERRCODE CRegPathW::Assign(PCSTR sz)
{
	return ParsePath(CStrW(sz));
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const wchar_t ch)
{
	return ParsePath(CStrW(ch));
}

//#################################################################################################
ERRCODE CRegPathW::Assign(const char ch)
{
	return ParsePath(CStrW(ch));
}

//#################################################################################################
CRegPathW &CRegPathW::operator+=(const CRegPathW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator+=(const CRegPath8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator+=(const CRegPathSegmentW &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator+=(const CRegPathSegment8 &path)
{
	Append(path);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator+=(const CHiveNameW &hive)
{
	Append(hive);
	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::operator+=(const CHiveName8 &hive)
{
	Append(hive);
	return *this;
}

//#################################################################################################
ERRCODE CRegPathW::Prepend(const CRegPathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Add segments to the path
			m_bRooted = path.m_bRooted;
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegmentW>(**itr));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Prepend(const CRegPath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Add segments to the path
			m_bRooted = path.m_bRooted;
			for(auto itr = path.m_vecSegments.rbegin(); itr != path.m_vecSegments.rend(); ++itr)
				m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegmentW>(**itr));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Prepend(const CRegPathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegmentW>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Prepend(const CRegPathSegment8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CRegPathSegmentW>(path));
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Prepend(const CHiveNameW &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_bRooted)
		{
			m_bRooted = true;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CHiveNameW>(hive));
		}
		else
			nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Prepend(const CHiveName8 &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_bRooted)
		{
			m_bRooted = true;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CHiveNameW>(hive));
		}
		else
			nErrorCode = FW_ERROR_INVALID_DATA;
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Append(const CRegPathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Cannot append a rooted path to an existing path
			if(path.m_bRooted)
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{	// Add segments to the path
				for(const auto &segment : path.m_vecSegments)
					m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(*segment));
			}
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Append(const CRegPath8 &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(m_vecSegments.empty())
			Assign(path);
		else
		{	// Cannot append a rooted path to an existing path
			if(path.m_bRooted)
				nErrorCode = FW_ERROR_INVALID_DATA;
			else
			{	// Add segments to the path
				for(const auto &segment : path.m_vecSegments)
					m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(*segment));
			}
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Append(const CRegPathSegmentW &path)
{
	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Append(const CRegPathSegment8 &path)
{
	if(!path.IsEmpty())
		m_vecSegments.push_back(std::make_unique<CRegPathSegmentW>(path));

	return FW_NO_ERROR;
}

//#################################################################################################
ERRCODE CRegPathW::Append(const CHiveNameW &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_vecSegments.empty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_bRooted = true;
			m_vecSegments.push_back(std::make_unique<CHiveNameW>(hive));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Append(const CHiveName8 &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(!m_vecSegments.empty())
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_bRooted = true;
			m_vecSegments.push_back(std::make_unique<CHiveNameW>(hive));
		}
	}

	return nErrorCode;
}

//#################################################################################################
CRegPathW operator+(const CRegPathW &path1, const CRegPathW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathW &path1, const CRegPath8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPath8 &path1, const CRegPathW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPath8 &path1, const CRegPath8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathW &path1, const CRegPathSegmentW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathW &path1, const CRegPathSegment8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPath8 &path1, const CRegPathSegmentW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPath8 &path1, const CRegPathSegment8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPathW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPath8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPathW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPath8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPathSegmentW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPathSegment8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPathSegmentW &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPathSegment8 &path2)
{
	CRegPathW pathRtn(path1);
	pathRtn.Append(path2);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveNameW &hive, const CRegPathW &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveNameW &hive, const CRegPath8 &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveName8 &hive, const CRegPathW &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveName8 &hive, const CRegPath8 &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveNameW &hive, const CRegPathSegmentW &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveNameW &hive, const CRegPathSegment8 &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveName8 &hive, const CRegPathSegmentW &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
CRegPathW operator+(const CHiveName8 &hive, const CRegPathSegment8 &path)
{
	CRegPathW pathRtn(hive);
	pathRtn.Append(path);
	return pathRtn;
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CRegPathW &path)
{
	stream << path.Get();
	return stream;
}

//#################################################################################################
bool CRegPathW::operator==(const CRegPathW &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_bRooted != path.m_bRooted)
		return false;

	// We need to compare each segment in the path
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();

	if(m_bRooted)
	{
		if(*(CHiveNameW*)itr1->get() != *(CHiveNameW*)itr2->get())
			return false;

		++itr1;
		++itr2;
	}

	while(itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if(**itr1 != **itr2)
			return false;

		++itr1;
		++itr2;
	}

	return true;
}

//#################################################################################################
bool CRegPathW::operator==(const CRegPathSegmentW &path) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
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
bool CRegPathW::operator==(const CHiveNameW &hive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
		return hive.IsEmpty();

	// Are they the same size, and is the path rooted?
	if(m_vecSegments.size() != 1 || !m_bRooted)
		return false;

	// We need to compare the segment
	if(hive != *m_vecSegments.front())
		return false;

	return true;
}

//#################################################################################################
bool CRegPathW::operator!=(const CRegPathW &path) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_bRooted != path.m_bRooted)
		return true;

	// We need to compare each segment in the path
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();

	if(m_bRooted)
	{
		if(*(CHiveNameW*)itr1->get() != *(CHiveNameW*)itr2->get())
			return true;

		++itr1;
		++itr2;
	}

	while(itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if(**itr1 != **itr2)
			return true;

		++itr1;
		++itr2;
	}

	return false;
}

//#################################################################################################
bool CRegPathW::operator!=(const CRegPathSegmentW &path) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
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
bool CRegPathW::operator!=(const CHiveNameW &hive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
		return !hive.IsEmpty();

	// Are they the same size, and is the path rooted?
	if(m_vecSegments.size() != 1 || !m_bRooted)
		return true;

	// We need to compare the segment
	if(hive != *m_vecSegments.front())
		return true;

	return false;
}

//#################################################################################################
bool CRegPathW::operator<(const CRegPathW &path) const
{
	return (Get().Compare(path.Get(), true) < 0);
}

//#################################################################################################
bool CRegPathW::operator<=(const CRegPathW &path) const
{
	return (Get().Compare(path.Get(), true) <= 0);
}

//#################################################################################################
bool CRegPathW::operator>(const CRegPathW &path) const
{
	return (Get().Compare(path.Get(), true) > 0);
}

//#################################################################################################
bool CRegPathW::operator>=(const CRegPathW &path) const
{
	return (Get().Compare(path.Get(), true) >= 0);
}

//#################################################################################################
bool CRegPathW::Compare(const CRegPathW &path, const bool bCaseInsensitive) const
{	// Are they the same size and type of path?
	if(m_vecSegments.size() != path.m_vecSegments.size() || m_bRooted != path.m_bRooted)
		return false;

	// We need to compare each segment in the path
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();

	if(m_bRooted)
	{
		if(*(CHiveNameW*)itr1->get() != *(CHiveNameW*)itr2->get())
			return false;

		++itr1;
		++itr2;
	}

	while(itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
			return false;

		++itr1;
		++itr2;
	}

	return true;
}

//#################################################################################################
bool CRegPathW::Compare(const CRegPathSegmentW &path, const bool bCaseInsensitive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
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
bool CRegPathW::Compare(const CHiveNameW &hive) const
{	// If the path is empty we have nothing to compare against
	if(m_vecSegments.empty())
		return hive.IsEmpty();

	// Are they the same size, and is the path rooted?
	if(m_vecSegments.size() != 1 || !m_bRooted)
		return false;

	// We need to compare the segment
	if(hive != *m_vecSegments.front())
		return false;

	return true;
}

//#################################################################################################
bool CRegPathW::CompareLeft(const CRegPathW &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// Are they the same type of path?
	if(m_bRooted != path.m_bRooted)
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	auto itr1 = m_vecSegments.begin();
	auto itr2 = path.m_vecSegments.begin();
	while(nCount != 0 && itr1 != m_vecSegments.end() && itr2 != path.m_vecSegments.end())
	{
		if(m_bRooted && (*itr1) == m_vecSegments.front())
		{
			if(((CHiveNameW*)itr1->get())->Compare(*(CHiveNameW*)itr2->get()) != 0)
				return false;
		}
		else
		{
			if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
				return false;
		}

		++itr1;
		++itr2;
		--nCount;
	}

	return true;
}

//#################################################################################################
bool CRegPathW::CompareRight(const CRegPathW &path, const size_t nSegmentCount, const bool bCaseInsensitive) const
{
	if(nSegmentCount == 0)
		return false;

	// Are they the same size (up to the compare size)?
	if((m_vecSegments.size() < nSegmentCount || path.m_vecSegments.size() < nSegmentCount) && m_vecSegments.size() != path.m_vecSegments.size())
		return false;

	// We need to compare each segment in the path
	size_t nCount = nSegmentCount;
	auto itr1 = m_vecSegments.rbegin();
	auto itr2 = path.m_vecSegments.rbegin();
	while(nCount != 0 && itr1 != m_vecSegments.rend() && itr2 != path.m_vecSegments.rend())
	{
		if(m_bRooted && (*itr1) == m_vecSegments.front())
		{
			if(((CHiveNameW*)itr1->get())->Compare(*(CHiveNameW*)itr2->get()) != 0)
				return false;
		}
		else
		{
			if((*itr1)->Compare(**itr2, bCaseInsensitive) != 0)
				return false;
		}
		++itr1;
		++itr2;
		--nCount;
	}

	return true;
}

//#################################################################################################
bool CRegPathW::Contains(const CRegPathW &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty() && m_vecSegments.size() >= path.m_vecSegments.size())
	{
		CRegPathW pathMid;
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
bool CRegPathW::Contains(const CRegPathSegmentW &path, const bool bCaseInsensitive) const
{
	bool bFound = false;

	if(!path.IsEmpty())
		bFound = std::any_of(m_vecSegments.begin(), m_vecSegments.end(), [&](const std::unique_ptr<CRegPathSegmentW> &segment) {return (segment->Compare(path, bCaseInsensitive) == 0);});

	return bFound;
}

//#################################################################################################
bool CRegPathW::Contains(const CHiveNameW &hive) const
{
	bool bFound = false;

	// We need to compare the segment
	if(!hive.IsEmpty() && hive == *m_vecSegments.front())
		bFound = true;

	return bFound;
}

//#################################################################################################
void CRegPathW::Empty(void)
{
	m_bRooted = false;
	m_vecSegments.clear();
}

//#################################################################################################
bool CRegPathW::IsEmpty(void) const noexcept
{
	return m_vecSegments.empty();
}

//#################################################################################################
bool CRegPathW::IsRoot(void) const noexcept
{
	return (m_bRooted && m_vecSegments.size() == 1);
}

//#################################################################################################
bool CRegPathW::IsRelative(void) const noexcept
{
	return (!m_bRooted && !m_vecSegments.empty());
}

//#################################################################################################
size_t CRegPathW::GetSegmentCount(void) const noexcept
{
	return m_vecSegments.size();
}

//#################################################################################################
CRegPathSegmentW CRegPathW::GetSegment(const size_t nSegment) const
{
	CRegPathSegmentW path;

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
CRegPathSegmentW CRegPathW::GetFirstSegment(void) const
{
	return (!m_vecSegments.empty()) ? CRegPathSegmentW(*m_vecSegments.front()) : CRegPathSegmentW();
}

//#################################################################################################
CRegPathSegmentW CRegPathW::GetLastSegment(void) const
{
	return (!m_vecSegments.empty()) ? CRegPathSegmentW(*m_vecSegments.back()) : CRegPathSegmentW();
}

//#################################################################################################
CRegPathW CRegPathW::GetLeft(const size_t nSegmentCount) const
{
	CRegPathW path(*this);
	path.CropLeft(nSegmentCount);

	return path;
}

//#################################################################################################
CRegPathW CRegPathW::GetRight(const size_t nSegmentCount) const
{
	CRegPathW path(*this);
	path.CropRight(nSegmentCount);

	return path;
}

//#################################################################################################
CRegPathW CRegPathW::GetMid(const size_t nSegment, const size_t nSegmentCount) const
{
	CRegPathW path;

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
			path.m_bRooted = m_bRooted;
	}

	return path;
}

//#################################################################################################
CRegPathW &CRegPathW::TrimLeft(const size_t nSegmentCount)
{
	if(nSegmentCount)
	{
		if(nSegmentCount >= m_vecSegments.size())
			Empty();
		else
		{	// If it was rooted it no longer is because we are removing the first segment
			m_bRooted = false;

			size_t nRemoved = 0;
			while(nRemoved++ < nSegmentCount)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
CRegPathW &CRegPathW::TrimRight(const size_t nSegmentCount)
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
CRegPathW &CRegPathW::CropLeft(const size_t nSegmentCount)
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
CRegPathW &CRegPathW::CropRight(const size_t nSegmentCount)
{
	if(nSegmentCount < m_vecSegments.size())
	{
		if(nSegmentCount == 0)
			Empty();
		else
		{	// If it was rooted it no longer is because we are removing the first segment
			m_bRooted = false;

			size_t nToRemove = m_vecSegments.size() - nSegmentCount;
			while(nToRemove--)
				m_vecSegments.erase(m_vecSegments.begin());
		}
	}

	return *this;
}

//#################################################################################################
ERRCODE CRegPathW::Insert(const size_t nSegment, const CRegPathW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if((nSegment == 0 && m_bRooted) || (nSegment && path.m_bRooted))
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			for(const auto &segment : path.m_vecSegments)
				m_vecSegments.insert(itr, std::make_unique<CRegPathSegmentW>(*segment));

			if(nSegment == 0)
				m_bRooted = path.m_bRooted;
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Insert(const size_t nSegment, const CRegPathSegmentW &path)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!path.IsEmpty())
	{
		if(nSegment == 0 && m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else if(m_vecSegments.empty())
			Assign(path);
		else
		{
			size_t nCount = nSegment;
			auto itr = m_vecSegments.begin();
			while(itr != m_vecSegments.end() && nCount-- != 0)
				++itr;

			m_vecSegments.insert(itr, std::make_unique<CRegPathSegmentW>(path));
		}
	}

	return nErrorCode;
}

//#################################################################################################
ERRCODE CRegPathW::Insert(const CHiveNameW &hive)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	if(!hive.IsEmpty())
	{
		if(m_bRooted)
			nErrorCode = FW_ERROR_INVALID_DATA;
		else
		{
			m_bRooted = true;
			m_vecSegments.insert(m_vecSegments.begin(), std::make_unique<CHiveNameW>(hive));
		}
	}

	return nErrorCode;
}

//#################################################################################################
CRegPathW &CRegPathW::Delete(const size_t nSegment, const size_t nSegmentCount)
{
	if(nSegment < m_vecSegments.size() && nSegmentCount != 0)
	{
		if(nSegment == 0)
			m_bRooted = false;

		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(itr != m_vecSegments.end() && nCount-- != 0)
			++itr;

		nCount = nSegmentCount;
		while(itr != m_vecSegments.end() && nCount-- != 0)
			itr = m_vecSegments.erase(itr);
	}

	return *this;
}

//#################################################################################################
size_t CRegPathW::Remove(const CRegPathSegmentW &path, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	auto itr = m_vecSegments.begin();
	while(itr != m_vecSegments.end())
	{
		if((*itr)->Compare(path, bCaseInsensitive) == 0)
		{
			if(itr == m_vecSegments.begin())
				m_bRooted = false;

			itr = m_vecSegments.erase(itr);
			++nCount;
			continue;
		}
		++itr;
	}

	return nCount;
}

//#################################################################################################
size_t CRegPathW::Replace(const CRegPathSegmentW &pathOld, const CRegPathSegmentW &pathNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!pathNew.IsEmpty())
	{
		for(auto &itr : m_vecSegments)
		{
			if(itr->Compare(pathOld, bCaseInsensitive) == 0)
			{
				itr = std::make_unique<CRegPathSegmentW>(pathNew);
				++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
CRegPathW &CRegPathW::Modify(const size_t nSegment, const CRegPathSegmentW &path)
{
	if(nSegment < m_vecSegments.size() && !path.IsEmpty())
	{
		size_t nCount = nSegment;
		auto itr = m_vecSegments.begin();
		while(nCount-- != 0)
			++itr;

		*itr = std::make_unique<CRegPathSegmentW>(path);
	}

	return *this;
}

//#################################################################################################
CStrW CRegPathW::GetLongPath(void) const
{	// Convert the vector of path segments into a single string
	CStrW str;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		if(m_bRooted)
			str.Assign(((CHiveNameW*)itr->get())->GetLongName());
		else
			str.Assign((*itr)->Get());

		while(++itr != m_vecSegments.end())
		{
			str.Append(g_chBackSlashW);
			str.Append((*itr)->Get());
		}
	}

	return str;
}

//#################################################################################################
CStrW CRegPathW::GetShortPath(void) const
{	// Convert the vector of path segments into a single string
	CStrW str;

	if(!m_vecSegments.empty())
	{
		auto itr = m_vecSegments.begin();
		if(m_bRooted)
			str.Assign(((CHiveNameW*)itr->get())->GetShortName());
		else
			str.Assign((*itr)->Get());

		while(++itr != m_vecSegments.end())
		{
			str.Append(g_chBackSlashW);
			str.Append((*itr)->Get());
		}
	}

	return str;
}

//#################################################################################################
CHiveNameW CRegPathW::GetHive(void) const
{
	return (m_bRooted && !m_vecSegments.empty()) ? CHiveNameW(*m_vecSegments.front()) : CHiveNameW();
}

//#################################################################################################
CRegPathW CRegPathW::GetRelativePath(void) const
{
	CRegPathW path(*this);

	if(path.m_bRooted)
		path.TrimLeft();

	return path;
}

//#################################################################################################
void CRegPathW::DebugPrint(const bool bAppendNewline) const
{
	CStrW str(Get());
	if(bAppendNewline)
		str.Append(g_szEolW);
	OutputDebugStringW(str);
}

//#################################################################################################
ERRCODE CRegPathW::ParsePath(const CStrW &strPath)
{
	ERRCODE nErrorCode = FW_NO_ERROR;

	Empty();

	if(strPath.IsEmpty())
		return nErrorCode;

	CStrW strSegment;
	PCWSTR szPath = strPath;
	PCWSTR szLastDelim = szPath;
	PCWSTR szNextDelim;
	size_t nDelimLen = strPath.GetLength();

	// Is it a kernel path?
	if(strPath.CompareLeft(g_szRegKernelMachineW, g_nKernelMachineLen, true) == 0)
	{
		m_bRooted = true;
		auto pHive = std::make_unique<CHiveNameW>(g_szHKLMW);
		if(pHive->IsEmpty())
			return FW_ERROR_INVALID_DATA;

		m_vecSegments.push_back(std::move(pHive));

		szLastDelim += g_nKernelMachineLen;
		nDelimLen -= g_nKernelMachineLen;
	}
	else if(strPath.CompareLeft(g_szRegKernelUserW, g_nKernelUserLen, true) == 0)
	{
		m_bRooted = true;
		auto pHive = std::make_unique<CHiveNameW>(g_szHKUW);
		if(pHive->IsEmpty())
			return FW_ERROR_INVALID_DATA;

		m_vecSegments.push_back(std::move(pHive));

		szLastDelim += g_nKernelUserLen;
		nDelimLen -= g_nKernelUserLen;
	}

	// Find first non-delimiter character
	while(*szLastDelim == g_chBackSlashW)
		++szLastDelim, --nDelimLen;

	do{
		szNextDelim = (PCWSTR)std::wmemchr(szLastDelim, g_chBackSlashW, nDelimLen);
		if(szNextDelim == nullptr)
		{	// No more delimiters, the rest of the string is the last segment
			if(m_vecSegments.empty() && CHiveNameW::IsValidHiveName(szLastDelim))
			{
				m_bRooted = true;
				auto pHive = std::make_unique<CHiveNameW>(szLastDelim);
				if(pHive->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pHive));
			}
			else
			{
				auto pPath = std::make_unique<CRegPathSegmentW>(szLastDelim);
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
			strSegment.Assign(strPath, (size_t)(szLastDelim - szPath), (size_t)(szNextDelim - szLastDelim));
			if(m_vecSegments.empty() && CHiveNameW::IsValidHiveName(strSegment))
			{
				m_bRooted = true;
				auto pHive = std::make_unique<CHiveNameW>(strSegment);
				if(pHive->IsEmpty())
				{
					Empty();
					nErrorCode = FW_ERROR_INVALID_DATA;
					break;
				}

				m_vecSegments.push_back(std::move(pHive));
			}
			else
			{
				auto pPath = std::make_unique<CRegPathSegmentW>(strSegment);
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
bool CRegPathW::SplitPath(PCWSTR szPath, CStrW &strHive, CStrW &strPath)
{
	bool bValid = false;

	strHive.Empty();
	strPath.Empty();

	CRegPathW path;
	if(path.Assign(szPath) == FW_NO_ERROR)
	{
		strHive.Assign(path.GetHive());
		strPath.Assign(path.GetRelativePath());
		bValid = true;
	}

	return bValid;
}

//#################################################################################################
bool CRegPathW::IsValidPath(const CStrW &strPath)
{
	bool bValid = true;

	CRegPathW path;
	if(path.Assign(strPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CRegPathW::IsValidPath(PCWSTR szPath)
{
	bool bValid = true;

	CRegPathW path;
	if(path.Assign(szPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

//#################################################################################################
bool CRegPathW::IsValidPath(const wchar_t chPath)
{
	bool bValid = true;

	CRegPathW path;
	if(path.Assign(chPath) != FW_NO_ERROR)
		bValid = false;

	return bValid;
}

NS_END

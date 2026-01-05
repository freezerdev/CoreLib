#pragma once

#include <vector>

NS_BEGIN

// Forward declarations
class CDomainName8;
class CDomainNameW;
class CUrlPathSegment8;
class CUrlPathSegmentW;
class CUrlParameter8;
class CUrlParameterW;
class CUrl8;
class CUrlW;

#ifdef _WIN32
// Windows headers interfere with these definitions
#undef SetPort
#endif

//#################################################################################################
class CDomainName8 final
{
private:
	friend class CDomainNameW;

public:
	// Constructs an empty object
	CDomainName8(void);
	// Copy and move constructors
	CDomainName8(const CDomainName8 &src);
	CDomainName8(CDomainName8 &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CDomainName8(const CDomainNameW &domain);
	explicit CDomainName8(const CStr8 &str);
	explicit CDomainName8(const CStrW &str);
	explicit CDomainName8(PCSTR sz);
	explicit CDomainName8(PCWSTR sz);
	explicit CDomainName8(const char ch);
	explicit CDomainName8(const wchar_t ch);

	~CDomainName8(void);

	// Returns a pointer to the raw string data
	operator PCSTR(void) const noexcept;
	PCSTR Get(void) const noexcept;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const noexcept;
	// Empties the string
	void Empty(void);
	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;

	// Returns a UTF8 copy of the object
	CDomainName8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CDomainNameW AsWide(void) const;
#ifdef _WIN32
	CDomainNameW AsNative(void) const;
#else
	CDomainName8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CDomainName8 &operator=(const CDomainName8 &src);
	CDomainName8 &operator=(CDomainName8 &&src) noexcept;
	// Assigns the object to the string/character
	CDomainName8 &operator=(const CDomainNameW &domain);
	CDomainName8 &operator=(const CStr8 &str);
	CDomainName8 &operator=(const CStrW &str);
	CDomainName8 &operator=(PCSTR sz);
	CDomainName8 &operator=(PCWSTR sz);
	CDomainName8 &operator=(const char ch);
	CDomainName8 &operator=(const wchar_t ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CDomainName8 &domain);
	ERRCODE Assign(const CDomainNameW &domain);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(PCSTR sz);
	ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(const char ch);
	ERRCODE Assign(const wchar_t ch);

	friend std::ostream &operator<<(std::ostream &stream, const CDomainName8 &domain);

	// Default compare operators are case insensitive
	// After all "GOOGLE.COM" and "google.com" are the same site
	// For case sensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CDomainName8 &domain) const;
	bool operator==(const CStr8 &str) const;
	bool operator==(PCSTR sz) const;
	bool operator==(const char ch) const;

	// Tests for not equal
	bool operator!=(const CDomainName8 &domain) const;
	bool operator!=(const CStr8 &str) const;
	bool operator!=(PCSTR sz) const;
	bool operator!=(const char ch) const;

	// Tests for less than
	bool operator<(const CDomainName8 &domain) const;
	bool operator<(const CStr8 &str) const;
	bool operator<(PCSTR sz) const;
	bool operator<(const char ch) const;

	// Tests for less than or equal
	bool operator<=(const CDomainName8 &domain) const;
	bool operator<=(const CStr8 &str) const;
	bool operator<=(PCSTR sz) const;
	bool operator<=(const char ch) const;

	// Tests for greater than
	bool operator>(const CDomainName8 &domain) const;
	bool operator>(const CStr8 &str) const;
	bool operator>(PCSTR sz) const;
	bool operator>(const char ch) const;

	// Tests for greater than or equal
	bool operator>=(const CDomainName8 &domain) const;
	bool operator>=(const CStr8 &str) const;
	bool operator>=(PCSTR sz) const;
	bool operator>=(const char ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CDomainName8 &domain, const bool bCaseInsensitive = true) const;
	int Compare(const CStr8 &str, const bool bCaseInsensitive = true) const;
	int Compare(PCSTR sz, const bool bCaseInsensitive = true) const;
	int Compare(const char ch, const bool bCaseInsensitive = true) const;

	// Returns whether the string/character is a valid domain name
	static bool IsValidDomainName(const CStr8 &strDomainName);
	static bool IsValidDomainName(PCSTR szDomainName);
	static bool IsValidDomainName(const char chDomainName);

private:
	PSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);

	static PCSTR FindStrPtr(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
	static PCSTR FindFirstNotOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
};


//#################################################################################################
class CDomainNameW final
{
private:
	friend class CDomainName8;

public:
	// Constructs an empty object
	CDomainNameW(void);
	// Copy and move constructors
	CDomainNameW(const CDomainNameW &src);
	CDomainNameW(CDomainNameW &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CDomainNameW(const CDomainName8 &domain);
	explicit CDomainNameW(const CStrW &str);
	explicit CDomainNameW(const CStr8 &str);
	explicit CDomainNameW(PCWSTR sz);
	explicit CDomainNameW(PCSTR sz);
	explicit CDomainNameW(const wchar_t ch);
	explicit CDomainNameW(const char ch);

	~CDomainNameW(void);

	// Returns a pointer to the raw string data
	operator PCWSTR(void) const noexcept;
	PCWSTR Get(void) const noexcept;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const noexcept;
	// Empties the string
	void Empty(void);
	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;

	// Returns a UTF8 copy of the object
	CDomainName8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CDomainNameW AsWide(void) const;
#ifdef _WIN32
	CDomainNameW AsNative(void) const;
#else
	CDomainName8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CDomainNameW &operator=(const CDomainNameW &src);
	CDomainNameW &operator=(CDomainNameW &&src) noexcept;
	// Assigns the object to the string/character
	CDomainNameW &operator=(const CDomainName8 &domain);
	CDomainNameW &operator=(const CStrW &str);
	CDomainNameW &operator=(const CStr8 &str);
	CDomainNameW &operator=(PCWSTR sz);
	CDomainNameW &operator=(PCSTR sz);
	CDomainNameW &operator=(const wchar_t ch);
	CDomainNameW &operator=(const char ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CDomainNameW &domain);
	ERRCODE Assign(const CDomainName8 &domain);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(PCSTR sz);
	ERRCODE Assign(const wchar_t ch);
	ERRCODE Assign(const char ch);

	friend std::wostream &operator<<(std::wostream &stream, const CDomainNameW &domain);

	// Default compare operators are case insensitive
	// After all "GOOGLE.COM" and "google.com" are the same site
	// For case sensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CDomainNameW &domain) const;
	bool operator==(const CStrW &str) const;
	bool operator==(PCWSTR sz) const;
	bool operator==(const wchar_t ch) const;

	// Tests for not equal
	bool operator!=(const CDomainNameW &domain) const;
	bool operator!=(const CStrW &str) const;
	bool operator!=(PCWSTR sz) const;
	bool operator!=(const wchar_t ch) const;

	// Tests for less than
	bool operator<(const CDomainNameW &domain) const;
	bool operator<(const CStrW &str) const;
	bool operator<(PCWSTR sz) const;
	bool operator<(const wchar_t ch) const;

	// Tests for less than or equal
	bool operator<=(const CDomainNameW &domain) const;
	bool operator<=(const CStrW &str) const;
	bool operator<=(PCWSTR sz) const;
	bool operator<=(const wchar_t ch) const;

	// Tests for greater than
	bool operator>(const CDomainNameW &domain) const;
	bool operator>(const CStrW &str) const;
	bool operator>(PCWSTR sz) const;
	bool operator>(const wchar_t ch) const;

	// Tests for greater than or equal
	bool operator>=(const CDomainNameW &domain) const;
	bool operator>=(const CStrW &str) const;
	bool operator>=(PCWSTR sz) const;
	bool operator>=(const wchar_t ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CDomainNameW &domain, const bool bCaseInsensitive = true) const;
	int Compare(const CStrW &str, const bool bCaseInsensitive = true) const;
	int Compare(PCWSTR sz, const bool bCaseInsensitive = true) const;
	int Compare(const wchar_t ch, const bool bCaseInsensitive = true) const;

	// Returns whether the string/character is a valid domain name
	static bool IsValidDomainName(const CStrW &strDomainName);
	static bool IsValidDomainName(PCWSTR szDomainName);
	static bool IsValidDomainName(const wchar_t chDomainName);

private:
	PWSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);

	static PCWSTR FindStrPtr(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
	static PCWSTR FindFirstNotOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
};


//#################################################################################################
class CUrlPathSegment8 final
{
private:
	friend class CUrlPathSegmentW;

public:
	// Constructs an empty object
	CUrlPathSegment8(void);
	// Copy and move constructors
	CUrlPathSegment8(const CUrlPathSegment8 &src);
	CUrlPathSegment8(CUrlPathSegment8 &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CUrlPathSegment8(const CUrlPathSegmentW &path);
	explicit CUrlPathSegment8(const CStr8 &str, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegment8(const CStrW &str, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegment8(PCSTR sz, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegment8(PCWSTR sz, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegment8(const char ch);
	explicit CUrlPathSegment8(const wchar_t ch);

	~CUrlPathSegment8(void);

	// Returns a pointer to the raw string data
	operator PCSTR(void) const noexcept;
	PCSTR Get(void) const noexcept;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const noexcept;
	// Empties the string
	void Empty(void);
	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;

	// Returns a UTF8 copy of the object
	CUrlPathSegment8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CUrlPathSegmentW AsWide(void) const;
#ifdef _WIN32
	CUrlPathSegmentW AsNative(void) const;
#else
	CUrlPathSegment8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CUrlPathSegment8 &operator=(const CUrlPathSegment8 &src);
	CUrlPathSegment8 &operator=(CUrlPathSegment8 &&src) noexcept;
	// Assigns the object to the string/character
	CUrlPathSegment8 &operator=(const CUrlPathSegmentW &path);
	CUrlPathSegment8 &operator=(const CStr8 &str);
	CUrlPathSegment8 &operator=(const CStrW &str);
	CUrlPathSegment8 &operator=(PCSTR sz);
	CUrlPathSegment8 &operator=(PCWSTR sz);
	CUrlPathSegment8 &operator=(const char ch);
	CUrlPathSegment8 &operator=(const wchar_t ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CUrlPathSegment8 &path);
	ERRCODE Assign(const CUrlPathSegmentW &path);
	ERRCODE Assign(const CStr8 &str, const bool bAlreadyEscaped = false);
	ERRCODE Assign(const CStrW &str, const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCSTR sz, const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCWSTR sz, const bool bAlreadyEscaped = false);
	ERRCODE Assign(const char ch);
	ERRCODE Assign(const wchar_t ch);

	friend std::ostream &operator<<(std::ostream &stream, const CUrlPathSegment8 &path);

	// Default compare operators are case sensitive since
	// "Downloads" and "downloads" are different paths
	// For case insensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CUrlPathSegment8 &path) const;
	bool operator==(const CStr8 &str) const;
	bool operator==(PCSTR sz) const;
	bool operator==(const char ch) const;

	// Tests for not equal
	bool operator!=(const CUrlPathSegment8 &path) const;
	bool operator!=(const CStr8 &str) const;
	bool operator!=(PCSTR sz) const;
	bool operator!=(const char ch) const;

	// Tests for less than
	bool operator<(const CUrlPathSegment8 &path) const;
	bool operator<(const CStr8 &str) const;
	bool operator<(PCSTR sz) const;
	bool operator<(const char ch) const;

	// Tests for less than or equal
	bool operator<=(const CUrlPathSegment8 &path) const;
	bool operator<=(const CStr8 &str) const;
	bool operator<=(PCSTR sz) const;
	bool operator<=(const char ch) const;

	// Tests for greater than
	bool operator>(const CUrlPathSegment8 &path) const;
	bool operator>(const CStr8 &str) const;
	bool operator>(PCSTR sz) const;
	bool operator>(const char ch) const;

	// Tests for greater than or equal
	bool operator>=(const CUrlPathSegment8 &path) const;
	bool operator>=(const CStr8 &str) const;
	bool operator>=(PCSTR sz) const;
	bool operator>=(const char ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CUrlPathSegment8 &path, const bool bCaseInsensitive = false) const;
	int Compare(const CStr8 &str, const bool bCaseInsensitive = false) const;
	int Compare(PCSTR sz, const bool bCaseInsensitive = false) const;
	int Compare(const char ch, const bool bCaseInsensitive = false) const;

	// Returns whether the string/character is a valid path segment
	static bool IsValidSegment(const CStr8 &strSegment);
	static bool IsValidSegment(PCSTR szSegment);
	static bool IsValidSegment(const char chSegment) noexcept;

private:
	PSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);
};


//#################################################################################################
class CUrlPathSegmentW final
{
private:
	friend class CUrlPathSegment8;

public:
	// Constructs an empty object
	CUrlPathSegmentW(void);
	// Copy and move constructors
	CUrlPathSegmentW(const CUrlPathSegmentW &src);
	CUrlPathSegmentW(CUrlPathSegmentW &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CUrlPathSegmentW(const CUrlPathSegment8 &path);
	explicit CUrlPathSegmentW(const CStrW &str, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegmentW(const CStr8 &str, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegmentW(PCWSTR sz, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegmentW(PCSTR sz, const bool bAlreadyEscaped = false);
	explicit CUrlPathSegmentW(const wchar_t ch);
	explicit CUrlPathSegmentW(const char ch);

	~CUrlPathSegmentW(void);

	// Returns a pointer to the raw string data
	operator PCWSTR(void) const noexcept;
	PCWSTR Get(void) const noexcept;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const noexcept;
	// Empties the string
	void Empty(void);
	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;

	// Returns a UTF8 copy of the object
	CUrlPathSegment8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CUrlPathSegmentW AsWide(void) const;
#ifdef _WIN32
	CUrlPathSegmentW AsNative(void) const;
#else
	CUrlPathSegment8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CUrlPathSegmentW &operator=(const CUrlPathSegmentW &src);
	CUrlPathSegmentW &operator=(CUrlPathSegmentW &&src) noexcept;
	// Assigns the object to the string/character
	CUrlPathSegmentW &operator=(const CUrlPathSegment8 &path);
	CUrlPathSegmentW &operator=(const CStrW &str);
	CUrlPathSegmentW &operator=(const CStr8 &str);
	CUrlPathSegmentW &operator=(PCWSTR sz);
	CUrlPathSegmentW &operator=(PCSTR sz);
	CUrlPathSegmentW &operator=(const wchar_t ch);
	CUrlPathSegmentW &operator=(const char ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CUrlPathSegmentW &path);
	ERRCODE Assign(const CUrlPathSegment8 &path);
	ERRCODE Assign(const CStrW &str, const bool bAlreadyEscaped = false);
	ERRCODE Assign(const CStr8 &str, const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCWSTR sz, const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCSTR sz, const bool bAlreadyEscaped = false);
	ERRCODE Assign(const wchar_t ch);
	ERRCODE Assign(const char ch);

	friend std::wostream &operator<<(std::wostream &stream, const CUrlPathSegmentW &path);

	// Default compare operators are case sensitive since
	// "Downloads" and "downloads" are different paths
	// For case insensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CUrlPathSegmentW &path) const;
	bool operator==(const CStrW &str) const;
	bool operator==(PCWSTR sz) const;
	bool operator==(const wchar_t ch) const;

	// Tests for not equal
	bool operator!=(const CUrlPathSegmentW &path) const;
	bool operator!=(const CStrW &str) const;
	bool operator!=(PCWSTR sz) const;
	bool operator!=(const wchar_t ch) const;

	// Tests for less than
	bool operator<(const CUrlPathSegmentW &path) const;
	bool operator<(const CStrW &str) const;
	bool operator<(PCWSTR sz) const;
	bool operator<(const wchar_t ch) const;

	// Tests for less than or equal
	bool operator<=(const CUrlPathSegmentW &path) const;
	bool operator<=(const CStrW &str) const;
	bool operator<=(PCWSTR sz) const;
	bool operator<=(const wchar_t ch) const;

	// Tests for greater than
	bool operator>(const CUrlPathSegmentW &path) const;
	bool operator>(const CStrW &str) const;
	bool operator>(PCWSTR sz) const;
	bool operator>(const wchar_t ch) const;

	// Tests for greater than or equal
	bool operator>=(const CUrlPathSegmentW &path) const;
	bool operator>=(const CStrW &str) const;
	bool operator>=(PCWSTR sz) const;
	bool operator>=(const wchar_t ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CUrlPathSegmentW &path, const bool bCaseInsensitive = false) const;
	int Compare(const CStrW &str, const bool bCaseInsensitive = false) const;
	int Compare(PCWSTR sz, const bool bCaseInsensitive = false) const;
	int Compare(const wchar_t ch, const bool bCaseInsensitive = false) const;

	// Returns whether the string/character is a valid path segment
	static bool IsValidSegment(const CStrW &strSegment);
	static bool IsValidSegment(PCWSTR szSegment);
	static bool IsValidSegment(const wchar_t chSegment) noexcept;

private:
	PWSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);
};


//#################################################################################################
class CUrlParameter8 final
{
private:
	friend class CUrlParameterW;

public:
	// Constructs an empty object
	CUrlParameter8(void);
	// Copy and move constructors
	CUrlParameter8(const CUrlParameter8 &src);
	CUrlParameter8(CUrlParameter8 &&src) noexcept;
	// Constructs an object set to the name and value pair
	explicit CUrlParameter8(const CUrlParameterW &param);
	explicit CUrlParameter8(const CStr8 &strName, const CStr8 &strValue = CStr8(g_szNull8), const bool bAlreadyEscaped = false);
	explicit CUrlParameter8(const CStrW &strName, const CStrW &strValue = CStrW(g_szNullW), const bool bAlreadyEscaped = false);
	explicit CUrlParameter8(PCSTR szName, PCSTR szValue = nullptr, const bool bAlreadyEscaped = false);
	explicit CUrlParameter8(PCWSTR szName, PCWSTR szValue = nullptr, const bool bAlreadyEscaped = false);

	~CUrlParameter8(void);

	// Returns a pointer to the combined string name and value
	operator CStr8(void) const;
	CStr8 Get(const bool bAlwaysIncludeEqual = false) const;
	// Returns the length in characters
	size_t GetLength(const bool bAlwaysIncludeEqual = false, const bool bIncludeNullTerm = false) const;
	// Returns the size in bytes
	size_t GetSize(const bool bAlwaysIncludeEqual = false, const bool bIncludeNullTerm = false) const;

	// Returns a pointer to the raw string name
	PCSTR GetName(void) const noexcept;
	// Returns the length in characters of the name
	size_t GetNameLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes of the name
	size_t GetNameSize(const bool bIncludeNullTerm = false) const noexcept;

	// Returns a pointer to the raw string value
	PCSTR GetValue(void) const noexcept;
	// Returns the length in characters of the value
	size_t GetValueLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes of the value
	size_t GetValueSize(const bool bIncludeNullTerm = false) const noexcept;

	// Empties the strings
	void Empty(void);
	// Returns whether the name string is empty
	bool IsEmpty(void) const noexcept;
	// Returns whether the value string is empty
	bool IsValueEmpty(void) const noexcept;

	// Returns a UTF8 copy of the object
	CUrlParameter8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CUrlParameterW AsWide(void) const;
#ifdef _WIN32
	CUrlParameterW AsNative(void) const;
#else
	CUrlParameter8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CUrlParameter8 &operator=(const CUrlParameter8 &src);
	CUrlParameter8 &operator=(CUrlParameter8 &&src) noexcept;
	// Assigns the object to the name and value pair
	CUrlParameter8 &operator=(const CUrlParameterW &param);

	// Assigns the object to the name and value pair, returns an error code
	ERRCODE Assign(const CUrlParameter8 &param);
	ERRCODE Assign(const CUrlParameterW &param);
	ERRCODE Assign(const CStr8 &strName, const CStr8 &strValue = CStr8(g_szNull8), const bool bAlreadyEscaped = false);
	ERRCODE Assign(const CStrW &strName, const CStrW &strValue = CStrW(g_szNullW), const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCSTR szName, PCSTR szValue = nullptr, const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCWSTR szName, PCWSTR szValue = nullptr, const bool bAlreadyEscaped = false);

	// Assigns the object to the name, returns an error code
	ERRCODE SetName(const CUrlParameter8 &param);
	ERRCODE SetName(const CUrlParameterW &param);
	ERRCODE SetName(const CStr8 &strName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(const CStrW &strName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(PCSTR szName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(PCWSTR szName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(const char chName);
	ERRCODE SetName(const wchar_t chName);

	// Assigns the object to the value, returns an error code
	ERRCODE SetValue(const CUrlParameter8 &param);
	ERRCODE SetValue(const CUrlParameterW &param);
	ERRCODE SetValue(const CStr8 &strValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(const CStrW &strValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(PCSTR szValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(PCWSTR szValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(const char chValue);
	ERRCODE SetValue(const wchar_t chValue);

	// Deletes the value
	void DeleteValue(void);

	friend std::ostream &operator<<(std::ostream &stream, const CUrlParameter8 &param);

	// Default compare operators are case sensitive since
	// "Downloads" and "downloads" are different paths
	// For case insensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CUrlParameter8 &param) const;

	// Tests for not equal
	bool operator!=(const CUrlParameter8 &param) const;

	// Tests for less than
	bool operator<(const CUrlParameter8 &param) const;

	// Tests for less than or equal
	bool operator<=(const CUrlParameter8 &param) const;

	// Tests for greater than
	bool operator>(const CUrlParameter8 &param) const;

	// Tests for greater than or equal
	bool operator>=(const CUrlParameter8 &param) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CUrlParameter8 &param, const bool bCaseInsensitive = false) const;
	int Compare(const CStr8 &strName, const CStr8 &strValue, const bool bCaseInsensitive = false) const;
	int Compare(PCSTR szName, PCSTR szValue, const bool bCaseInsensitive = false) const;

	// Returns whether the string/character is a valid parameter
	static bool IsValidParameter(const CStr8 &strParam);
	static bool IsValidParameter(PCSTR szParam);
	static bool IsValidParameter(const char chParam);

private:
	PSTR m_szName;			// Name string buffer
	size_t m_nNameBufLen;	// Length of the name buffer in characters
	size_t m_nNameStrLen;	// Length of the name string in characters

	PSTR m_szValue;		// Value string buffer
	size_t m_nValueBufLen;	// Length of the value buffer in characters
	size_t m_nValueStrLen;	// Length of the value string in characters

	bool AllocName(const size_t nBufLen);
	void DeallocName(void);

	bool AllocValue(const size_t nBufLen);
	void DeallocValue(void);

	static PCSTR FindFirstOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
};


//#################################################################################################
class CUrlParameterW final
{
private:
	friend class CUrlParameter8;

public:
	// Constructs an empty object
	CUrlParameterW(void);
	// Copy and move constructors
	CUrlParameterW(const CUrlParameterW &src);
	CUrlParameterW(CUrlParameterW &&src) noexcept;
	// Constructs an object set to the name and value pair
	explicit CUrlParameterW(const CUrlParameter8 &param);
	explicit CUrlParameterW(const CStrW &strName, const CStrW &strValue = CStrW(g_szNullW), const bool bAlreadyEscaped = false);
	explicit CUrlParameterW(const CStr8 &strName, const CStr8 &strValue = CStr8(g_szNull8), const bool bAlreadyEscaped = false);
	explicit CUrlParameterW(PCWSTR szName, PCWSTR szValue = nullptr, const bool bAlreadyEscaped = false);
	explicit CUrlParameterW(PCSTR szName, PCSTR szValue = nullptr, const bool bAlreadyEscaped = false);

	~CUrlParameterW(void);

	// Returns a pointer to the combined string name and value
	operator CStrW(void) const;
	CStrW Get(const bool bAlwaysIncludeEqual = false) const;
	// Returns the length in characters
	size_t GetLength(const bool bAlwaysIncludeEqual = false, const bool bIncludeNullTerm = false) const;
	// Returns the size in bytes
	size_t GetSize(const bool bAlwaysIncludeEqual = false, const bool bIncludeNullTerm = false) const;

	// Returns a pointer to the raw string name
	PCWSTR GetName(void) const noexcept;
	// Returns the length in characters of the name
	size_t GetNameLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes of the name
	size_t GetNameSize(const bool bIncludeNullTerm = false) const noexcept;

	// Returns a pointer to the raw string value
	PCWSTR GetValue(void) const noexcept;
	// Returns the length in characters of the value
	size_t GetValueLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes of the value
	size_t GetValueSize(const bool bIncludeNullTerm = false) const noexcept;

	// Empties the strings
	void Empty(void);
	// Returns whether the name string is empty
	bool IsEmpty(void) const noexcept;
	// Returns whether the value string is empty
	bool IsValueEmpty(void) const noexcept;

	// Returns a UTF8 copy of the object
	CUrlParameter8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CUrlParameterW AsWide(void) const;
#ifdef _WIN32
	CUrlParameterW AsNative(void) const;
#else
	CUrlParameter8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CUrlParameterW &operator=(const CUrlParameterW &src);
	CUrlParameterW &operator=(CUrlParameterW &&src) noexcept;
	// Assigns the object to the name and value pair
	CUrlParameterW &operator=(const CUrlParameter8 &param);

	// Assigns the object to the name and value pair, returns an error code
	ERRCODE Assign(const CUrlParameterW &param);
	ERRCODE Assign(const CUrlParameter8 &param);
	ERRCODE Assign(const CStrW &strName, const CStrW &strValue = CStrW(g_szNullW), const bool bAlreadyEscaped = false);
	ERRCODE Assign(const CStr8 &strName, const CStr8 &strValue = CStr8(g_szNull8), const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCWSTR szName, PCWSTR szValue = nullptr, const bool bAlreadyEscaped = false);
	ERRCODE Assign(PCSTR szName, PCSTR szValue = nullptr, const bool bAlreadyEscaped = false);

	// Assigns the object to the name, returns an error code
	ERRCODE SetName(const CUrlParameterW &param);
	ERRCODE SetName(const CUrlParameter8 &param);
	ERRCODE SetName(const CStrW &strName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(const CStr8 &strName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(PCWSTR szName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(PCSTR szName, const bool bAlreadyEscaped = false);
	ERRCODE SetName(const wchar_t chName);
	ERRCODE SetName(const char chName);

	// Assigns the object to the value, returns an error code
	ERRCODE SetValue(const CUrlParameterW &param);
	ERRCODE SetValue(const CUrlParameter8 &param);
	ERRCODE SetValue(const CStrW &strValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(const CStr8 &strValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(PCWSTR szValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(PCSTR szValue, const bool bAlreadyEscaped = false);
	ERRCODE SetValue(const wchar_t chValue);
	ERRCODE SetValue(const char chValue);

	// Deletes the value
	void DeleteValue(void);

	friend std::wostream &operator<<(std::wostream &stream, const CUrlParameterW &param);

	// Default compare operators are case sensitive since
	// "Downloads" and "downloads" are different paths
	// For case insensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CUrlParameterW &param) const;

	// Tests for not equal
	bool operator!=(const CUrlParameterW &param) const;

	// Tests for less than
	bool operator<(const CUrlParameterW &param) const;

	// Tests for less than or equal
	bool operator<=(const CUrlParameterW &param) const;

	// Tests for greater than
	bool operator>(const CUrlParameterW &param) const;

	// Tests for greater than or equal
	bool operator>=(const CUrlParameterW &param) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CUrlParameterW &param, const bool bCaseInsensitive = false) const;
	int Compare(const CStrW &strName, const CStrW &strValue, const bool bCaseInsensitive = false) const;
	int Compare(PCWSTR szName, PCWSTR szValue, const bool bCaseInsensitive = false) const;

	// Returns whether the string/character is a valid parameter
	static bool IsValidParameter(const CStrW &strParam);
	static bool IsValidParameter(PCWSTR szParam);
	static bool IsValidParameter(const wchar_t chParam);

private:
	PWSTR m_szName;			// Name string buffer
	size_t m_nNameBufLen;	// Length of the name buffer in characters
	size_t m_nNameStrLen;	// Length of the name string in characters

	PWSTR m_szValue;		// Value string buffer
	size_t m_nValueBufLen;	// Length of the value buffer in characters
	size_t m_nValueStrLen;	// Length of the value string in characters

	bool AllocName(const size_t nBufLen);
	void DeallocName(void);

	bool AllocValue(const size_t nBufLen);
	void DeallocValue(void);

	static PCWSTR FindFirstOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
};


//#################################################################################################
class CUrl8 final
{
private:
	friend class CUrlW;

public:
	static const uint32_t no_port;

	// Constructs an empty object
	CUrl8(void);
	// Copy and move constructors
	CUrl8(const CUrl8 &src);
	CUrl8(CUrl8 &&src) noexcept;
	// Constructs an object set to the path/string
	explicit CUrl8(const CUrlW &url);
	explicit CUrl8(const CDomainName8 &domain);
	explicit CUrl8(const CDomainNameW &domain);
	explicit CUrl8(const CStr8 &str);
	explicit CUrl8(const CStrW &str);
	explicit CUrl8(PCSTR sz);
	explicit CUrl8(PCWSTR sz);

	// Returns a string representing the full path
	operator CStr8(void) const;
	CStr8 Get(const bool bIncludeScheme = true, const bool bIncludeUsername = true, const bool bIncludePassword = true, const bool bIncludePort = true, const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false, const bool bIncludeScheme = true, const bool bIncludeUsername = true, const bool bIncludePassword = true, const bool bIncludePort = true, const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false, const bool bIncludeScheme = true, const bool bIncludeUsername = true, const bool bIncludePassword = true, const bool bIncludePort = true, const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;

	// Copy and move assignment operators
	CUrl8 &operator=(const CUrl8 &src);
	CUrl8 &operator=(CUrl8 &&src) noexcept;
	// Assigns the object to the path/string
	CUrl8 &operator=(const CUrlW &url);
	CUrl8 &operator=(const CDomainName8 &domain);
	CUrl8 &operator=(const CDomainNameW &domain);
	CUrl8 &operator=(const CStr8 &str);
	CUrl8 &operator=(const CStrW &str);
	CUrl8 &operator=(PCSTR sz);
	CUrl8 &operator=(PCWSTR sz);

	// Assigns the object to the path/string, returns an error code
	ERRCODE Assign(const CUrl8 &url);
	ERRCODE Assign(const CUrlW &url);
	ERRCODE Assign(const CDomainName8 &domain);
	ERRCODE Assign(const CDomainNameW &domain);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(PCSTR sz);
	ERRCODE Assign(PCWSTR sz);

	// Appends the path
	CUrl8 &operator+=(const CDomainName8 &domain);
	CUrl8 &operator+=(const CDomainNameW &domain);
	CUrl8 &operator+=(const CUrlPathSegment8 &path);
	CUrl8 &operator+=(const CUrlPathSegmentW &path);
	CUrl8 &operator+=(const CUrlParameter8 &param);
	CUrl8 &operator+=(const CUrlParameterW &param);

	// Appends the path, returns an error code
	ERRCODE Append(const CDomainName8 &domain);
	ERRCODE Append(const CDomainNameW &domain);
	ERRCODE Append(const CUrlPathSegment8 &path);
	ERRCODE Append(const CUrlPathSegmentW &path);
	ERRCODE Append(const CUrlParameter8 &param);
	ERRCODE Append(const CUrlParameterW &param);

#ifndef _WIN32
	// Returns the concatenation of two paths
	friend CUrl8 operator+(const CUrl8 &url, const CUrlPathSegment8 &path);
	friend CUrl8 operator+(const CUrl8 &url, const CUrlPathSegmentW &path);
	friend CUrl8 operator+(const CUrlW &url, const CUrlPathSegment8 &path);
	friend CUrl8 operator+(const CUrlW &url, const CUrlPathSegmentW &path);
	friend CUrl8 operator+(const CUrl8 &url, const CUrlParameter8 &param);
	friend CUrl8 operator+(const CUrl8 &url, const CUrlParameterW &param);
	friend CUrl8 operator+(const CUrlW &url, const CUrlParameter8 &param);
	friend CUrl8 operator+(const CUrlW &url, const CUrlParameterW &param);
	friend CUrl8 operator+(const CDomainName8 &domain, const CUrlPathSegment8 &path);
	friend CUrl8 operator+(const CDomainName8 &domain, const CUrlPathSegmentW &path);
	friend CUrl8 operator+(const CDomainNameW &domain, const CUrlPathSegment8 &path);
	friend CUrl8 operator+(const CDomainNameW &domain, const CUrlPathSegmentW &path);
	friend CUrl8 operator+(const CDomainName8 &domain, const CUrlParameter8 &param);
	friend CUrl8 operator+(const CDomainName8 &domain, const CUrlParameterW &param);
	friend CUrl8 operator+(const CDomainNameW &domain, const CUrlParameter8 &param);
	friend CUrl8 operator+(const CDomainNameW &domain, const CUrlParameterW &param);
#endif

	friend std::ostream &operator<<(std::ostream &stream, const CUrl8 &url);

	// Default compare operators are case sensitive since
	// "Downloads" and "downloads" are different paths
	// For case insensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CUrl8 &url) const;

	// Tests for not equal
	bool operator!=(const CUrl8 &url) const;

	// Tests for less than (case sensitive)
	bool operator<(const CUrl8 &url) const;

	// Tests for less than or equal (case sensitive)
	bool operator<=(const CUrl8 &url) const;

	// Tests for greater than (case sensitive)
	bool operator>(const CUrl8 &url) const;

	// Tests for greater than or equal (case sensitive)
	bool operator>=(const CUrl8 &url) const;

	// Compares the object against the path, optionally case insensitive
	bool Compare(const CUrl8 &url, const bool bCaseInsensitive = false) const;

	// Empties the path object, freeing the memory
	void Empty(void);
	// Returns whether the path is empty
	bool IsEmpty(void) const noexcept;

	// Gets/sets the Url scheme, for example 'https'
	const CStr8 &GetScheme(void) const noexcept;
	void SetScheme(const CStr8 &strScheme);

	// Gets/sets the username embedded in the Url
	const CStr8 &GetUsername(void) const noexcept;
	void SetUsername(const CStr8 &strUsername);

	// Gets/sets the password embedded in the Url
	const CStr8 &GetPassword(void) const noexcept;
	void SetPassword(const CStr8 &strPassword);

	// Gets/sets the domain, for example 'google.com'
	const CDomainName8 &GetDomain(void) const noexcept;
	void SetDomain(const CDomainName8 &domain);

	// Gets/sets the port number, or -1 for no port
	uint32_t GetPort(void) const noexcept;
	void SetPort(const uint32_t nPort);

	// Returns the number of segments in the path
	size_t GetSegmentCount(void) const noexcept;
	// Returns the segment at a given location, or an empty object if beyond the end of the path
	CUrlPathSegment8 GetSegment(const size_t nSegment) const;
	// Returns the first segment in the path, or an empty object if the path is empty
	CUrlPathSegment8 GetFirstSegment(void) const;
	// Returns the last segment in the path, or an empty object if the path is empty
	CUrlPathSegment8 GetLastSegment(void) const;
	// Removes the leftmost 'nSegmentCount' from the path object
	CUrl8 &TrimLeftSegment(const size_t nSegmentCount = 1);
	// Removes the rightmost 'nSegmentCount' from the path object
	CUrl8 &TrimRightSegment(const size_t nSegmentCount = 1);
	// Removes everything except the leftmost 'nSegmentCount' from the path object
	CUrl8 &CropLeftSegment(const size_t nSegmentCount = 1);
	// Removes everything except the rightmost 'nSegmentCount' from the path object
	CUrl8 &CropRightSegment(const size_t nSegmentCount = 1);
	// Inserts the given path into the path object before the given segment, returns an error code
	ERRCODE InsertSegment(const size_t nSegment, const CUrlPathSegment8 &path);
	// Removes 'nSegmentCount' segments from the path object starting at 'nSegment'
	CUrl8 &DeleteSegment(const size_t nSegment, const size_t nSegmentCount = 1);
	// Removes all segments from the path object that match the given path segment, returns the number of segments removed
	size_t RemoveSegment(const CUrlPathSegment8 &path, const bool bCaseInsensitive = false);
	// Replaces all segments from the path with the new path segment, returns the number of segments replaced
	size_t ReplaceSegment(const CUrlPathSegment8 &pathOld, const CUrlPathSegment8 &pathNew, const bool bCaseInsensitive = false);
	// Replaces segment 'nSegment' with the give path segment
	CUrl8 &ModifySegment(const size_t nSegment, const CUrlPathSegment8 &path);

	// Returns the number of parameters in the path
	size_t GetParameterCount(void) const noexcept;
	// Returns the parameter at a given location, or an empty object if beyond the end of the path
	CUrlParameter8 GetParameter(const size_t nParam) const;
	// Returns the first parameter in the path, or an empty object if the path is empty
	CUrlParameter8 GetFirstParameter(void) const;
	// Returns the last parameter in the path, or an empty object if the path is empty
	CUrlParameter8 GetLastParameter(void) const;
	// Removes the leftmost 'nParamCount' from the path object
	CUrl8 &TrimLeftParameter(const size_t nParamCount = 1);
	// Removes the rightmost 'nParamCount' from the path object
	CUrl8 &TrimRightParameter(const size_t nParamCount = 1);
	// Removes everything except the leftmost 'nParamCount' from the path object
	CUrl8 &CropLeftParameter(const size_t nParamCount = 1);
	// Removes everything except the rightmost 'nParamCount' from the path object
	CUrl8 &CropRightParameter(const size_t nParamCount = 1);
	// Inserts the given parameter into the path object before the given parameter, returns an error code
	ERRCODE InsertParameter(const size_t nParam, const CUrlParameter8 &param);
	// Removes 'nParamCount' parameters from the path object starting at 'nParam'
	CUrl8 &DeleteParameter(const size_t nParam, const size_t nParamCount = 1);
	// Removes all parameters from the path object that match the given parameter, returns the number of parameters removed
	size_t RemoveParameter(const CUrlParameter8 &param, const bool bCaseInsensitive = false);
	// Replaces all parameters from the path with the new parameters, returns the number of parameters replaced
	size_t ReplaceParameter(const CUrlParameter8 &paramOld, const CUrlParameter8 &paramNew, const bool bCaseInsensitive = false);
	// Replaces parameter 'nParam' with the give parameter
	CUrl8 &ModifyParameter(const size_t nParam, const CUrlParameter8 &param);

	// Gets/sets the Url fragment
	bool HasFragment(void) const noexcept;
	const CStr8 &GetFragment(void) const noexcept;
	void SetFragment(const CStr8 &strFragment);

	// Returns just the path portion of the Url, optionally including things like parameters and fragments
	CStr8 GetPath(const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;
	// Sets just the path portion of the Url
	ERRCODE SetPath(const CStr8 &strPath);
	// Appends to just the path portion of the Url
	ERRCODE AppendPath(const CStr8 &strPath);

	// Returns just the parameters portion of the Url
	CStr8 GetParameters(const bool bAlwaysIncludeEqual = false) const;
	// Sets just the parameters portion of the Url
	ERRCODE SetParameters(const CStr8 &strParams);
	// Appends to just the parameters portion of the Url
	ERRCODE AppendParameters(const CStr8 &strParams);

	// Splits the given Url into scheme, username, password, domain, port, path, parameters, and fragment
	static bool SplitUrl(PCSTR szUrl, CStr8 &strScheme, CStr8 &strUsername, CStr8 &strPassword, CStr8 &strDomain, CStr8 &strPort, CStr8 &strPath, CStr8 &strParams, CStr8 &strFragment, const bool bAlwaysIncludeEqual = false);

	// Returns whether the string is a valid Url
	static bool IsValidUrl(const CStr8 &strUrl);
	static bool IsValidUrl(PCSTR szUrl);

	void DebugPrint(const bool bAppendNewline = true) const;

private:
	CStr8 m_strScheme;
	CStr8 m_strUsername;
	CStr8 m_strPassword;
	CDomainName8 m_domain;
	uint32_t m_nPort;
	std::vector<CUrlPathSegment8> m_vecPath;
	std::vector<CUrlParameter8> m_vecParams;
	CStr8 m_strFragment;

	ERRCODE ParseUrl(const CStr8 &strUrl);
	ERRCODE ParseDomain(const CStr8 &strDomain);
	ERRCODE ParsePath(const CStr8 &strPath);
	ERRCODE ParseParams(const CStr8 &strParams);
};


//#################################################################################################
class CUrlW final
{
private:
	friend class CUrl8;

public:
	static const uint32_t no_port;

	// Constructs an empty object
	CUrlW(void);
	// Copy and move constructors
	CUrlW(const CUrlW &src);
	CUrlW(CUrlW &&src) noexcept;
	// Constructs an object set to the path/string
	explicit CUrlW(const CUrl8 &url);
	explicit CUrlW(const CDomainNameW &domain);
	explicit CUrlW(const CDomainName8 &domain);
	explicit CUrlW(const CStrW &str);
	explicit CUrlW(const CStr8 &str);
	explicit CUrlW(PCWSTR sz);
	explicit CUrlW(PCSTR sz);

	// Returns a string representing the full path
	operator CStrW(void) const;
	CStrW Get(const bool bIncludeScheme = true, const bool bIncludeUsername = true, const bool bIncludePassword = true, const bool bIncludePort = true, const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false, const bool bIncludeScheme = true, const bool bIncludeUsername = true, const bool bIncludePassword = true, const bool bIncludePort = true, const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false, const bool bIncludeScheme = true, const bool bIncludeUsername = true, const bool bIncludePassword = true, const bool bIncludePort = true, const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;

	// Copy and move assignment operators
	CUrlW &operator=(const CUrlW &src);
	CUrlW &operator=(CUrlW &&src) noexcept;
	// Assigns the object to the path/string
	CUrlW &operator=(const CUrl8 &url);
	CUrlW &operator=(const CDomainNameW &domain);
	CUrlW &operator=(const CDomainName8 &domain);
	CUrlW &operator=(const CStrW &str);
	CUrlW &operator=(const CStr8 &str);
	CUrlW &operator=(PCWSTR sz);
	CUrlW &operator=(PCSTR sz);

	// Assigns the object to the path/string, returns an error code
	ERRCODE Assign(const CUrlW &url);
	ERRCODE Assign(const CUrl8 &url);
	ERRCODE Assign(const CDomainNameW &domain);
	ERRCODE Assign(const CDomainName8 &domain);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(PCSTR sz);

	// Appends the path
	CUrlW &operator+=(const CDomainNameW &domain);
	CUrlW &operator+=(const CDomainName8 &domain);
	CUrlW &operator+=(const CUrlPathSegmentW &path);
	CUrlW &operator+=(const CUrlPathSegment8 &path);
	CUrlW &operator+=(const CUrlParameterW &param);
	CUrlW &operator+=(const CUrlParameter8 &param);

	// Appends the path, returns an error code
	ERRCODE Append(const CDomainNameW &domain);
	ERRCODE Append(const CDomainName8 &domain);
	ERRCODE Append(const CUrlPathSegmentW &path);
	ERRCODE Append(const CUrlPathSegment8 &path);
	ERRCODE Append(const CUrlParameterW &param);
	ERRCODE Append(const CUrlParameter8 &param);

#ifdef _WIN32
	// Returns the concatenation of two paths
	friend CUrlW operator+(const CUrlW &url, const CUrlPathSegmentW &path);
	friend CUrlW operator+(const CUrlW &url, const CUrlPathSegment8 &path);
	friend CUrlW operator+(const CUrl8 &url, const CUrlPathSegmentW &path);
	friend CUrlW operator+(const CUrl8 &url, const CUrlPathSegment8 &path);
	friend CUrlW operator+(const CUrlW &url, const CUrlParameterW &param);
	friend CUrlW operator+(const CUrlW &url, const CUrlParameter8 &param);
	friend CUrlW operator+(const CUrl8 &url, const CUrlParameterW &param);
	friend CUrlW operator+(const CUrl8 &url, const CUrlParameter8 &param);
	friend CUrlW operator+(const CDomainNameW &domain, const CUrlPathSegmentW &path);
	friend CUrlW operator+(const CDomainNameW &domain, const CUrlPathSegment8 &path);
	friend CUrlW operator+(const CDomainName8 &domain, const CUrlPathSegmentW &path);
	friend CUrlW operator+(const CDomainName8 &domain, const CUrlPathSegment8 &path);
	friend CUrlW operator+(const CDomainNameW &domain, const CUrlParameterW &param);
	friend CUrlW operator+(const CDomainNameW &domain, const CUrlParameter8 &param);
	friend CUrlW operator+(const CDomainName8 &domain, const CUrlParameterW &param);
	friend CUrlW operator+(const CDomainName8 &domain, const CUrlParameter8 &param);
#endif

	friend std::wostream &operator<<(std::wostream &stream, const CUrlW &url);

	// Default compare operators are case sensitive since
	// "Downloads" and "downloads" are different paths
	// For case insensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CUrlW &url) const;

	// Tests for not equal
	bool operator!=(const CUrlW &url) const;

	// Tests for less than (case sensitive)
	bool operator<(const CUrlW &url) const;

	// Tests for less than or equal (case sensitive)
	bool operator<=(const CUrlW &url) const;

	// Tests for greater than (case sensitive)
	bool operator>(const CUrlW &url) const;

	// Tests for greater than or equal (case sensitive)
	bool operator>=(const CUrlW &url) const;

	// Compares the object against the path, optionally case insensitive
	bool Compare(const CUrlW &url, const bool bCaseInsensitive = false) const;

	// Empties the path object, freeing the memory
	void Empty(void);
	// Returns whether the path is empty
	bool IsEmpty(void) const noexcept;

	// Gets/sets the Url scheme, for example 'https'
	const CStrW &GetScheme(void) const noexcept;
	void SetScheme(const CStrW &strScheme);

	// Gets/sets the username embedded in the Url
	const CStrW &GetUsername(void) const noexcept;
	void SetUsername(const CStrW &strUsername);

	// Gets/sets the password embedded in the Url
	const CStrW &GetPassword(void) const noexcept;
	void SetPassword(const CStrW &strPassword);

	// Gets/sets the domain, for example 'google.com'
	const CDomainNameW &GetDomain(void) const noexcept;
	void SetDomain(const CDomainNameW &domain);

	// Gets/sets the port number, or -1 for no port
	uint32_t GetPort(void) const noexcept;
	void SetPort(const uint32_t nPort);

	// Returns the number of segments in the path
	size_t GetSegmentCount(void) const noexcept;
	// Returns the segment at a given location, or an empty object if beyond the end of the path
	CUrlPathSegmentW GetSegment(const size_t nSegment) const;
	// Returns the first segment in the path, or an empty object if the path is empty
	CUrlPathSegmentW GetFirstSegment(void) const;
	// Returns the last segment in the path, or an empty object if the path is empty
	CUrlPathSegmentW GetLastSegment(void) const;
	// Removes the leftmost 'nSegmentCount' from the path object
	CUrlW &TrimLeftSegment(const size_t nSegmentCount = 1);
	// Removes the rightmost 'nSegmentCount' from the path object
	CUrlW &TrimRightSegment(const size_t nSegmentCount = 1);
	// Removes everything except the leftmost 'nSegmentCount' from the path object
	CUrlW &CropLeftSegment(const size_t nSegmentCount = 1);
	// Removes everything except the rightmost 'nSegmentCount' from the path object
	CUrlW &CropRightSegment(const size_t nSegmentCount = 1);
	// Inserts the given path into the path object before the given segment, returns an error code
	ERRCODE InsertSegment(const size_t nSegment, const CUrlPathSegmentW &path);
	// Removes 'nSegmentCount' segments from the path object starting at 'nSegment'
	CUrlW &DeleteSegment(const size_t nSegment, const size_t nSegmentCount = 1);
	// Removes all segments from the path object that match the given path segment, returns the number of segments removed
	size_t RemoveSegment(const CUrlPathSegmentW &path, const bool bCaseInsensitive = false);
	// Replaces all segments from the path with the new path segment, returns the number of segments replaced
	size_t ReplaceSegment(const CUrlPathSegmentW &pathOld, const CUrlPathSegmentW &pathNew, const bool bCaseInsensitive = false);
	// Replaces segment 'nSegment' with the give path segment
	CUrlW &ModifySegment(const size_t nSegment, const CUrlPathSegmentW &path);

	// Returns the number of parameters in the path
	size_t GetParameterCount(void) const noexcept;
	// Returns the parameter at a given location, or an empty object if beyond the end of the path
	CUrlParameterW GetParameter(const size_t nParam) const;
	// Returns the first parameter in the path, or an empty object if the path is empty
	CUrlParameterW GetFirstParameter(void) const;
	// Returns the last parameter in the path, or an empty object if the path is empty
	CUrlParameterW GetLastParameter(void) const;
	// Removes the leftmost 'nParamCount' from the path object
	CUrlW &TrimLeftParameter(const size_t nParamCount = 1);
	// Removes the rightmost 'nParamCount' from the path object
	CUrlW &TrimRightParameter(const size_t nParamCount = 1);
	// Removes everything except the leftmost 'nParamCount' from the path object
	CUrlW &CropLeftParameter(const size_t nParamCount = 1);
	// Removes everything except the rightmost 'nParamCount' from the path object
	CUrlW &CropRightParameter(const size_t nParamCount = 1);
	// Inserts the given parameter into the path object before the given parameter, returns an error code
	ERRCODE InsertParameter(const size_t nParam, const CUrlParameterW &param);
	// Removes 'nParamCount' parameters from the path object starting at 'nParam'
	CUrlW &DeleteParameter(const size_t nParam, const size_t nParamCount = 1);
	// Removes all parameters from the path object that match the given parameter, returns the number of parameters removed
	size_t RemoveParameter(const CUrlParameterW &param, const bool bCaseInsensitive = false);
	// Replaces all parameters from the path with the new parameters, returns the number of parameters replaced
	size_t ReplaceParameter(const CUrlParameterW &paramOld, const CUrlParameterW &paramNew, const bool bCaseInsensitive = false);
	// Replaces parameter 'nParam' with the give parameter
	CUrlW &ModifyParameter(const size_t nParam, const CUrlParameterW &param);

	// Gets/sets the Url fragment
	bool HasFragment(void) const noexcept;
	const CStrW &GetFragment(void) const noexcept;
	void SetFragment(const CStrW &strFragment);

	// Returns just the path portion of the Url, optionally including things like parameters and fragments
	CStrW GetPath(const bool bIncludeParams = true, const bool bAlwaysIncludeEqual = false, const bool bIncludeFragment = true) const;
	// Sets just the path portion of the Url
	ERRCODE SetPath(const CStrW &strPath);
	// Appends to just the path portion of the Url
	ERRCODE AppendPath(const CStrW &strPath);

	// Returns just the parameters portion of the Url
	CStrW GetParameters(const bool bAlwaysIncludeEqual = false) const;
	// Sets just the parameters portion of the Url
	ERRCODE SetParameters(const CStrW &strParams);
	// Appends to just the parameters portion of the Url
	ERRCODE AppendParameters(const CStrW &strParams);

	// Splits the given Url into scheme, username, password, domain, port, path, parameters, and fragment
	static bool SplitUrl(PCWSTR szUrl, CStrW &strScheme, CStrW &strUsername, CStrW &strPassword, CStrW &strDomain, CStrW &strPort, CStrW &strPath, CStrW &strParams, CStrW &strFragment, const bool bAlwaysIncludeEqual = false);

	// Returns whether the string is a valid Url
	static bool IsValidUrl(const CStrW &strUrl);
	static bool IsValidUrl(PCWSTR szUrl);

	void DebugPrint(const bool bAppendNewline = true) const;

private:
	CStrW m_strScheme;
	CStrW m_strUsername;
	CStrW m_strPassword;
	CDomainNameW m_domain;
	uint32_t m_nPort;
	std::vector<CUrlPathSegmentW> m_vecPath;
	std::vector<CUrlParameterW> m_vecParams;
	CStrW m_strFragment;

	ERRCODE ParseUrl(const CStrW &strUrl);
	ERRCODE ParseDomain(const CStrW &strDomain);
	ERRCODE ParsePath(const CStrW &strPath);
	ERRCODE ParseParams(const CStrW &strParams);
};


#ifdef _WIN32
using CDomainName = CDomainNameW;
using CUrlPathSegment = CUrlPathSegmentW;
using CUrlParameter = CUrlParameterW;
using CUrl = CUrlW;
#else
using CDomainName = CDomainName8;
using CUrlPathSegment = CUrlPathSegment8;
using CUrlParameter = CUrlParameter8;
using CUrl = CUrl8;
#endif

NS_END

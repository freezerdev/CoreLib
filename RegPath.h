#pragma once

#include <vector>

NS_BEGIN

// Forward declarations
class CRegPathSegment8;
class CRegPathSegmentW;
class CHiveName8;
class CHiveNameW;
class CRegPath8;
class CRegPathW;

//#################################################################################################
class CRegPathSegment8
{
private:
	friend class CRegPathSegmentW;

public:
	// Constructs an empty object
	CRegPathSegment8(void);
	// Copy and move constructors
	CRegPathSegment8(const CRegPathSegment8 &src);
	CRegPathSegment8(CRegPathSegment8 &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CRegPathSegment8(const CRegPathSegmentW &path);
	explicit CRegPathSegment8(const CStr8 &str);
	explicit CRegPathSegment8(const CStrW &str);
	explicit CRegPathSegment8(PCSTR sz);
	explicit CRegPathSegment8(PCWSTR sz);
	explicit CRegPathSegment8(const char ch);
	explicit CRegPathSegment8(const wchar_t ch);

	virtual ~CRegPathSegment8(void);

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
	CRegPathSegment8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CRegPathSegmentW AsWide(void) const;
	CRegPathSegmentW AsNative(void) const;

	// Copy and move assignment operators
	CRegPathSegment8 &operator=(const CRegPathSegment8 &src);
	CRegPathSegment8 &operator=(CRegPathSegment8 &&src) noexcept;
	// Assigns the object to the string/character
	CRegPathSegment8 &operator=(const CRegPathSegmentW &path);
	virtual CRegPathSegment8 &operator=(const CStr8 &str);
	virtual CRegPathSegment8 &operator=(const CStrW &str);
	virtual CRegPathSegment8 &operator=(PCSTR sz);
	virtual CRegPathSegment8 &operator=(PCWSTR sz);
	CRegPathSegment8 &operator=(const char ch);
	CRegPathSegment8 &operator=(const wchar_t ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CRegPathSegment8 &path);
	ERRCODE Assign(const CRegPathSegmentW &path);
	virtual ERRCODE Assign(const CStr8 &str);
	virtual ERRCODE Assign(const CStrW &str);
	virtual ERRCODE Assign(PCSTR sz);
	virtual ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(const char ch);
	ERRCODE Assign(const wchar_t ch);

	friend std::ostream &operator<<(std::ostream &stream, const CRegPathSegment8 &path);

	// Default compare operators are case insensitive
	// After all "SOFTWARE" and "software" are the same path
	// For case sensitive compares use Compare()

	// Tests for equal
	bool operator==(const CRegPathSegment8 &path) const;
	virtual bool operator==(const CStr8 &str) const;
	virtual bool operator==(PCSTR sz) const;
	bool operator==(const char ch) const;

	// Tests for not equal
	bool operator!=(const CRegPathSegment8 &path) const;
	virtual bool operator!=(const CStr8 &str) const;
	virtual bool operator!=(PCSTR sz) const;
	bool operator!=(const char ch) const;

	// Tests for less than
	bool operator<(const CRegPathSegment8 &path) const;
	virtual bool operator<(const CStr8 &str) const;
	virtual bool operator<(PCSTR sz) const;
	bool operator<(const char ch) const;

	// Tests for less than or equal
	bool operator<=(const CRegPathSegment8 &path) const;
	virtual bool operator<=(const CStr8 &str) const;
	virtual bool operator<=(PCSTR sz) const;
	bool operator<=(const char ch) const;

	// Tests for greater than
	bool operator>(const CRegPathSegment8 &path) const;
	virtual bool operator>(const CStr8 &str) const;
	virtual bool operator>(PCSTR sz) const;
	bool operator>(const char ch) const;

	// Tests for greater than or equal
	bool operator>=(const CRegPathSegment8 &path) const;
	virtual bool operator>=(const CStr8 &str) const;
	virtual bool operator>=(PCSTR sz) const;
	bool operator>=(const char ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CRegPathSegment8 &path, const bool bCaseInsensitive = true) const;
	int Compare(const CStr8 &str, const bool bCaseInsensitive = true) const;
	int Compare(PCSTR sz, const bool bCaseInsensitive = true) const;
	int Compare(const char ch, const bool bCaseInsensitive = true) const;

	// Returns whether the string/character is a valid path segment
	static bool IsValidSegment(const CStr8 &str);
	static bool IsValidSegment(PCSTR szSegment);
	static bool IsValidSegment(const char chSegment);

protected:
	PSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);
};


//#################################################################################################
class CRegPathSegmentW
{
private:
	friend class CRegPathSegment8;

public:
	// Constructs an empty object
	CRegPathSegmentW(void);
	// Copy and move constructors
	CRegPathSegmentW(const CRegPathSegmentW &src);
	CRegPathSegmentW(CRegPathSegmentW &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CRegPathSegmentW(const CRegPathSegment8 &path);
	explicit CRegPathSegmentW(const CStrW &str);
	explicit CRegPathSegmentW(const CStr8 &str);
	explicit CRegPathSegmentW(PCWSTR sz);
	explicit CRegPathSegmentW(PCSTR sz);
	explicit CRegPathSegmentW(const wchar_t ch);
	explicit CRegPathSegmentW(const char ch);

	virtual ~CRegPathSegmentW(void);

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
	CRegPathSegment8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CRegPathSegmentW AsWide(void) const;
	CRegPathSegmentW AsNative(void) const;

	// Copy and move assignment operators
	CRegPathSegmentW &operator=(const CRegPathSegmentW &src);
	CRegPathSegmentW &operator=(CRegPathSegmentW &&src) noexcept;
	// Assigns the object to the string/character
	CRegPathSegmentW &operator=(const CRegPathSegment8 &path);
	virtual CRegPathSegmentW &operator=(const CStrW &str);
	virtual CRegPathSegmentW &operator=(const CStr8 &str);
	virtual CRegPathSegmentW &operator=(PCWSTR sz);
	virtual CRegPathSegmentW &operator=(PCSTR sz);
	CRegPathSegmentW &operator=(const wchar_t ch);
	CRegPathSegmentW &operator=(const char ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CRegPathSegmentW &path);
	ERRCODE Assign(const CRegPathSegment8 &path);
	virtual ERRCODE Assign(const CStrW &str);
	virtual ERRCODE Assign(const CStr8 &str);
	virtual ERRCODE Assign(PCWSTR sz);
	virtual ERRCODE Assign(PCSTR sz);
	virtual ERRCODE Assign(const wchar_t ch);
	virtual ERRCODE Assign(const char ch);

	friend std::wostream &operator<<(std::wostream &stream, const CRegPathSegmentW &path);

	// Default compare operators are case insensitive
	// After all "SOFTWARE" and "software" are the same path
	// For case sensitive compares use Compare()

	// Tests for equal
	bool operator==(const CRegPathSegmentW &path) const;
	virtual bool operator==(const CStrW &str) const;
	virtual bool operator==(PCWSTR sz) const;
	bool operator==(const wchar_t ch) const;

	// Tests for not equal
	bool operator!=(const CRegPathSegmentW &path) const;
	virtual bool operator!=(const CStrW &str) const;
	virtual bool operator!=(PCWSTR sz) const;
	bool operator!=(const wchar_t ch) const;

	// Tests for less than
	bool operator<(const CRegPathSegmentW &path) const;
	virtual bool operator<(const CStrW &str) const;
	virtual bool operator<(PCWSTR sz) const;
	bool operator<(const wchar_t ch) const;

	// Tests for less than or equal
	bool operator<=(const CRegPathSegmentW &path) const;
	virtual bool operator<=(const CStrW &str) const;
	virtual bool operator<=(PCWSTR sz) const;
	bool operator<=(const wchar_t ch) const;

	// Tests for greater than
	bool operator>(const CRegPathSegmentW &path) const;
	virtual bool operator>(const CStrW &str) const;
	virtual bool operator>(PCWSTR sz) const;
	bool operator>(const wchar_t ch) const;

	// Tests for greater than or equal
	bool operator>=(const CRegPathSegmentW &path) const;
	virtual bool operator>=(const CStrW &str) const;
	virtual bool operator>=(PCWSTR sz) const;
	bool operator>=(const wchar_t ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CRegPathSegmentW &path, const bool bCaseInsensitive = true) const;
	int Compare(const CStrW &str, const bool bCaseInsensitive = true) const;
	int Compare(PCWSTR sz, const bool bCaseInsensitive = true) const;
	int Compare(const wchar_t ch, const bool bCaseInsensitive = true) const;

	// Returns whether the string/character is a valid path segment
	static bool IsValidSegment(const CStrW &str);
	static bool IsValidSegment(PCWSTR szSegment);
	static bool IsValidSegment(const wchar_t chSegment);

protected:
	PWSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);
};


//#################################################################################################
class CHiveName8 final : public CRegPathSegment8
{
private:
	friend class CHiveNameW;

public:
	// Constructs an empty object
	CHiveName8(void) = default;
	// Copy and move constructors
	CHiveName8(const CHiveName8 &src) = default;
	CHiveName8(CHiveName8 &&src) = default;
	// Constructs an object set to the string/character
	explicit CHiveName8(const CHiveNameW &hive);
	explicit CHiveName8(const CStr8 &str);
	explicit CHiveName8(const CStrW &str);
	explicit CHiveName8(PCSTR sz);
	explicit CHiveName8(PCWSTR sz);
	explicit CHiveName8(const HKEY hHive, const bool bShort = false);

	virtual ~CHiveName8(void) override = default;

	// Returns the hive handle
	operator HKEY(void) const;
	HKEY GetHive(void) const;
	// Returns the hive long name, for example HKEY_LOCAL_MACHINE
	PCSTR GetLongName(void) const;
	// Returns the hive long name, for example HKLM
	PCSTR GetShortName(void) const;

	// Assigns the object to the string/character
	CHiveName8 &operator=(const CHiveName8 &hive) = default;
	CHiveName8 &operator=(CHiveName8 &&src) = default;
	CHiveName8 &operator=(const CHiveNameW &hive);
	virtual CHiveName8 &operator=(const CStr8 &str) override;
	virtual CHiveName8 &operator=(const CStrW &str) override;
	virtual CHiveName8 &operator=(PCSTR sz) override;
	virtual CHiveName8 &operator=(PCWSTR sz) override;
	CHiveName8 &operator=(const HKEY hHive);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CHiveName8 &hive);
	ERRCODE Assign(const CHiveNameW &hive);
	virtual ERRCODE Assign(const CStr8 &str) override;
	virtual ERRCODE Assign(const CStrW &str) override;
	virtual ERRCODE Assign(PCSTR sz) override;
	virtual ERRCODE Assign(PCWSTR sz) override;
	ERRCODE Assign(const HKEY hHive, const bool bShort = false);

	// Default compare operators are case insensitive
	// After all "HKLM" and "hklm" are the same hive

	// Tests for equal
	bool operator==(const CHiveName8 &hive) const;
	virtual bool operator==(const CStr8 &str) const override;
	virtual bool operator==(PCSTR sz) const override;
	bool operator==(const HKEY hHive) const;

	// Tests for not equal
	bool operator!=(const CHiveName8 &hive) const;
	virtual bool operator!=(const CStr8 &str) const override;
	virtual bool operator!=(PCSTR sz) const override;
	bool operator!=(const HKEY hHive) const;

	// Tests for less than
	bool operator<(const CHiveName8 &hive) const;
	virtual bool operator<(const CStr8 &str) const override;
	virtual bool operator<(PCSTR sz) const override;
	bool operator<(const HKEY hHive) const;

	// Tests for less than or equal
	bool operator<=(const CHiveName8 &hive) const;
	virtual bool operator<=(const CStr8 &str) const override;
	virtual bool operator<=(PCSTR sz) const override;
	bool operator<=(const HKEY hHive) const;

	// Tests for greater than
	bool operator>(const CHiveName8 &hive) const;
	virtual bool operator>(const CStr8 &str) const override;
	virtual bool operator>(PCSTR sz) const override;
	bool operator>(const HKEY hHive) const;

	// Tests for greater than or equal
	bool operator>=(const CHiveName8 &hive) const;
	virtual bool operator>=(const CStr8 &str) const override;
	virtual bool operator>=(PCSTR sz) const override;
	bool operator>=(const HKEY hHive) const;

	// Compares the object against the string/character
	int Compare(const CHiveName8 &hive) const;
	int Compare(const CStr8 &str) const;
	int Compare(PCSTR sz) const;
	int Compare(const HKEY hHive) const;

	static PCSTR GetLongName(const CStr8 &str);
	static PCSTR GetLongName(const CStrW &str);
	static PCSTR GetLongName(PCSTR sz);
	static PCSTR GetLongName(PCWSTR sz);
	static PCSTR GetLongName(const HKEY hHive);
	static PCSTR GetShortName(const CStr8 &str);
	static PCSTR GetShortName(const CStrW &str);
	static PCSTR GetShortName(PCSTR sz);
	static PCSTR GetShortName(PCWSTR sz);
	static PCSTR GetShortName(const HKEY hHive);

	static HKEY GetHive(const CStr8 &str);
	static HKEY GetHive(PCSTR sz);

	static bool IsValidHiveName(const CStr8 &str);
	static bool IsValidHiveName(PCSTR sz);
};


//#################################################################################################
class CHiveNameW final : public CRegPathSegmentW
{
private:
	friend class CHiveName8;

public:
	// Constructs an empty object
	CHiveNameW(void) = default;
	// Copy and move constructors
	CHiveNameW(const CHiveNameW &src) = default;
	CHiveNameW(CHiveNameW &&src) = default;
	// Constructs an object set to the string/character
	explicit CHiveNameW(const CHiveName8 &hive);
	explicit CHiveNameW(const CStrW &str);
	explicit CHiveNameW(const CStr8 &str);
	explicit CHiveNameW(PCWSTR sz);
	explicit CHiveNameW(PCSTR sz);
	explicit CHiveNameW(const HKEY hHive, const bool bShort = false);

	virtual ~CHiveNameW(void) override = default;

	// Returns the hive handle
	operator HKEY(void) const;
	HKEY GetHive(void) const;
	// Returns the hive long name, for example HKEY_LOCAL_MACHINE
	PCWSTR GetLongName(void) const;
	// Returns the hive long name, for example HKLM
	PCWSTR GetShortName(void) const;

	// Assigns the object to the string/character
	CHiveNameW &operator=(const CHiveNameW &hive) = default;
	CHiveNameW &operator=(CHiveNameW &&src) = default;
	CHiveNameW &operator=(const CHiveName8 &hive);
	virtual CHiveNameW &operator=(const CStrW &str) override;
	virtual CHiveNameW &operator=(const CStr8 &str) override;
	virtual CHiveNameW &operator=(PCWSTR sz) override;
	virtual CHiveNameW &operator=(PCSTR sz) override;
	CHiveNameW &operator=(const HKEY hHive);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CHiveNameW &hive);
	ERRCODE Assign(const CHiveName8 &hive);
	virtual ERRCODE Assign(const CStrW &str) override;
	virtual ERRCODE Assign(const CStr8 &str) override;
	virtual ERRCODE Assign(PCWSTR sz) override;
	virtual ERRCODE Assign(PCSTR sz) override;
	ERRCODE Assign(const HKEY hHive, const bool bShort = false);

	// Default compare operators are case insensitive
	// After all "HKLM" and "hklm" are the same hive

	// Tests for equal
	bool operator==(const CHiveNameW &hive) const;
	virtual bool operator==(const CStrW &str) const override;
	virtual bool operator==(PCWSTR sz) const override;
	bool operator==(const HKEY hHive) const;

	// Tests for not equal
	bool operator!=(const CHiveNameW &hive) const;
	virtual bool operator!=(const CStrW &str) const override;
	virtual bool operator!=(PCWSTR sz) const override;
	bool operator!=(const HKEY hHive) const;

	// Tests for less than
	bool operator<(const CHiveNameW &hive) const;
	virtual bool operator<(const CStrW &str) const override;
	virtual bool operator<(PCWSTR sz) const override;
	bool operator<(const HKEY hHive) const;

	// Tests for less than or equal
	bool operator<=(const CHiveNameW &hive) const;
	virtual bool operator<=(const CStrW &str) const override;
	virtual bool operator<=(PCWSTR sz) const override;
	bool operator<=(const HKEY hHive) const;

	// Tests for greater than
	bool operator>(const CHiveNameW &hive) const;
	virtual bool operator>(const CStrW &str) const override;
	virtual bool operator>(PCWSTR sz) const override;
	bool operator>(const HKEY hHive) const;

	// Tests for greater than or equal
	bool operator>=(const CHiveNameW &hive) const;
	virtual bool operator>=(const CStrW &str) const override;
	virtual bool operator>=(PCWSTR sz) const override;
	bool operator>=(const HKEY hHive) const;

	// Compares the object against the string/character
	int Compare(const CHiveNameW &hive) const;
	int Compare(const CStrW &str) const;
	int Compare(PCWSTR sz) const;
	int Compare(const HKEY hHive) const;

	static PCWSTR GetLongName(const CStrW &str);
	static PCWSTR GetLongName(const CStr8 &str);
	static PCWSTR GetLongName(PCWSTR sz);
	static PCWSTR GetLongName(PCSTR sz);
	static PCWSTR GetLongName(const HKEY hHive);
	static PCWSTR GetShortName(const CStrW &str);
	static PCWSTR GetShortName(const CStr8 &str);
	static PCWSTR GetShortName(PCWSTR sz);
	static PCWSTR GetShortName(PCSTR sz);
	static PCWSTR GetShortName(const HKEY hHive);

	static HKEY GetHive(const CStrW &str);
	static HKEY GetHive(PCWSTR sz);

	static bool IsValidHiveName(const CStrW &str);
	static bool IsValidHiveName(PCWSTR sz);
};


//#################################################################################################
class CRegPath8 final
{
private:
	friend class CRegPathW;

public:
	// Constructs an empty object
	CRegPath8(void);
	// Copy and move constructors
	CRegPath8(const CRegPath8 &src);
	CRegPath8(CRegPath8 &&src) noexcept;
	// Constructs an object set to the path/string/character
	explicit CRegPath8(const CRegPathW &path);
	explicit CRegPath8(const CRegPathSegment8 &path);
	explicit CRegPath8(const CRegPathSegmentW &path);
	explicit CRegPath8(const CHiveName8 &hive);
	explicit CRegPath8(const CHiveNameW &hive);
	explicit CRegPath8(const CStr8 &str);
	explicit CRegPath8(const CStrW &str);
	explicit CRegPath8(PCSTR sz);
	explicit CRegPath8(PCWSTR sz);
	explicit CRegPath8(const char ch);
	explicit CRegPath8(const wchar_t ch);

	// Returns a string representing the full path
	operator CStr8(void) const;
	CStr8 Get(void) const;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const;

	// Copy and move assignment operators
	CRegPath8 &operator=(const CRegPath8 &src);
	CRegPath8 &operator=(CRegPath8 &&src) noexcept;
	// Assigns the object to the path/string/character
	CRegPath8 &operator=(const CRegPathW &path);
	CRegPath8 &operator=(const CRegPathSegment8 &path);
	CRegPath8 &operator=(const CRegPathSegmentW &path);
	CRegPath8 &operator=(const CHiveName8 &hive);
	CRegPath8 &operator=(const CHiveNameW &hive);
	CRegPath8 &operator=(const CStr8 &str);
	CRegPath8 &operator=(const CStrW &str);
	CRegPath8 &operator=(PCSTR sz);
	CRegPath8 &operator=(PCWSTR sz);
	CRegPath8 &operator=(const char ch);
	CRegPath8 &operator=(const wchar_t ch);

	// Assigns the object to the path/string/character, returns an error code
	ERRCODE Assign(const CRegPath8 &path);
	ERRCODE Assign(const CRegPathW &path);
	ERRCODE Assign(const CRegPathSegment8 &path);
	ERRCODE Assign(const CRegPathSegmentW &path);
	ERRCODE Assign(const CHiveName8 &hive);
	ERRCODE Assign(const CHiveNameW &hive);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(PCSTR sz);
	ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(const char ch);
	ERRCODE Assign(const wchar_t ch);

	// Appends the path
	CRegPath8 &operator+=(const CRegPath8 &path);
	CRegPath8 &operator+=(const CRegPathW &path);
	CRegPath8 &operator+=(const CRegPathSegment8 &path);
	CRegPath8 &operator+=(const CRegPathSegmentW &path);
	CRegPath8 &operator+=(const CHiveName8 &hive);
	CRegPath8 &operator+=(const CHiveNameW &hive);

	// Prepends the path, returns an error code
	ERRCODE Prepend(const CRegPath8 &path);
	ERRCODE Prepend(const CRegPathW &path);
	ERRCODE Prepend(const CRegPathSegment8 &path);
	ERRCODE Prepend(const CRegPathSegmentW &path);
	ERRCODE Prepend(const CHiveName8 &hive);
	ERRCODE Prepend(const CHiveNameW &hive);

	// Appends the path, returns an error code
	ERRCODE Append(const CRegPath8 &path);
	ERRCODE Append(const CRegPathW &path);
	ERRCODE Append(const CRegPathSegment8 &path);
	ERRCODE Append(const CRegPathSegmentW &path);
	ERRCODE Append(const CHiveName8 &hive);
	ERRCODE Append(const CHiveNameW &hive);

	friend std::ostream &operator<<(std::ostream &stream, const CRegPath8 &path);

	// Default compare operators are case insensitive
	// After all "HKLM\SOFTWARE" and "hklm\software" are the same path
	// For case sensitive compares use Compare()

	// Tests for equal
	bool operator==(const CRegPath8 &path) const;
	bool operator==(const CRegPathSegment8 &path) const;
	bool operator==(const CHiveName8 &hive) const;

	// Tests for not equal
	bool operator!=(const CRegPath8 &path) const;
	bool operator!=(const CRegPathSegment8 &path) const;
	bool operator!=(const CHiveName8 &hive) const;

	// Tests for less than (case sensitive)
	bool operator<(const CRegPath8 &path) const;

	// Tests for less than or equal (case sensitive)
	bool operator<=(const CRegPath8 &path) const;

	// Tests for greater than (case sensitive)
	bool operator>(const CRegPath8 &path) const;

	// Tests for greater than or equal (case sensitive)
	bool operator>=(const CRegPath8 &path) const;

	// Compares the object against the path, optionally case insensitive
	bool Compare(const CRegPath8 &path, const bool bCaseInsensitive = true) const;
	bool Compare(const CRegPathSegment8 &path, const bool bCaseInsensitive = true) const;
	bool Compare(const CHiveName8 &hive) const;

	// Compares the leftmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareLeft(const CRegPath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive = true) const;
	// Compares the rightmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareRight(const CRegPath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive = true) const;

	// Tests if the path object contains given path
	bool Contains(const CRegPath8 &path, const bool bCaseInsensitive = true) const;
	bool Contains(const CRegPathSegment8 &path, const bool bCaseInsensitive = true) const;
	bool Contains(const CHiveName8 &hive) const;

	// Empties the path object, freeing the memory
	void Empty(void);

	// Returns whether the path is empty
	bool IsEmpty(void) const noexcept;
	// Returns whether the path is a registry hive
	bool IsRoot(void) const noexcept;
	// Returns whether the path is a fragment not containing a hive
	bool IsRelative(void) const noexcept;

	// Returns the number of segments in the path
	size_t GetSegmentCount(void) const noexcept;
	// Returns the segment at a given location, or an empty object if beyond the end of the path
	CRegPathSegment8 GetSegment(const size_t nSegment) const;
	// Returns the first segment in the path, or an empty object if the path is empty
	CRegPathSegment8 GetFirstSegment(void) const;
	// Returns the last segment in the path, or an empty object if the path is empty
	CRegPathSegment8 GetLastSegment(void) const;
	// Returns the leftmost 'nSegmentCount' segments from the path object
	CRegPath8 GetLeft(const size_t nSegmentCount) const;
	// Returns the rightmost 'nSegmentCount' segments from the path object
	CRegPath8 GetRight(const size_t nSegmentCount) const;
	// Returns the middle 'nSegmentCount' segments from the path object starting at 'nSegment'
	CRegPath8 GetMid(const size_t nSegment, const size_t nSegmentCount) const;
	// Removes the leftmost 'nSegmentCount' from the path object
	CRegPath8 &TrimLeft(const size_t nSegmentCount = 1);
	// Removes the rightmost 'nSegmentCount' from the path object
	CRegPath8 &TrimRight(const size_t nSegmentCount = 1);
	// Removes everything except the leftmost 'nSegmentCount' from the path object
	CRegPath8 &CropLeft(const size_t nSegmentCount = 1);
	// Removes everything except the rightmost 'nSegmentCount' from the path object
	CRegPath8 &CropRight(const size_t nSegmentCount = 1);
	// Inserts the given path into the path object before the given segment, returns an error code
	ERRCODE Insert(const size_t nSegment, const CRegPath8 &path);
	ERRCODE Insert(const size_t nSegment, const CRegPathSegment8 &path);
	ERRCODE Insert(const CHiveName8 &hive);
	// Removes 'nSegmentCount' segments from the path object starting at 'nSegment'
	CRegPath8 &Delete(const size_t nSegment, const size_t nSegmentCount = 1);
	// Removes all segments from the path object that match the given path segment, returns the number of segments removed
	size_t Remove(const CRegPathSegment8 &path, const bool bCaseInsensitive = true);
	// Replaces all segments from the path with the new path segment, returns the number of segments replaced
	size_t Replace(const CRegPathSegment8 &pathOld, const CRegPathSegment8 &pathNew, const bool bCaseInsensitive = true);
	// Replaces segment 'nSegment' with the give path segment
	CRegPath8 &Modify(const size_t nSegment, const CRegPathSegment8 &path);

	// Returns the full path using the long hive representation, for example HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft
	CStr8 GetLongPath(void) const;
	// Returns the full path using the short hive representation, for example HKLM\SOFTWARE\Microsoft
	CStr8 GetShortPath(void) const;
	// Returns the registry hive, or an empty object if the path does not contain a hive
	CHiveName8 GetHive(void) const;
	// Returns the full path excluding the hive
	CRegPath8 GetRelativePath(void) const;

	// Splits the given path into hive and path
	static bool SplitPath(PCSTR szPath, CStr8 &strHive, CStr8 &strPath);

	// Returns whether the string/character is a valid path
	static bool IsValidPath(const CStr8 &strPath);
	static bool IsValidPath(PCSTR szPath);
	static bool IsValidPath(const char chPath);

	void DebugPrint(const bool bAppendNewline = true) const;

private:
	std::vector<std::unique_ptr<CRegPathSegment8>> m_vecSegments;
	bool m_bRooted;

	ERRCODE ParsePath(const CStr8 &strPath);
};


//#################################################################################################
class CRegPathW final
{
private:
	friend class CRegPath8;

public:
	// Constructs an empty object
	CRegPathW(void);
	// Copy and move constructors
	CRegPathW(const CRegPathW &src);
	CRegPathW(CRegPathW &&src) noexcept;
	// Constructs an object set to the path/string/character
	explicit CRegPathW(const CRegPath8 &path);
	explicit CRegPathW(const CRegPathSegmentW &path);
	explicit CRegPathW(const CRegPathSegment8 &path);
	explicit CRegPathW(const CHiveNameW &hive);
	explicit CRegPathW(const CHiveName8 &hive);
	explicit CRegPathW(const CStrW &str);
	explicit CRegPathW(const CStr8 &str);
	explicit CRegPathW(PCWSTR sz);
	explicit CRegPathW(PCSTR sz);
	explicit CRegPathW(const wchar_t ch);
	explicit CRegPathW(const char ch);

	// Returns a string representing the full path
	operator CStrW(void) const;
	CStrW Get(void) const;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const;

	// Copy and move assignment operators
	CRegPathW &operator=(const CRegPathW &src);
	CRegPathW &operator=(CRegPathW &&src) noexcept;
	// Assigns the object to the path/string/character
	CRegPathW &operator=(const CRegPath8 &path);
	CRegPathW &operator=(const CRegPathSegmentW &path);
	CRegPathW &operator=(const CRegPathSegment8 &path);
	CRegPathW &operator=(const CHiveNameW &hive);
	CRegPathW &operator=(const CHiveName8 &hive);
	CRegPathW &operator=(const CStrW &str);
	CRegPathW &operator=(const CStr8 &str);
	CRegPathW &operator=(PCWSTR sz);
	CRegPathW &operator=(PCSTR sz);
	CRegPathW &operator=(const wchar_t ch);
	CRegPathW &operator=(const char ch);

	// Assigns the object to the path/string/character, returns an error code
	ERRCODE Assign(const CRegPathW &path);
	ERRCODE Assign(const CRegPath8 &path);
	ERRCODE Assign(const CRegPathSegmentW &path);
	ERRCODE Assign(const CRegPathSegment8 &path);
	ERRCODE Assign(const CHiveNameW &hive);
	ERRCODE Assign(const CHiveName8 &hive);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(PCSTR sz);
	ERRCODE Assign(const wchar_t ch);
	ERRCODE Assign(const char ch);

	// Appends the path
	CRegPathW &operator+=(const CRegPathW &path);
	CRegPathW &operator+=(const CRegPath8 &path);
	CRegPathW &operator+=(const CRegPathSegmentW &path);
	CRegPathW &operator+=(const CRegPathSegment8 &path);
	CRegPathW &operator+=(const CHiveNameW &hive);
	CRegPathW &operator+=(const CHiveName8 &hive);

	// Prepends the path, returns an error code
	ERRCODE Prepend(const CRegPathW &path);
	ERRCODE Prepend(const CRegPath8 &path);
	ERRCODE Prepend(const CRegPathSegmentW &path);
	ERRCODE Prepend(const CRegPathSegment8 &path);
	ERRCODE Prepend(const CHiveNameW &hive);
	ERRCODE Prepend(const CHiveName8 &hive);

	// Appends the path, returns an error code
	ERRCODE Append(const CRegPathW &path);
	ERRCODE Append(const CRegPath8 &path);
	ERRCODE Append(const CRegPathSegmentW &path);
	ERRCODE Append(const CRegPathSegment8 &path);
	ERRCODE Append(const CHiveNameW &hive);
	ERRCODE Append(const CHiveName8 &hive);

	// Returns the concatenation of two paths
	friend CRegPathW operator+(const CRegPathW &path1, const CRegPathW &path2);
	friend CRegPathW operator+(const CRegPathW &path1, const CRegPath8 &path2);
	friend CRegPathW operator+(const CRegPath8 &path1, const CRegPathW &path2);
	friend CRegPathW operator+(const CRegPath8 &path1, const CRegPath8 &path2);
	friend CRegPathW operator+(const CRegPathW &path1, const CRegPathSegmentW &path2);
	friend CRegPathW operator+(const CRegPathW &path1, const CRegPathSegment8 &path2);
	friend CRegPathW operator+(const CRegPath8 &path1, const CRegPathSegmentW &path2);
	friend CRegPathW operator+(const CRegPath8 &path1, const CRegPathSegment8 &path2);
	friend CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPathW &path2);
	friend CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPath8 &path2);
	friend CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPathW &path2);
	friend CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPath8 &path2);
	friend CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPathSegmentW &path2);
	friend CRegPathW operator+(const CRegPathSegmentW &path1, const CRegPathSegment8 &path2);
	friend CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPathSegmentW &path2);
	friend CRegPathW operator+(const CRegPathSegment8 &path1, const CRegPathSegment8 &path2);
	friend CRegPathW operator+(const CHiveNameW &hive, const CRegPathW &path);
	friend CRegPathW operator+(const CHiveNameW &hive, const CRegPath8 &path);
	friend CRegPathW operator+(const CHiveName8 &hive, const CRegPathW &path);
	friend CRegPathW operator+(const CHiveName8 &hive, const CRegPath8 &path);
	friend CRegPathW operator+(const CHiveNameW &hive, const CRegPathSegmentW &path);
	friend CRegPathW operator+(const CHiveNameW &hive, const CRegPathSegment8 &path);
	friend CRegPathW operator+(const CHiveName8 &hive, const CRegPathSegmentW &path);
	friend CRegPathW operator+(const CHiveName8 &hive, const CRegPathSegment8 &path);

	friend std::wostream &operator<<(std::wostream &stream, const CRegPathW &path);

	// Default compare operators are case insensitive
	// After all "HKLM\SOFTWARE" and "hklm\software" are the same path
	// For case sensitive compares use Compare()

	// Tests for equal
	bool operator==(const CRegPathW &path) const;
	bool operator==(const CRegPathSegmentW &path) const;
	bool operator==(const CHiveNameW &hive) const;

	// Tests for not equal
	bool operator!=(const CRegPathW &path) const;
	bool operator!=(const CRegPathSegmentW &path) const;
	bool operator!=(const CHiveNameW &hive) const;

	// Tests for less than (case sensitive)
	bool operator<(const CRegPathW &path) const;

	// Tests for less than or equal (case sensitive)
	bool operator<=(const CRegPathW &path) const;

	// Tests for greater than (case sensitive)
	bool operator>(const CRegPathW &path) const;

	// Tests for greater than or equal (case sensitive)
	bool operator>=(const CRegPathW &path) const;

	// Compares the object against the path, optionally case insensitive
	bool Compare(const CRegPathW &path, const bool bCaseInsensitive = true) const;
	bool Compare(const CRegPathSegmentW &path, const bool bCaseInsensitive = true) const;
	bool Compare(const CHiveNameW &hive) const;

	// Compares the leftmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareLeft(const CRegPathW &path, const size_t nSegmentCount, const bool bCaseInsensitive = true) const;
	// Compares the rightmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareRight(const CRegPathW &path, const size_t nSegmentCount, const bool bCaseInsensitive = true) const;

	// Tests if the path object contains given path
	bool Contains(const CRegPathW &path, const bool bCaseInsensitive = true) const;
	bool Contains(const CRegPathSegmentW &path, const bool bCaseInsensitive = true) const;
	bool Contains(const CHiveNameW &hive) const;

	// Empties the path object, freeing the memory
	void Empty(void);

	// Returns whether the path is empty
	bool IsEmpty(void) const noexcept;
	// Returns whether the path is a registry hive
	bool IsRoot(void) const noexcept;
	// Returns whether the path is a fragment not containing a hive
	bool IsRelative(void) const noexcept;

	// Returns the number of segments in the path
	size_t GetSegmentCount(void) const noexcept;
	// Returns the segment at a given location, or an empty object if beyond the end of the path
	CRegPathSegmentW GetSegment(const size_t nSegment) const;
	// Returns the first segment in the path, or an empty object if the path is empty
	CRegPathSegmentW GetFirstSegment(void) const;
	// Returns the last segment in the path, or an empty object if the path is empty
	CRegPathSegmentW GetLastSegment(void) const;
	// Returns the leftmost 'nSegmentCount' segments from the path object
	CRegPathW GetLeft(const size_t nSegmentCount) const;
	// Returns the rightmost 'nSegmentCount' segments from the path object
	CRegPathW GetRight(const size_t nSegmentCount) const;
	// Returns the middle 'nSegmentCount' segments from the path object starting at 'nSegment'
	CRegPathW GetMid(const size_t nSegment, const size_t nSegmentCount) const;
	// Removes the leftmost 'nSegmentCount' from the path object
	CRegPathW &TrimLeft(const size_t nSegmentCount = 1);
	// Removes the rightmost 'nSegmentCount' from the path object
	CRegPathW &TrimRight(const size_t nSegmentCount = 1);
	// Removes everything except the leftmost 'nSegmentCount' from the path object
	CRegPathW &CropLeft(const size_t nSegmentCount = 1);
	// Removes everything except the rightmost 'nSegmentCount' from the path object
	CRegPathW &CropRight(const size_t nSegmentCount = 1);
	// Inserts the given path into the path object before the given segment, returns an error code
	ERRCODE Insert(const size_t nSegment, const CRegPathW &path);
	ERRCODE Insert(const size_t nSegment, const CRegPathSegmentW &path);
	ERRCODE Insert(const CHiveNameW &hive);
	// Removes 'nSegmentCount' segments from the path object starting at 'nSegment'
	CRegPathW &Delete(const size_t nSegment, const size_t nSegmentCount = 1);
	// Removes all segments from the path object that match the given path segment, returns the number of segments removed
	size_t Remove(const CRegPathSegmentW &path, const bool bCaseInsensitive = true);
	// Replaces all segments from the path with the new path segment, returns the number of segments replaced
	size_t Replace(const CRegPathSegmentW &pathOld, const CRegPathSegmentW &pathNew, const bool bCaseInsensitive = true);
	// Replaces segment 'nSegment' with the give path segment
	CRegPathW &Modify(const size_t nSegment, const CRegPathSegmentW &path);

	// Returns the full path using the long hive representation, for example HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft
	CStrW GetLongPath(void) const;
	// Returns the full path using the short hive representation, for example HKLM\SOFTWARE\Microsoft
	CStrW GetShortPath(void) const;
	// Returns the registry hive, or an empty object if the path does not contain a hive
	CHiveNameW GetHive(void) const;
	// Returns the full path excluding the hive
	CRegPathW GetRelativePath(void) const;

	// Splits the given path into hive and path
	static bool SplitPath(PCWSTR szPath, CStrW &strHive, CStrW &strPath);

	// Returns whether the string/character is a valid path
	static bool IsValidPath(const CStrW &strPath);
	static bool IsValidPath(PCWSTR szPath);
	static bool IsValidPath(const wchar_t chPath);

	void DebugPrint(const bool bAppendNewline = true) const;

private:
	std::vector<std::unique_ptr<CRegPathSegmentW>> m_vecSegments;
	bool m_bRooted;

	ERRCODE ParsePath(const CStrW &strPath);
};


using CRegPathSegment = CRegPathSegmentW;
using CHiveName = CHiveNameW;
using CRegPath = CRegPathW;

NS_END

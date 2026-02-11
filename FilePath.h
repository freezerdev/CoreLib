#pragma once

#include <vector>

NS_BEGIN

// Forward declarations
class CFilePathSegment8;
class CFilePathSegmentW;
class CMachineName8;
class CMachineNameW;
class CFilePath8;
class CFilePathW;

#ifdef _WIN32
static constexpr auto FILE_DEFAULT_CMP = true;
#else
static constexpr auto FILE_DEFAULT_CMP = false;
#endif

//#################################################################################################
class CFilePathSegment8
{
private:
	friend class CFilePathSegmentW;

	static const size_t not_found;
	static const size_t end;

public:
	// Constructs an empty object
	CFilePathSegment8(void);
	// Copy and move constructors
	CFilePathSegment8(const CFilePathSegment8 &src);
	CFilePathSegment8(CFilePathSegment8 &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CFilePathSegment8(const CFilePathSegmentW &path);
	explicit CFilePathSegment8(const CStr8 &str);
	explicit CFilePathSegment8(const CStrW &str);
	explicit CFilePathSegment8(PCSTR sz);
	explicit CFilePathSegment8(PCWSTR sz);
	explicit CFilePathSegment8(const char ch);
	explicit CFilePathSegment8(const wchar_t ch);

	virtual ~CFilePathSegment8(void);

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
	// Returns whether the string is a drive (e.g. C:), only possibly on Windows
	bool IsDrive(void) const noexcept;
	// Returns whether the object has a wildcard character (* or ?)
	bool HasWildcard(void) const;

	// Returns a UTF8 copy of the object
	CFilePathSegment8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CFilePathSegmentW AsWide(void) const;
#ifdef _WIN32
	CFilePathSegmentW AsNative(void) const;
#else
	CFilePathSegment8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CFilePathSegment8 &operator=(const CFilePathSegment8 &src);
	CFilePathSegment8 &operator=(CFilePathSegment8 &&src) noexcept;
	// Assigns the object to the string/character
	CFilePathSegment8 &operator=(const CFilePathSegmentW &path);
	virtual CFilePathSegment8 &operator=(const CStr8 &str);
	virtual CFilePathSegment8 &operator=(const CStrW &str);
	virtual CFilePathSegment8 &operator=(PCSTR sz);
	virtual CFilePathSegment8 &operator=(PCWSTR sz);
	virtual CFilePathSegment8 &operator=(const char ch);
	virtual CFilePathSegment8 &operator=(const wchar_t ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CFilePathSegment8 &path);
	ERRCODE Assign(const CFilePathSegmentW &path);
	virtual ERRCODE Assign(const CStr8 &str);
	virtual ERRCODE Assign(const CStrW &str);
	virtual ERRCODE Assign(PCSTR sz);
	virtual ERRCODE Assign(PCWSTR sz);
	virtual ERRCODE Assign(const char ch);
	virtual ERRCODE Assign(const wchar_t ch);

	friend std::ostream &operator<<(std::ostream &stream, const CFilePathSegment8 &path);

	// On Windows, default compare operators are case insensitive
	// After all "TEMP" and "temp" are the same path
	// For case sensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CFilePathSegment8 &path) const;
	bool operator==(const CStr8 &str) const;
	bool operator==(PCSTR sz) const;
	bool operator==(const char ch) const;

	// Tests for not equal
	bool operator!=(const CFilePathSegment8 &path) const;
	bool operator!=(const CStr8 &str) const;
	bool operator!=(PCSTR sz) const;
	bool operator!=(const char ch) const;

	// Tests for less than
	bool operator<(const CFilePathSegment8 &path) const;
	bool operator<(const CStr8 &str) const;
	bool operator<(PCSTR sz) const;
	bool operator<(const char ch) const;

	// Tests for less than or equal
	bool operator<=(const CFilePathSegment8 &path) const;
	bool operator<=(const CStr8 &str) const;
	bool operator<=(PCSTR sz) const;
	bool operator<=(const char ch) const;

	// Tests for greater than
	bool operator>(const CFilePathSegment8 &path) const;
	bool operator>(const CStr8 &str) const;
	bool operator>(PCSTR sz) const;
	bool operator>(const char ch) const;

	// Tests for greater than or equal
	bool operator>=(const CFilePathSegment8 &path) const;
	bool operator>=(const CStr8 &str) const;
	bool operator>=(PCSTR sz) const;
	bool operator>=(const char ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CFilePathSegment8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int Compare(const CStr8 &str, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int Compare(PCSTR sz, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int Compare(const char ch, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Compares the object (minus extension) against the provided string, optionally case insensitive
	int CompareBase(const CStr8 &strBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int CompareBase(PCSTR szBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Compares the extension against the provided extension, optionally case insensitive
	int CompareExtension(const CStr8 &strExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int CompareExtension(PCSTR szExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Returns the object minus the extension
	CStr8 GetBase(void) const;
	// Returns the extension
	CStr8 GetExtension(void) const;

	// Sets the object's base filename
	bool SetBase(const CStr8 &strBase);
	bool SetBase(PCSTR szBase);
	// Sets the object's extension, if the current filename contains a period then everything after the period is assumed to be the extension
	bool SetExtension(const CStr8 &strExt);
	bool SetExtension(PCSTR szExt);
	// Adds the extension to the object; if the current filename contains a period then the new extension is appended to the end
	bool AddExtension(const CStr8 &strExt);
	bool AddExtension(PCSTR szExt);
	// Removes the extension
	void RemoveExtension(void);

	// Returns whether the string/character is a valid path segment
	static bool IsValidSegment(const CStr8 &strSegment);
	static bool IsValidSegment(PCSTR szSegment);
	static bool IsValidSegment(const char chSegment);

protected:
	PSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);

	static PCSTR FindFirstOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
	static size_t ReverseFind(PCSTR sz, const size_t nStrLen, const char chFind, const size_t nStart = end);
};


//#################################################################################################
class CFilePathSegmentW
{
private:
	friend class CFilePathSegment8;

	static const size_t not_found;
	static const size_t end;

public:
	// Constructs an empty object
	CFilePathSegmentW(void);
	// Copy and move constructors
	CFilePathSegmentW(const CFilePathSegmentW &src);
	CFilePathSegmentW(CFilePathSegmentW &&src) noexcept;
	// Constructs an object set to the string/character
	explicit CFilePathSegmentW(const CFilePathSegment8 &path);
	explicit CFilePathSegmentW(const CStrW &str);
	explicit CFilePathSegmentW(const CStr8 &str);
	explicit CFilePathSegmentW(PCWSTR sz);
	explicit CFilePathSegmentW(PCSTR sz);
	explicit CFilePathSegmentW(const wchar_t ch);
	explicit CFilePathSegmentW(const char ch);

	virtual ~CFilePathSegmentW(void);

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
	// Returns whether the string is a drive (e.g. C:), only possibly on Windows
	bool IsDrive(void) const noexcept;
	// Returns whether the object has a wildcard character (* or ?)
	bool HasWildcard(void) const;

	// Returns a UTF8 copy of the object
	CFilePathSegment8 AsUtf8(void) const;
	// Returns a wchar_t copy of the object
	CFilePathSegmentW AsWide(void) const;
#ifdef _WIN32
	CFilePathSegmentW AsNative(void) const;
#else
	CFilePathSegment8 AsNative(void) const;
#endif

	// Copy and move assignment operators
	CFilePathSegmentW &operator=(const CFilePathSegmentW &src);
	CFilePathSegmentW &operator=(CFilePathSegmentW &&src) noexcept;
	// Assigns the object to the string/character
	CFilePathSegmentW &operator=(const CFilePathSegment8 &path);
	virtual CFilePathSegmentW &operator=(const CStrW &str);
	virtual CFilePathSegmentW &operator=(const CStr8 &str);
	virtual CFilePathSegmentW &operator=(PCWSTR sz);
	virtual CFilePathSegmentW &operator=(PCSTR sz);
	virtual CFilePathSegmentW &operator=(const wchar_t ch);
	virtual CFilePathSegmentW &operator=(const char ch);

	// Assigns the object to the string/character, returns an error code
	ERRCODE Assign(const CFilePathSegmentW &path);
	ERRCODE Assign(const CFilePathSegment8 &path);
	virtual ERRCODE Assign(const CStrW &str);
	virtual ERRCODE Assign(const CStr8 &str);
	virtual ERRCODE Assign(PCWSTR sz);
	virtual ERRCODE Assign(PCSTR sz);
	virtual ERRCODE Assign(const wchar_t ch);
	virtual ERRCODE Assign(const char ch);

	friend std::wostream &operator<<(std::wostream &stream, const CFilePathSegmentW &path);

	// On Windows, default compare operators are case insensitive
	// After all "TEMP" and "temp" are the same path
	// For case sensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CFilePathSegmentW &path) const;
	bool operator==(const CStrW &str) const;
	bool operator==(PCWSTR sz) const;
	bool operator==(const wchar_t ch) const;

	// Tests for not equal
	bool operator!=(const CFilePathSegmentW &path) const;
	bool operator!=(const CStrW &str) const;
	bool operator!=(PCWSTR sz) const;
	bool operator!=(const wchar_t ch) const;

	// Tests for less than
	bool operator<(const CFilePathSegmentW &path) const;
	bool operator<(const CStrW &str) const;
	bool operator<(PCWSTR sz) const;
	bool operator<(const wchar_t ch) const;

	// Tests for less than or equal
	bool operator<=(const CFilePathSegmentW &path) const;
	bool operator<=(const CStrW &str) const;
	bool operator<=(PCWSTR sz) const;
	bool operator<=(const wchar_t ch) const;

	// Tests for greater than
	bool operator>(const CFilePathSegmentW &path) const;
	bool operator>(const CStrW &str) const;
	bool operator>(PCWSTR sz) const;
	bool operator>(const wchar_t ch) const;

	// Tests for greater than or equal
	bool operator>=(const CFilePathSegmentW &path) const;
	bool operator>=(const CStrW &str) const;
	bool operator>=(PCWSTR sz) const;
	bool operator>=(const wchar_t ch) const;

	// Compares the object against the string/character, optionally case insensitive
	int Compare(const CFilePathSegmentW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int Compare(const CStrW &str, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int Compare(PCWSTR sz, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int Compare(const wchar_t ch, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Compares the object (minus extension) against the provided string, optionally case insensitive
	int CompareBase(const CStrW &strBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int CompareBase(PCWSTR szBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Compares the extension against the provided extension, optionally case insensitive
	int CompareExtension(const CStrW &strExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	int CompareExtension(PCWSTR szExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Returns the object minus the extension
	CStrW GetBase(void) const;
	// Returns the extension
	CStrW GetExtension(void) const;

	// Sets the object's base filename
	bool SetBase(const CStrW &strBase);
	bool SetBase(PCWSTR szBase);
	// Sets the object's extension, if the current filename contains a period then everything after the period is assumed to be the extension
	bool SetExtension(const CStrW &strExt);
	bool SetExtension(PCWSTR szExt);
	// Adds the extension to the object; if the current filename contains a period then the new extension is appended to the end
	bool AddExtension(const CStrW &strExt);
	bool AddExtension(PCWSTR szExt);
	// Removes the extension
	void RemoveExtension(void);

	// Returns whether the string/character is a valid path segment
	static bool IsValidSegment(const CStrW &strSegment);
	static bool IsValidSegment(PCWSTR szSegment);
	static bool IsValidSegment(const wchar_t chSegment);

protected:
	PWSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);

	static PCWSTR FindFirstOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
	static size_t ReverseFind(PCWSTR sz, const size_t nStrLen, const wchar_t chFind, const size_t nStart = end);
};


//#################################################################################################
class CMachineName8 final : public CFilePathSegment8
{
private:
	friend class CMachineNameW;

public:
	// Constructs an empty object
	CMachineName8(void) = default;
	// Copy and move constructors
	CMachineName8(const CMachineName8 &src) = default;
	CMachineName8(CMachineName8 &&src) = default;
	// Constructs a string object set to the string/character
	explicit CMachineName8(const CMachineNameW &machine);
	explicit CMachineName8(const CStr8 &str);
	explicit CMachineName8(const CStrW &str);
	explicit CMachineName8(PCSTR sz);
	explicit CMachineName8(PCWSTR sz);
	explicit CMachineName8(const char ch);
	explicit CMachineName8(const wchar_t ch);

	virtual ~CMachineName8(void) override = default;

	// Copy and move assignment operators
	CMachineName8 &operator=(const CMachineName8 &src) = default;
	CMachineName8 &operator=(CMachineName8 &&src) = default;
	// Assigns the string object to the string/character
	CMachineName8 &operator=(const CMachineNameW &machine);
	virtual CMachineName8 &operator=(const CStr8 &str) override;
	virtual CMachineName8 &operator=(const CStrW &str) override;
	virtual CMachineName8 &operator=(PCSTR sz) override;
	virtual CMachineName8 &operator=(PCWSTR sz) override;
	virtual CMachineName8 &operator=(const char ch) override;
	virtual CMachineName8 &operator=(const wchar_t ch) override;

	// Assigns the string object to the string/character, returns an error code
	ERRCODE Assign(const CMachineName8 &machine);
	ERRCODE Assign(const CMachineNameW &machine);
	virtual ERRCODE Assign(const CStr8 &str) override;
	virtual ERRCODE Assign(const CStrW &str) override;
	virtual ERRCODE Assign(PCSTR sz) override;
	virtual ERRCODE Assign(PCWSTR sz) override;
	virtual ERRCODE Assign(const char ch) override;
	virtual ERRCODE Assign(const wchar_t ch) override;

	// Returns whether the string/character is a valid machine name
	static bool IsValidMachineName(const CStr8 &strMachineName);
	static bool IsValidMachineName(PCSTR szMachineName);
	static bool IsValidMachineName(const char chMachineName);

private:
	static PCSTR FindStrPtr(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
	static PCSTR FindFirstNotOf(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
};


//#################################################################################################
class CMachineNameW final : public CFilePathSegmentW
{
private:
	friend class CMachineName8;

public:
	// Constructs an empty object
	CMachineNameW(void) = default;
	// Copy and move constructors
	CMachineNameW(const CMachineNameW &src) = default;
	CMachineNameW(CMachineNameW &&src) = default;
	// Constructs a string object set to the string/character
	explicit CMachineNameW(const CMachineName8 &machine);
	explicit CMachineNameW(const CStrW &str);
	explicit CMachineNameW(const CStr8 &str);
	explicit CMachineNameW(PCWSTR sz);
	explicit CMachineNameW(PCSTR sz);
	explicit CMachineNameW(const wchar_t ch);
	explicit CMachineNameW(const char ch);

	virtual ~CMachineNameW(void) override = default;

	// Copy and move assignment operators
	CMachineNameW &operator=(const CMachineNameW &src) = default;
	CMachineNameW &operator=(CMachineNameW &&src) = default;
	// Assigns the string object to the string/character
	CMachineNameW &operator=(const CMachineName8 &machine);
	virtual CMachineNameW &operator=(const CStrW &str) override;
	virtual CMachineNameW &operator=(const CStr8 &str) override;
	virtual CMachineNameW &operator=(PCWSTR sz) override;
	virtual CMachineNameW &operator=(PCSTR sz) override;
	virtual CMachineNameW &operator=(const wchar_t ch) override;
	virtual CMachineNameW &operator=(const char ch) override;

	// Assigns the string object to the string/character, returns an error code
	ERRCODE Assign(const CMachineNameW &machine);
	ERRCODE Assign(const CMachineName8 &machine);
	virtual ERRCODE Assign(const CStrW &str) override;
	virtual ERRCODE Assign(const CStr8 &str) override;
	virtual ERRCODE Assign(PCWSTR sz) override;
	virtual ERRCODE Assign(PCSTR sz) override;
	virtual ERRCODE Assign(const wchar_t ch) override;
	virtual ERRCODE Assign(const char ch) override;

	// Returns whether the string/character is a valid machine name
	static bool IsValidMachineName(const CStrW &strMachineName);
	static bool IsValidMachineName(PCWSTR szMachineName);
	static bool IsValidMachineName(const wchar_t chMachineName);

private:
	static PCWSTR FindStrPtr(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
	static PCWSTR FindFirstNotOf(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
};


//#################################################################################################
class CFilePath8 final
{
private:
	friend class CFilePathW;

	enum EPathType : uint8_t
	{
		EPT_Empty = 0,
		EPT_FullyRooted,			// Path fully rooted - "C:\Windows" or "/etc/system"
		EPT_Relative,				// Path fragment - "files\drivers" or "files/drivers"
		EPT_DriveRelative,			// Path relative to a given drive - "C:temp"
		EPT_CurrentDriveRelative,	// Path relative to current drive - "\Files\Current"
		EPT_UNC						// UNC paths - "\\server\share" or "//server/share"
	};

public:
#ifdef _WIN32
	enum EKernelPath : uint8_t
	{
		EKP_Never = 0,				// Never add the kernel prefix to paths
		EKP_Always,					// Always add the kernel prefix to paths
		EKP_Auto					// Add the kernel prefix if the path length exceeds 256
	};
#endif

	// Constructs an empty object
	CFilePath8(void);
	// Copy and move constructors
	CFilePath8(const CFilePath8 &src);
	CFilePath8(CFilePath8 &&src) noexcept;
	// Constructs an object set to the path/string/character
	explicit CFilePath8(const CFilePathW &path);
	explicit CFilePath8(const CFilePathSegment8 &path);
	explicit CFilePath8(const CFilePathSegmentW &path);
	explicit CFilePath8(const CMachineName8 &machine);
	explicit CFilePath8(const CMachineNameW &machine);
	explicit CFilePath8(const CStr8 &str);
	explicit CFilePath8(const CStrW &str);
	explicit CFilePath8(PCSTR sz);
	explicit CFilePath8(PCWSTR sz);
	explicit CFilePath8(const char ch);
	explicit CFilePath8(const wchar_t ch);

	// Returns a string representing the full path
	operator CStr8(void) const;
#ifdef _WIN32
	// Returns a string representing the full path, set eKernalPath to allow for paths greater than 256 in length
	CStr8 Get(const EKernelPath eKernelPath = EKP_Auto) const;
	// Returns the length in characters, set eKernalPath to allow for paths greater than 256 in length
	size_t GetLength(const bool bIncludeNullTerm = false, const EKernelPath eKernelPath = EKP_Auto) const;
	// Returns the size in bytes, set eKernalPath to allow for paths greater than 256 in length
	size_t GetSize(const bool bIncludeNullTerm = false, const EKernelPath eKernelPath = EKP_Auto) const;
#else
	// Returns a string representing the full path
	CStr8 Get(void) const;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const;
#endif

	// Copy and move assignment operators
	CFilePath8 &operator=(const CFilePath8 &src);
	CFilePath8 &operator=(CFilePath8 &&src) noexcept;
	// Assigns the object to the path/string/character
	CFilePath8 &operator=(const CFilePathW &path);
	CFilePath8 &operator=(const CFilePathSegment8 &path);
	CFilePath8 &operator=(const CFilePathSegmentW &path);
	CFilePath8 &operator=(const CMachineName8 &machine);
	CFilePath8 &operator=(const CMachineNameW &machine);
	CFilePath8 &operator=(const CStr8 &str);
	CFilePath8 &operator=(const CStrW &str);
	CFilePath8 &operator=(PCSTR sz);
	CFilePath8 &operator=(PCWSTR sz);
	CFilePath8 &operator=(const char ch);
	CFilePath8 &operator=(const wchar_t ch);

	// Assigns the object to the path/string/character, returns an error code
	ERRCODE Assign(const CFilePath8 &path);
	ERRCODE Assign(const CFilePathW &path);
	ERRCODE Assign(const CFilePathSegment8 &path);
	ERRCODE Assign(const CFilePathSegmentW &path);
	ERRCODE Assign(const CMachineName8 &machine);
	ERRCODE Assign(const CMachineNameW &machine);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(PCSTR sz);
	ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(const char ch);
	ERRCODE Assign(const wchar_t ch);

	// Appends the path
	CFilePath8 &operator+=(const CFilePath8 &path);
	CFilePath8 &operator+=(const CFilePathW &path);
	CFilePath8 &operator+=(const CFilePathSegment8 &path);
	CFilePath8 &operator+=(const CFilePathSegmentW &path);
	CFilePath8 &operator+=(const CMachineName8 &machine);
	CFilePath8 &operator+=(const CMachineNameW &machine);

	// Prepends the path, returns an error code
	ERRCODE Prepend(const CFilePath8 &path);
	ERRCODE Prepend(const CFilePathW &path);
	ERRCODE Prepend(const CFilePathSegment8 &path);
	ERRCODE Prepend(const CFilePathSegmentW &path);
	ERRCODE Prepend(const CMachineName8 &machine);
	ERRCODE Prepend(const CMachineNameW &machine);

	// Appends the path, returns an error code
	ERRCODE Append(const CFilePath8 &path);
	ERRCODE Append(const CFilePathW &path);
	ERRCODE Append(const CFilePathSegment8 &path);
	ERRCODE Append(const CFilePathSegmentW &path);
	ERRCODE Append(const CMachineName8 &machine);
	ERRCODE Append(const CMachineNameW &machine);

#ifndef _WIN32
	// Returns the concatenation of two paths
	friend CFilePath8 operator+(const CFilePath8 &path1, const CFilePath8 &path2);
	friend CFilePath8 operator+(const CFilePath8 &path1, const CFilePathW &path2);
	friend CFilePath8 operator+(const CFilePathW &path1, const CFilePath8 &path2);
	friend CFilePath8 operator+(const CFilePathW &path1, const CFilePathW &path2);
	friend CFilePath8 operator+(const CFilePath8 &path1, const CFilePathSegment8 &path2);
	friend CFilePath8 operator+(const CFilePath8 &path1, const CFilePathSegmentW &path2);
	friend CFilePath8 operator+(const CFilePathW &path1, const CFilePathSegment8 &path2);
	friend CFilePath8 operator+(const CFilePathW &path1, const CFilePathSegmentW &path2);
	friend CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePath8 &path2);
	friend CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePathW &path2);
	friend CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePath8 &path2);
	friend CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePathW &path2);
	friend CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePathSegment8 &path2);
	friend CFilePath8 operator+(const CFilePathSegment8 &path1, const CFilePathSegmentW &path2);
	friend CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePathSegment8 &path2);
	friend CFilePath8 operator+(const CFilePathSegmentW &path1, const CFilePathSegmentW &path2);
	friend CFilePath8 operator+(const CMachineName8 &machine, const CFilePath8 &path);
	friend CFilePath8 operator+(const CMachineName8 &machine, const CFilePathW &path);
	friend CFilePath8 operator+(const CMachineNameW &machine, const CFilePath8 &path);
	friend CFilePath8 operator+(const CMachineNameW &machine, const CFilePathW &path);
	friend CFilePath8 operator+(const CMachineName8 &machine, const CFilePathSegment8 &path);
	friend CFilePath8 operator+(const CMachineName8 &machine, const CFilePathSegmentW &path);
	friend CFilePath8 operator+(const CMachineNameW &machine, const CFilePathSegment8 &path);
	friend CFilePath8 operator+(const CMachineNameW &machine, const CFilePathSegmentW &path);
#endif

	friend std::ostream &operator<<(std::ostream &stream, const CFilePath8 &path);

	// On Windows, default compare operators are case insensitive
	// After all "C:\TEMP" and "c:\temp" are the same path
	// For case sensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CFilePath8 &path) const;
	bool operator==(const CFilePathSegment8 &path) const;

	// Tests for not equal
	bool operator!=(const CFilePath8 &path) const;
	bool operator!=(const CFilePathSegment8 &path) const;

	// Tests for less than
	bool operator<(const CFilePath8 &path) const;

	// Tests for less than or equal
	bool operator<=(const CFilePath8 &path) const;

	// Tests for greater than
	bool operator>(const CFilePath8 &path) const;

	// Tests for greater than or equal
	bool operator>=(const CFilePath8 &path) const;

	// Compares the object against the path, optionally case insensitive
	bool Compare(const CFilePath8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool Compare(const CFilePathSegment8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Compares the leftmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareLeft(const CFilePath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Compares the rightmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareRight(const CFilePath8 &path, const size_t nSegmentCount, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Tests if the path object contains given path
	bool Contains(const CFilePath8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool Contains(const CFilePathSegment8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Tests the filename minus extension against the give filename minus extension, optionally case insensitive
	bool CompareBase(const CFilePath8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareBase(const CFilePathSegment8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareBase(const CStr8 &strBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareBase(PCSTR szBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Tests the filename extension against the give filename extension, optionally case insensitive
	bool CompareExtension(const CFilePath8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareExtension(const CFilePathSegment8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareExtension(const CStr8 &strExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareExtension(PCSTR szExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Returns the count of path segments that the path object and provided path match starting at the left, optionally case insensitive
	size_t CountCompare(const CFilePath8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Returns the count of path segments that the path object and provided path match starting at the right, optionally case insensitive
	size_t ReverseCountCompare(const CFilePath8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Empties the path object, freeing the memory
	void Empty(void);
	// Sets a path to be anchored, returns true if the path was modified; 'c:temp' becomes 'c:\temp', and 'windows\system' becomes '\windows\system32'
	bool SetAnchor(const bool bAnchor = true);
	// Canonicalize paths removing embedded dots; 'c:\windows\..\temp' becomes 'c:\temp'
	bool SimplifyDots(void);

	// Returns whether the path is empty
	bool IsEmpty(void) const noexcept;
	// Returns whether the path is rooted with a drive letter; 'c:\windows'
	bool IsDrive(void) const noexcept;
	// Returns whether the path is a UNC path
	bool IsUNC(void) const noexcept;
	// Returns whether the path is fully rooted; "c:\windows" or "/etc/system"
	bool IsFullyRooted(void) const noexcept;
	// Returns whether the path is relative (not connected to a drive); 'files\test'
	bool IsRelative(void) const noexcept;
	// Returns whether the path is a root drive letter; 'C:\'
	bool IsRoot(void) const noexcept;
	// Returns whether the path is anchored; 'c:\temp' or '\windows\system32'
	bool IsAnchored(void) const noexcept;
	// Returns whether the paths begins with dots; '.' or '..'
	bool HasLeadingDots(void) const;
	// Returns whether the path ends with an extension
	bool HasExtension(void) const;
	// Returns whether the path contains a wildcard; '*' or '?'
	bool HasWildcard(void) const;

	// Returns the number of segments in the path
	size_t GetSegmentCount(void) const noexcept;
	// Returns the segment at a given location, or an empty object if beyond the end of the path
	CFilePathSegment8 GetSegment(const size_t nSegment) const;
	// Returns the first segment in the path, or an empty object if the path is empty
	CFilePathSegment8 GetFirstSegment(void) const;
	// Returns the last segment in the path, or an empty object if the path is empty
	CFilePathSegment8 GetLastSegment(void) const;
	// Returns the leftmost 'nSegmentCount' segments from the path object
	CFilePath8 GetLeft(const size_t nSegmentCount) const;
	// Returns the rightmost 'nSegmentCount' segments from the path object
	CFilePath8 GetRight(const size_t nSegmentCount) const;
	// Returns the middle 'nSegmentCount' segments from the path object starting at 'nSegment'
	CFilePath8 GetMid(const size_t nSegment, const size_t nSegmentCount) const;
	// Removes the leftmost 'nSegmentCount' from the path object
	CFilePath8 &TrimLeft(const size_t nSegmentCount = 1);
	// Removes the rightmost 'nSegmentCount' from the path object
	CFilePath8 &TrimRight(const size_t nSegmentCount = 1);
	// Removes everything except the leftmost 'nSegmentCount' from the path object
	CFilePath8 &CropLeft(const size_t nSegmentCount = 1);
	// Removes everything except the rightmost 'nSegmentCount' from the path object
	CFilePath8 &CropRight(const size_t nSegmentCount = 1);
	// Inserts the given path into the path object before the given segment, returns an error code
	ERRCODE Insert(const size_t nSegment, const CFilePath8 &path);
	ERRCODE Insert(const size_t nSegment, const CFilePathSegment8 &path);
	ERRCODE Insert(const CMachineName8 &machine);
	// Removes 'nSegmentCount' segments from the path object starting at 'nSegment'
	CFilePath8 &Delete(const size_t nSegment, const size_t nSegmentCount = 1);
	// Removes all segments from the path object that match the given path segment, returns the number of segments removed
	size_t Remove(const CFilePathSegment8 &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP);
	// Replaces all segments from the path with the new path segment, returns the number of segments replaced
	size_t Replace(const CFilePathSegment8 &pathOld, const CFilePathSegment8 &pathNew, const bool bCaseInsensitive = FILE_DEFAULT_CMP);
	// Replaces segment 'nSegment' with the give path segment
	CFilePath8 &Modify(const size_t nSegment, const CFilePathSegment8 &path);

	// Returns the machine name, or an empty segment if the path is not a UNC path
	CMachineName8 GetMachine(void) const;
	// Returns the share name, or an empty segment if the path is not a UNC path
	CFilePathSegment8 GetShare(void) const;
	// Returns the machine and share names, or an empty segment if the path is not a UNC path
	CFilePath8 GetMachineAndShare(void) const;
	// Returns the drive letter (for example 'C:'), or an empty segment if the path is not a drive letter path
	CFilePathSegment8 GetDrive(void) const;
	// Returns the full path minus the last segment (which is assumed to be the filename)
	CFilePath8 GetPath(void) const;
	// Returns the full path including the filename but excluding the root which is the drive letter or machine name and share name
	CFilePath8 GetRelativePath(void) const;
	// Returns the filename, which is assumed to be the last extension, or an empty object if the path is empty or only contains a root like a drive letter
	CFilePathSegment8 GetFilename(void) const;
	// Returns filename minus the extension
	CStr8 GetBase(void) const;
	// Returns extension, which is assumed to be everything after the last period in the last segment
	CStr8 GetExtension(void) const;

	// Sets the filename minus the extension
	bool SetBase(const CStr8 &strBase);
	bool SetBase(PCSTR szBase);
	// Sets/changes the extension, which is assumed to be everything after the last period in the last segment
	bool SetExtension(const CStr8 &strExt);
	bool SetExtension(PCSTR szExt);
	// Appends a new extension even if the current filename contains a period
	bool AddExtension(const CStr8 &strExt);
	bool AddExtension(PCSTR szExt);
	// Removes the extension (if present), which is assumed to be everything after the last period in the last segment
	void RemoveExtension(void);

#ifdef _WIN32
	// Splits the given drive letter path into drive letter, path, and filename
	static bool SplitDrivePath(PCSTR szPath, CStr8 &strDrive, CStr8 &strPath, CStr8 &strFileBase, CStr8 &strFileExt);
	static bool SplitDrivePath(PCSTR szPath, CStr8 &strDrive, CStr8 &strPath, CStr8 &strFilename);
#endif
	// Splits the given UNC path into machine name, share name, path, and filename
	static bool SplitUNCPath(PCSTR szPath, CStr8 &strMachine, CStr8 &strShare, CStr8 &strPath, CStr8 &strFileBase, CStr8 &strFileExt);
	static bool SplitUNCPath(PCSTR szPath, CStr8 &strMachine, CStr8 &strShare, CStr8 &strPath, CStr8 &strFilename);
	// Splits the given path into path and filename
	static bool SplitPath(PCSTR szPath, CStr8 &strPath, CStr8 &strFileBase, CStr8 &strFileExt);
	static bool SplitPath(PCSTR szPath, CStr8 &strPath, CStr8 &strFilename);

	// Returns whether the string/character is a valid path
	static bool IsValidPath(const CStr8 &strPath);
	static bool IsValidPath(PCSTR szPath);
	static bool IsValidPath(const char chPath);

	void DebugPrint(const bool bAppendNewline = true) const;

private:
	EPathType m_eType;
	std::vector<std::unique_ptr<CFilePathSegment8>> m_vecSegments;

	ERRCODE ParsePath(const CStr8 &strPath);
	bool SafeDotAppend(const CFilePath8 &path1, const CFilePath8 &path2);
	bool SafeDotAppend(const CFilePath8 &path1, const CFilePathW &path2);
};


//#################################################################################################
class CFilePathW final
{
private:
	friend class CFilePath8;

	enum EPathType : uint8_t
	{
		EPT_Empty = 0,
		EPT_FullyRooted,			// Path fully rooted - "C:\Windows" or "/etc/system"
		EPT_Relative,				// Path fragment - "files\drivers" or "files/drivers"
		EPT_DriveRelative,			// Path relative to a given drive - "C:temp"
		EPT_CurrentDriveRelative,	// Path relative to current drive - "\Files\Current"
		EPT_UNC						// UNC paths - "\\server\share" or "//server/share"
	};

public:
#ifdef _WIN32
	enum EKernelPath : uint8_t
	{
		EKP_Never = 0,				// Never add the kernel prefix to paths
		EKP_Always,					// Always add the kernel prefix to paths
		EKP_Auto					// Add the kernel prefix if the path length exceeds 256
	};
#endif

	// Constructs an empty object
	CFilePathW(void);
	// Copy and move constructors
	CFilePathW(const CFilePathW &src);
	CFilePathW(CFilePathW &&src) noexcept;
	// Constructs an object set to the path/string/character
	explicit CFilePathW(const CFilePath8 &path);
	explicit CFilePathW(const CFilePathSegmentW &path);
	explicit CFilePathW(const CFilePathSegment8 &path);
	explicit CFilePathW(const CMachineNameW &machine);
	explicit CFilePathW(const CMachineName8 &machine);
	explicit CFilePathW(const CStrW &str);
	explicit CFilePathW(const CStr8 &str);
	explicit CFilePathW(PCWSTR sz);
	explicit CFilePathW(PCSTR sz);
	explicit CFilePathW(const wchar_t ch);
	explicit CFilePathW(const char ch);

	// Returns a string representing the full path
	operator CStrW(void) const;
#ifdef _WIN32
	// Returns a string representing the full path, set eKernalPath to allow for paths greater than 256 in length
	CStrW Get(const EKernelPath eKernelPath = EKP_Auto) const;
	// Returns the length in characters, set eKernalPath to allow for paths greater than 256 in length
	size_t GetLength(const bool bIncludeNullTerm = false, const EKernelPath eKernelPath = EKP_Auto) const;
	// Returns the size in bytes, set eKernalPath to allow for paths greater than 256 in length
	size_t GetSize(const bool bIncludeNullTerm = false, const EKernelPath eKernelPath = EKP_Auto) const;
#else
	// Returns a string representing the full path
	CStrW Get(void) const;
	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const;
#endif

	// Copy and move assignment operators
	CFilePathW &operator=(const CFilePathW &src);
	CFilePathW &operator=(CFilePathW &&src) noexcept;
	// Assigns the object to the path/string/character
	CFilePathW &operator=(const CFilePath8 &path);
	CFilePathW &operator=(const CFilePathSegmentW &path);
	CFilePathW &operator=(const CFilePathSegment8 &path);
	CFilePathW &operator=(const CMachineNameW &machine);
	CFilePathW &operator=(const CMachineName8 &machine);
	CFilePathW &operator=(const CStrW &str);
	CFilePathW &operator=(const CStr8 &str);
	CFilePathW &operator=(PCWSTR sz);
	CFilePathW &operator=(PCSTR sz);
	CFilePathW &operator=(const wchar_t ch);
	CFilePathW &operator=(const char ch);

	// Assigns the object to the path/string/character, returns an error code
	ERRCODE Assign(const CFilePathW &path);
	ERRCODE Assign(const CFilePath8 &path);
	ERRCODE Assign(const CFilePathSegmentW &path);
	ERRCODE Assign(const CFilePathSegment8 &path);
	ERRCODE Assign(const CMachineNameW &machine);
	ERRCODE Assign(const CMachineName8 &machine);
	ERRCODE Assign(const CStrW &str);
	ERRCODE Assign(const CStr8 &str);
	ERRCODE Assign(PCWSTR sz);
	ERRCODE Assign(PCSTR sz);
	ERRCODE Assign(const wchar_t ch);
	ERRCODE Assign(const char ch);

	// Appends the path
	CFilePathW &operator+=(const CFilePathW &path);
	CFilePathW &operator+=(const CFilePath8 &path);
	CFilePathW &operator+=(const CFilePathSegmentW &path);
	CFilePathW &operator+=(const CFilePathSegment8 &path);
	CFilePathW &operator+=(const CMachineNameW &machine);
	CFilePathW &operator+=(const CMachineName8 &machine);

	// Prepends the path, returns an error code
	ERRCODE Prepend(const CFilePathW &path);
	ERRCODE Prepend(const CFilePath8 &path);
	ERRCODE Prepend(const CFilePathSegmentW &path);
	ERRCODE Prepend(const CFilePathSegment8 &path);
	ERRCODE Prepend(const CMachineNameW &machine);
	ERRCODE Prepend(const CMachineName8 &machine);

	// Appends the path, returns an error code
	ERRCODE Append(const CFilePathW &path);
	ERRCODE Append(const CFilePath8 &path);
	ERRCODE Append(const CFilePathSegmentW &path);
	ERRCODE Append(const CFilePathSegment8 &path);
	ERRCODE Append(const CMachineNameW &machine);
	ERRCODE Append(const CMachineName8 &machine);

#ifdef _WIN32
	// Returns the concatenation of two paths
	friend CFilePathW operator+(const CFilePathW &path1, const CFilePathW &path2);
	friend CFilePathW operator+(const CFilePathW &path1, const CFilePath8 &path2);
	friend CFilePathW operator+(const CFilePath8 &path1, const CFilePathW &path2);
	friend CFilePathW operator+(const CFilePath8 &path1, const CFilePath8 &path2);
	friend CFilePathW operator+(const CFilePathW &path1, const CFilePathSegmentW &path2);
	friend CFilePathW operator+(const CFilePathW &path1, const CFilePathSegment8 &path2);
	friend CFilePathW operator+(const CFilePath8 &path1, const CFilePathSegmentW &path2);
	friend CFilePathW operator+(const CFilePath8 &path1, const CFilePathSegment8 &path2);
	friend CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePathW &path2);
	friend CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePath8 &path2);
	friend CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePathW &path2);
	friend CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePath8 &path2);
	friend CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePathSegmentW &path2);
	friend CFilePathW operator+(const CFilePathSegmentW &path1, const CFilePathSegment8 &path2);
	friend CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePathSegmentW &path2);
	friend CFilePathW operator+(const CFilePathSegment8 &path1, const CFilePathSegment8 &path2);
	friend CFilePathW operator+(const CMachineNameW &machine, const CFilePathW &path);
	friend CFilePathW operator+(const CMachineNameW &machine, const CFilePath8 &path);
	friend CFilePathW operator+(const CMachineName8 &machine, const CFilePathW &path);
	friend CFilePathW operator+(const CMachineName8 &machine, const CFilePath8 &path);
	friend CFilePathW operator+(const CMachineNameW &machine, const CFilePathSegmentW &path);
	friend CFilePathW operator+(const CMachineNameW &machine, const CFilePathSegment8 &path);
	friend CFilePathW operator+(const CMachineName8 &machine, const CFilePathSegmentW &path);
	friend CFilePathW operator+(const CMachineName8 &machine, const CFilePathSegment8 &path);
#endif

	friend std::wostream &operator<<(std::wostream &stream, const CFilePathW &path);

	// On Windows, default compare operators are case insensitive
	// After all "C:\TEMP" and "c:\temp" are the same path
	// For case sensitive compares use the Compare() function

	// Tests for equal
	bool operator==(const CFilePathW &path) const;
	bool operator==(const CFilePathSegmentW &path) const;

	// Tests for not equal
	bool operator!=(const CFilePathW &path) const;
	bool operator!=(const CFilePathSegmentW &path) const;

	// Tests for less than
	bool operator<(const CFilePathW &path) const;

	// Tests for less than or equal
	bool operator<=(const CFilePathW &path) const;

	// Tests for greater than
	bool operator>(const CFilePathW &path) const;

	// Tests for greater than or equal
	bool operator>=(const CFilePathW &path) const;

	// Compares the object against the path, optionally case insensitive
	bool Compare(const CFilePathW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool Compare(const CFilePathSegmentW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Compares the leftmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareLeft(const CFilePathW &path, const size_t nSegmentCount, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Compares the rightmost segments of the object against the provided path, up to 'nSegmentCount' segments, optionally case insensitive
	bool CompareRight(const CFilePathW &path, const size_t nSegmentCount, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Tests if the path object contains given path
	bool Contains(const CFilePathW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool Contains(const CFilePathSegmentW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Tests the filename minus extension against the give filename minus extension, optionally case insensitive
	bool CompareBase(const CFilePathW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareBase(const CFilePathSegmentW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareBase(const CStrW &strBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareBase(PCWSTR szBase, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Tests the filename extension against the give filename extension, optionally case insensitive
	bool CompareExtension(const CFilePathW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareExtension(const CFilePathSegmentW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareExtension(const CStrW &strExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	bool CompareExtension(PCWSTR szExt, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Returns the count of path segments that the path object and provided path match starting at the left, optionally case insensitive
	size_t CountCompare(const CFilePathW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;
	// Returns the count of path segments that the path object and provided path match starting at the right, optionally case insensitive
	size_t ReverseCountCompare(const CFilePathW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP) const;

	// Empties the path object, freeing the memory
	void Empty(void);
	// Sets a path to be anchored, returns true if the path was modified; 'c:temp' becomes 'c:\temp', and 'windows\system' becomes '\windows\system32'
	bool SetAnchor(const bool bAnchor = true);
	// Canonicalize paths removing embedded dots; 'c:\windows\..\temp' becomes 'c:\temp'
	bool SimplifyDots(void);

	// Returns whether the path is empty
	bool IsEmpty(void) const noexcept;
	// Returns whether the path is rooted with a drive letter; 'c:\windows'
	bool IsDrive(void) const noexcept;
	// Returns whether the path is a UNC path
	bool IsUNC(void) const noexcept;
	// Returns whether the path is fully rooted; "c:\windows" or "/etc/system"
	bool IsFullyRooted(void) const noexcept;
	// Returns whether the path is relative (not connected to a drive); 'files\test'
	bool IsRelative(void) const noexcept;
	// Returns whether the path is a root drive letter; 'C:\'
	bool IsRoot(void) const noexcept;
	// Returns whether the path is anchored; 'c:\temp' or '\windows\system32'
	bool IsAnchored(void) const noexcept;
	// Returns whether the paths begins with dots; '.' or '..'
	bool HasLeadingDots(void) const;
	// Returns whether the path ends with an extension
	bool HasExtension(void) const;
	// Returns whether the path contains a wildcard; '*' or '?'
	bool HasWildcard(void) const;

	// Returns the number of segments in the path
	size_t GetSegmentCount(void) const noexcept;
	// Returns the segment at a given location, or an empty object if beyond the end of the path
	CFilePathSegmentW GetSegment(const size_t nSegment) const;
	// Returns the first segment in the path, or an empty object if the path is empty
	CFilePathSegmentW GetFirstSegment(void) const;
	// Returns the last segment in the path, or an empty object if the path is empty
	CFilePathSegmentW GetLastSegment(void) const;
	// Returns the leftmost 'nSegmentCount' segments from the path object
	CFilePathW GetLeft(const size_t nSegmentCount) const;
	// Returns the rightmost 'nSegmentCount' segments from the path object
	CFilePathW GetRight(const size_t nSegmentCount) const;
	// Returns the middle 'nSegmentCount' segments from the path object starting at 'nSegment'
	CFilePathW GetMid(const size_t nSegment, const size_t nSegmentCount) const;
	// Removes the leftmost 'nSegmentCount' from the path object
	CFilePathW &TrimLeft(const size_t nSegmentCount = 1);
	// Removes the rightmost 'nSegmentCount' from the path object
	CFilePathW &TrimRight(const size_t nSegmentCount = 1);
	// Removes everything except the leftmost 'nSegmentCount' from the path object
	CFilePathW &CropLeft(const size_t nSegmentCount = 1);
	// Removes everything except the rightmost 'nSegmentCount' from the path object
	CFilePathW &CropRight(const size_t nSegmentCount = 1);
	// Inserts the given path into the path object before the given segment, returns an error code
	ERRCODE Insert(const size_t nSegment, const CFilePathW &path);
	ERRCODE Insert(const size_t nSegment, const CFilePathSegmentW &path);
	ERRCODE Insert(const CMachineNameW &machine);
	// Removes 'nSegmentCount' segments from the path object starting at 'nSegment'
	CFilePathW &Delete(const size_t nSegment, const size_t nSegmentCount = 1);
	// Removes all segments from the path object that match the given path segment, returns the number of segments removed
	size_t Remove(const CFilePathSegmentW &path, const bool bCaseInsensitive = FILE_DEFAULT_CMP);
	// Replaces all segments from the path with the new path segment, returns the number of segments replaced
	size_t Replace(const CFilePathSegmentW &pathOld, const CFilePathSegmentW &pathNew, const bool bCaseInsensitive = FILE_DEFAULT_CMP);
	// Replaces segment 'nSegment' with the give path segment
	CFilePathW &Modify(const size_t nSegment, const CFilePathSegmentW &path);

	// Returns the machine name, or an empty segment if the path is not a UNC path
	CMachineNameW GetMachine(void) const;
	// Returns the share name, or an empty segment if the path is not a UNC path
	CFilePathSegmentW GetShare(void) const;
	// Returns the machine and share names, or an empty segment if the path is not a UNC path
	CFilePathW GetMachineAndShare(void) const;
	// Returns the drive letter (for example 'C:'), or an empty segment if the path is not a drive letter path
	CFilePathSegmentW GetDrive(void) const;
	// Returns the full path minus the last segment (which is assumed to be the filename)
	CFilePathW GetPath(void) const;
	// Returns the full path including the filename but excluding the root which is the drive letter or machine name and share name
	CFilePathW GetRelativePath(void) const;
	// Returns the filename, which is assumed to be the last extension, or an empty object if the path is empty or only contains a root like a drive letter
	CFilePathSegmentW GetFilename(void) const;
	// Returns filename minus the extension
	CStrW GetBase(void) const;
	// Returns extension, which is assumed to be everything after the last period in the last segment
	CStrW GetExtension(void) const;

	// Sets the filename minus the extension
	bool SetBase(const CStrW &strBase);
	bool SetBase(PCWSTR szBase);
	// Sets/changes the extension, which is assumed to be everything after the last period in the last segment
	bool SetExtension(const CStrW &strExt);
	bool SetExtension(PCWSTR szExt);
	// Appends a new extension even if the current filename contains a period
	bool AddExtension(const CStrW &strExt);
	bool AddExtension(PCWSTR szExt);
	// Removes the extension (if present), which is assumed to be everything after the last period in the last segment
	void RemoveExtension(void);

#ifdef _WIN32
	// Splits the given drive letter path into drive letter, path, and filename
	static bool SplitDrivePath(PCWSTR szPath, CStrW &strDrive, CStrW &strPath, CStrW &strFileBase, CStrW &strFileExt);
	static bool SplitDrivePath(PCWSTR szPath, CStrW &strDrive, CStrW &strPath, CStrW &strFilename);
#endif
	// Splits the given UNC path into machine name, share name, path, and filename
	static bool SplitUNCPath(PCWSTR szPath, CStrW &strMachine, CStrW &strShare, CStrW &strPath, CStrW &strFileBase, CStrW &strFileExt);
	static bool SplitUNCPath(PCWSTR szPath, CStrW &strMachine, CStrW &strShare, CStrW &strPath, CStrW &strFilename);
	// Splits the given path into path and filename
	static bool SplitPath(PCWSTR szPath, CStrW &strPath, CStrW &strFileBase, CStrW &strFileExt);
	static bool SplitPath(PCWSTR szPath, CStrW &strPath, CStrW &strFilename);

	// Returns whether the string/character is a valid path
	static bool IsValidPath(const CStrW &strPath);
	static bool IsValidPath(PCWSTR szPath);
	static bool IsValidPath(const wchar_t chPath);

	void DebugPrint(const bool bAppendNewline = true) const;

private:
	EPathType m_eType;
	std::vector<std::unique_ptr<CFilePathSegmentW>> m_vecSegments;

	ERRCODE ParsePath(const CStrW &strPath);
	bool SafeDotAppend(const CFilePathW &path1, const CFilePathW &path2);
	bool SafeDotAppend(const CFilePathW &path1, const CFilePath8 &path2);
};


#ifdef _WIN32
using CFilePathSegment = CFilePathSegmentW;
using CMachineName = CMachineNameW;
using CFilePath = CFilePathW;
#else
using CFilePathSegment = CFilePathSegment8;
using CMachineName = CMachineName8;
using CFilePath = CFilePath8;
#endif

NS_END

#pragma once

NS_BEGIN

// Forward declarations
class CDoubleNullStr8;
class CDoubleNullStrW;

//#################################################################################################
class CDoubleNullStr8 final
{
public:
	// Constructs an empty string object
	CDoubleNullStr8(void);
	// Copy and move constructors
	CDoubleNullStr8(const CDoubleNullStr8 &src);
	CDoubleNullStr8(CDoubleNullStr8 &&src) noexcept;
	// Constructs a string object set to the string, which can be already double null terminated
	explicit CDoubleNullStr8(PCSTR sz, const bool bDoubleNullTerm = false);

	~CDoubleNullStr8(void);

	// Returns the length in characters (including null-terminators)
	size_t GetLength(void) const;
	// Returns the size in bytes (including null-terminators)
	size_t GetSize(void) const;

	// Returns a pointer to the raw string data
	operator PCSTR(void) const noexcept;
	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;
	// Empties the string, freeing the memory
	void Empty(void);

	// Copy and move assignment operators
	CDoubleNullStr8 &operator=(const CDoubleNullStr8 &src);
	CDoubleNullStr8 &operator=(CDoubleNullStr8 &&src) noexcept;
	// Assigns the string object to the double null string
	CDoubleNullStr8 &operator=(PCSTR sz);

	// Assigns the string object to the double null string
	void Assign(const CDoubleNullStr8 &str);
	void Assign(PCSTR sz, const bool bDoubleNullTerm = false);

	// Appends the string object to the double null string
	CDoubleNullStr8 &operator+=(const CDoubleNullStr8 &str);
	CDoubleNullStr8 &operator+=(PCSTR sz);

	// Appends the string object to the double null string
	void Append(const CDoubleNullStr8 &str);
	void Append(PCSTR sz, const bool bDoubleNullTerm = false);

	friend std::ostream &operator<<(std::ostream &stream, const CDoubleNullStr8 &str);

	// Returns the number of strings in the double null string object
	size_t GetCount(void) const noexcept;
	// Returns a pointer to the raw string at the given index, or null if beyond the array
	PCSTR GetAt(const size_t nIndex) const;

	// Returns an array of strings present in the double null string object, caller must pass to 'FreeArray' to release
	PCSTR *GetArray(void) const;
	// Release memory allocated by 'GetArray'
	static void FreeArray(PVOID szArr);

	// Returns the length of the buffer in characters
	size_t GetBufferLength(void) const noexcept;
	// Returns the size of the buffer in bytes
	size_t GetBufferSize(void) const noexcept;

private:
	PSTR m_szBuffer;
	size_t m_nBufLen;
	size_t m_nCount;
};


//#################################################################################################
class CDoubleNullStrW final
{
public:
	// Constructs an empty string object
	CDoubleNullStrW(void);
	// Copy and move constructors
	CDoubleNullStrW(const CDoubleNullStrW &src);
	CDoubleNullStrW(CDoubleNullStrW &&src) noexcept;
	// Constructs a string object set to the string, which can be already double null terminated
	explicit CDoubleNullStrW(PCWSTR sz, const bool bDoubleNullTerm = false);

	~CDoubleNullStrW(void);

	// Returns the length in characters (including null-terminators)
	size_t GetLength(void) const;
	// Returns the size in bytes (including null-terminators)
	size_t GetSize(void) const;

	// Returns a pointer to the raw string data
	operator PCWSTR(void) const noexcept;
	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;
	// Empties the string, freeing the memory
	void Empty(void);

	// Copy and move assignment operators
	CDoubleNullStrW &operator=(const CDoubleNullStrW &src);
	CDoubleNullStrW &operator=(CDoubleNullStrW &&src) noexcept;
	// Assigns the string object to the double null string
	CDoubleNullStrW &operator=(PCWSTR sz);

	// Assigns the string object to the double null string
	void Assign(const CDoubleNullStrW &str);
	void Assign(PCWSTR sz, const bool bDoubleNullTerm = false);

	// Appends the string object to the double null string
	CDoubleNullStrW &operator+=(const CDoubleNullStrW &str);
	CDoubleNullStrW &operator+=(PCWSTR sz);

	// Appends the string object to the double null string
	void Append(const CDoubleNullStrW &str);
	void Append(PCWSTR sz, const bool bDoubleNullTerm = false);

	friend std::wostream &operator<<(std::wostream &stream, const CDoubleNullStrW &str);

	// Returns the number of strings in the double null string object
	size_t GetCount(void) const noexcept;
	// Returns a pointer to the raw string at the given index, or null if beyond the array
	PCWSTR GetAt(const size_t nIndex) const;

	// Returns an array of strings present in the double null string object, caller must pass to 'FreeArray' to release
	PCWSTR *GetArray(void) const;
	// Release memory allocated by 'GetArray'
	static void FreeArray(PVOID pszArr);

	// Returns the length of the buffer in characters
	size_t GetBufferLength(void) const noexcept;
	// Returns the size of the buffer in bytes
	size_t GetBufferSize(void) const noexcept;

private:
	PWSTR m_szBuffer;
	size_t m_nBufLen;
	size_t m_nCount;
};


#ifdef _WIN32
using CDoubleNullStr = CDoubleNullStrW;
#else
using CDoubleNullStr = CDoubleNullStr8;
#endif

NS_END

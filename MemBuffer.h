#pragma once

NS_BEGIN

// Forward declarations
class CMemStream;

//#################################################################################################
class CMemBuffer final
{
public:
	CMemBuffer(void);
	CMemBuffer(const CMemBuffer &src);
	CMemBuffer(CMemBuffer &&src) noexcept;
	explicit CMemBuffer(const CMemStream &buf);
	explicit CMemBuffer(CMemStream &&buf) noexcept;
	explicit CMemBuffer(const size_t nBufSize);
	CMemBuffer(PCBYTE pBuf, const size_t nDataSize);
	CMemBuffer(PCBYTE pBuf, const size_t nDataSize, const size_t nBufSize);
	explicit CMemBuffer(PCSTR sz, const bool bIncludeNullTerm = true);
	explicit CMemBuffer(PCWSTR sz, const bool bIncludeNullTerm = true);
	explicit CMemBuffer(const CStr8 &str, const bool bIncludeNullTerm = true);
	explicit CMemBuffer(const CStrW &str, const bool bIncludeNullTerm = true);
	explicit CMemBuffer(const CStr16 &str, const bool bIncludeNullTerm = true);
	~CMemBuffer(void);

	CMemBuffer &Attach(PBYTE pBuf, const size_t nDataSize, const size_t nBufSize = 0);
	PBYTE Detach(void) noexcept;

	void SecureErase(void);

	CMemBuffer &operator=(const CMemBuffer &src);
	CMemBuffer &operator=(CMemBuffer &&src) noexcept;
	CMemBuffer &operator=(const CMemStream &buf);
	CMemBuffer &operator=(CMemStream &&buf) noexcept;
	bool Assign(const CMemBuffer &buf);
	bool Assign(PCBYTE pBuf, const size_t nDataSize);
	bool Assign(PCSTR sz, const bool bIncludeNullTerm = true);
	bool Assign(PCWSTR sz, const bool bIncludeNullTerm = true);
	bool Assign(const CStr8 &str, const bool bIncludeNullTerm = true);
	bool Assign(const CStrW &str, const bool bIncludeNullTerm = true);
	bool Assign(const CStr16 &str, const bool bIncludeNullTerm = true);

	CMemBuffer &operator+=(const CMemBuffer &src);
	bool Append(const CMemBuffer &buf);
	bool Append(PCBYTE pBuf, const size_t nDataSize);
	bool Append(PCSTR sz, const bool bIncludeNullTerm = true);
	bool Append(PCWSTR sz, const bool bIncludeNullTerm = true);
	bool Append(const CStr8 &str, const bool bIncludeNullTerm = true);
	bool Append(const CStrW &str, const bool bIncludeNullTerm = true);
	bool Append(const CStr16 &str, const bool bIncludeNullTerm = true);

	bool Prepend(const CMemBuffer &buf);
	bool Prepend(PCBYTE pBuf, const size_t nDataSize);
	bool Prepend(PCSTR sz, const bool bIncludeNullTerm = true);
	bool Prepend(PCWSTR sz, const bool bIncludeNullTerm = true);
	bool Prepend(const CStr8 &str, const bool bIncludeNullTerm = true);
	bool Prepend(const CStrW &str, const bool bIncludeNullTerm = true);
	bool Prepend(const CStr16 &str, const bool bIncludeNullTerm = true);

	bool operator==(const CMemBuffer &buf) const;
	bool operator!=(const CMemBuffer &buf) const;
	bool Compare(const CMemBuffer &buf) const;
	bool Compare(PCBYTE pBuf, const size_t nDataSize) const;

	operator PBYTE(void) noexcept;
	operator PCBYTE(void) const noexcept;
	BYTE GetAt(const size_t nIndex) const;
	void SetAt(const size_t nIndex, const BYTE n);

	BYTE& operator[](const size_t nIndex);
	BYTE operator[](const size_t nIndex) const;

	void Reverse(void);

	bool IsEmpty(void) const noexcept;
	void Empty(void);

	PBYTE GetBuffer(void) noexcept;
	PCBYTE GetBuffer(void) const noexcept;
	size_t GetDataSize(void) const noexcept;
	void SetDataSize(const size_t nDataSize) noexcept;
	size_t GetBufferSize(void) const noexcept;
	bool SetBufferSize(const size_t nBufSize);

private:
	PBYTE m_pBuf;
	size_t m_nDataSize;
	size_t m_nBufSize;
};

NS_END

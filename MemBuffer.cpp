#include "Base.h"
#include "MemBuffer.h"
#include "MemStream.h"

NS_BEGIN

//#################################################################################################
CMemBuffer::CMemBuffer(void)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
}

//#################################################################################################
CMemBuffer::CMemBuffer(const CMemBuffer &src)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(src.m_pBuf, src.m_nDataSize);
}

//#################################################################################################
CMemBuffer::CMemBuffer(CMemBuffer &&src) noexcept
	: m_pBuf(src.m_pBuf),
	m_nDataSize(src.m_nDataSize),
	m_nBufSize(src.m_nBufSize)
{
	src.m_pBuf = nullptr;
	src.m_nDataSize = 0;
	src.m_nBufSize = 0;
}

//#################################################################################################
CMemBuffer::CMemBuffer(const CMemStream &buf)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(buf.GetBuffer(), buf.GetDataSize());
}

//#################################################################################################
CMemBuffer::CMemBuffer(CMemStream &&buf) noexcept
	: m_pBuf(buf.GetBuffer()),
	m_nDataSize(buf.GetDataSize()),
	m_nBufSize(buf.GetBufferSize())
{
	buf.Detach();
}

//#################################################################################################
CMemBuffer::CMemBuffer(const size_t nBufSize)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	SetBufferSize(nBufSize);
}

//#################################################################################################
CMemBuffer::CMemBuffer(PCBYTE pBuf, const size_t nDataSize)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(pBuf, nDataSize);
}

//#################################################################################################
CMemBuffer::CMemBuffer(PCBYTE pBuf, const size_t nDataSize, const size_t nBufSize)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	SetBufferSize(MAX(nDataSize, nBufSize));
	Assign(pBuf, nDataSize);
}

//#################################################################################################
CMemBuffer::CMemBuffer(PCSTR sz, const bool bIncludeNullTerm)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(sz, bIncludeNullTerm);
}

//#################################################################################################
CMemBuffer::CMemBuffer(PCWSTR sz, const bool bIncludeNullTerm)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(sz, bIncludeNullTerm);
}

//#################################################################################################
CMemBuffer::CMemBuffer(const CStr8 &str, const bool bIncludeNullTerm)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(str, bIncludeNullTerm);
}

//#################################################################################################
CMemBuffer::CMemBuffer(const CStrW &str, const bool bIncludeNullTerm)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(str, bIncludeNullTerm);
}

//#################################################################################################
CMemBuffer::CMemBuffer(const CStr16 &str, const bool bIncludeNullTerm)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0)
{
	Assign(str, bIncludeNullTerm);
}

//#################################################################################################
CMemBuffer::~CMemBuffer(void)
{
	std::free(m_pBuf);
}

//#################################################################################################
CMemBuffer &CMemBuffer::Attach(PBYTE pBuf, const size_t nDataSize, const size_t nBufSize)
{
	Empty();

	if(nDataSize)
	{
		m_pBuf = pBuf;
		m_nDataSize = nDataSize;
		m_nBufSize = MAX(nBufSize, nDataSize);
	}

	return *this;
}

//#################################################################################################
PBYTE CMemBuffer::Detach(void) noexcept
{
	PBYTE pBuf = m_pBuf;
	m_pBuf = nullptr;
	m_nDataSize = m_nBufSize = 0;

	return pBuf;
}

//#################################################################################################
void CMemBuffer::SecureErase(void)
{	// Overwrite the memory with zeros, great for sensitive data like passwords which otherwise could be left in memory when the buffer is freed
	for(size_t n = 0; n < m_nBufSize; ++n)
		m_pBuf[n] = 0;
	m_nDataSize = 0;
}

//#################################################################################################
CMemBuffer &CMemBuffer::operator=(const CMemBuffer &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CMemBuffer &CMemBuffer::operator=(CMemBuffer &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufSize)
			std::free(m_pBuf);

		m_pBuf = src.m_pBuf;
		m_nDataSize = src.m_nDataSize;
		m_nBufSize = src.m_nBufSize;

		src.m_pBuf = nullptr;
		src.m_nDataSize = 0;
		src.m_nBufSize = 0;
	}

	return *this;
}

//#################################################################################################
CMemBuffer &CMemBuffer::operator=(const CMemStream &buf)
{
	Assign(buf.GetBuffer(), buf.GetDataSize());
	return *this;
}

//#################################################################################################
CMemBuffer &CMemBuffer::operator=(CMemStream &&buf) noexcept
{
	if(m_nBufSize)
		std::free(m_pBuf);

	m_pBuf = buf.GetBuffer();
	m_nDataSize = buf.GetDataSize();
	m_nBufSize = buf.GetBufferSize();

	buf.Detach();

	return *this;
}

//#################################################################################################
bool CMemBuffer::Assign(const CMemBuffer &buf)
{
	bool bResult = false;

	if(this != &buf)
		bResult = Assign(buf.m_pBuf, buf.m_nDataSize);

	return bResult;
}

//#################################################################################################
bool CMemBuffer::Assign(PCBYTE pBuf, const size_t nDataSize)
{
	bool bResult = true;

	if(m_nBufSize < nDataSize)
	{	// Existing buffer is not large enough
		PBYTE pBufTmp;
		if(m_nBufSize)
			pBufTmp = (PBYTE)std::realloc(m_pBuf, nDataSize);
		else
			pBufTmp = (PBYTE)std::malloc(nDataSize);

		if(pBufTmp)
		{
			m_pBuf = pBufTmp;
			std::memcpy(m_pBuf, pBuf, nDataSize);
			m_nDataSize = m_nBufSize = nDataSize;
		}
		else
			bResult = false;
	}
	else
	{
		if(nDataSize)
			std::memcpy(m_pBuf, pBuf, nDataSize);
		m_nDataSize = nDataSize;
	}

	return bResult;
}

//#################################################################################################
bool CMemBuffer::Assign(PCSTR sz, const bool bIncludeNullTerm)
{
	size_t nDataSize = StringGetLength(sz);
	if(bIncludeNullTerm)
		++nDataSize;

	return Assign((PCBYTE)sz, nDataSize);
}

//#################################################################################################
bool CMemBuffer::Assign(PCWSTR sz, const bool bIncludeNullTerm)
{
	size_t nDataSize = StringGetLength(sz) * sizeof(wchar_t);
	if(bIncludeNullTerm)
		nDataSize += sizeof(wchar_t);

	return Assign((PCBYTE)sz, nDataSize);
}

//#################################################################################################
bool CMemBuffer::Assign(const CStr8 &str, const bool bIncludeNullTerm)
{
	return Assign((PCBYTE)(PCSTR)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::Assign(const CStrW &str, const bool bIncludeNullTerm)
{
	return Assign((PCBYTE)(PCWSTR)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::Assign(const CStr16 &str, const bool bIncludeNullTerm)
{
	return Assign((PCBYTE)(const char16_t*)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
CMemBuffer &CMemBuffer::operator+=(const CMemBuffer &src)
{
	Append(src);
	return *this;
}

//#################################################################################################
bool CMemBuffer::Append(const CMemBuffer &buf)
{
	bool bResult = false;

	if(this != &buf)
		bResult = Append(buf.m_pBuf, buf.m_nDataSize);

	return bResult;
}

//#################################################################################################
bool CMemBuffer::Append(PCBYTE pBuf, const size_t nDataSize)
{
	bool bResult = true;

	if(m_nBufSize < m_nDataSize + nDataSize)
	{	// Existing buffer is not large enough
		PBYTE pBufTmp;
		if(m_nBufSize)
			pBufTmp = (PBYTE)std::realloc(m_pBuf, m_nDataSize + nDataSize);
		else
			pBufTmp = (PBYTE)std::malloc(m_nDataSize + nDataSize);

		if(pBufTmp)
		{
			m_pBuf = pBufTmp;
			std::memcpy(m_pBuf + m_nDataSize, pBuf, nDataSize);
			m_nBufSize = m_nDataSize + nDataSize;
			m_nDataSize += nDataSize;
		}
		else
			bResult = false;
	}
	else if(nDataSize)
	{
		std::memcpy(m_pBuf + m_nDataSize, pBuf, nDataSize);
		m_nDataSize += nDataSize;
	}

	return bResult;
}

//#################################################################################################
bool CMemBuffer::Append(PCSTR sz, const bool bIncludeNullTerm)
{
	size_t nDataSize = StringGetLength(sz);
	if(bIncludeNullTerm)
		++nDataSize;

	return Append((PCBYTE)sz, nDataSize);
}

//#################################################################################################
bool CMemBuffer::Append(PCWSTR sz, const bool bIncludeNullTerm)
{
	size_t nDataSize = StringGetLength(sz) * sizeof(wchar_t);
	if(bIncludeNullTerm)
		nDataSize += sizeof(wchar_t);

	return Append((PCBYTE)sz, nDataSize);
}

//#################################################################################################
bool CMemBuffer::Append(const CStr8 &str, const bool bIncludeNullTerm)
{
	return Append((PCBYTE)(PCSTR)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::Append(const CStrW &str, const bool bIncludeNullTerm)
{
	return Append((PCBYTE)(PCWSTR)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::Append(const CStr16 &str, const bool bIncludeNullTerm)
{
	return Append((PCBYTE)(const char16_t*)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::Prepend(const CMemBuffer &buf)
{
	bool bResult = false;

	if(this != &buf)
		bResult = Prepend(buf.m_pBuf, buf.m_nDataSize);

	return bResult;
}

//#################################################################################################
bool CMemBuffer::Prepend(PCBYTE pBuf, const size_t nDataSize)
{
	bool bResult = true;

	if(m_nBufSize < m_nDataSize + nDataSize)
	{	// Existing buffer is not large enough
		PBYTE pBufTmp;
		if(m_nBufSize)
			pBufTmp = (PBYTE)std::realloc(m_pBuf, m_nDataSize + nDataSize);
		else
			pBufTmp = (PBYTE)std::malloc(m_nDataSize + nDataSize);

		if(pBufTmp)
		{
			m_pBuf = pBufTmp;
			std::memmove(m_pBuf + nDataSize, m_pBuf, m_nDataSize);
			std::memcpy(m_pBuf, pBuf, nDataSize);
			m_nBufSize = m_nDataSize + nDataSize;
			m_nDataSize += nDataSize;
		}
		else
			bResult = false;
	}
	else if(nDataSize)
	{
		std::memmove(m_pBuf + nDataSize, m_pBuf, m_nDataSize);
		std::memcpy(m_pBuf, pBuf, nDataSize);
		m_nDataSize += nDataSize;
	}

	return bResult;
}

//#################################################################################################
bool CMemBuffer::Prepend(PCSTR sz, const bool bIncludeNullTerm)
{
	size_t nDataSize = StringGetLength(sz);
	if(bIncludeNullTerm)
		++nDataSize;

	return Prepend((PCBYTE)sz, nDataSize);
}

//#################################################################################################
bool CMemBuffer::Prepend(PCWSTR sz, const bool bIncludeNullTerm)
{
	size_t nDataSize = StringGetLength(sz) * sizeof(wchar_t);
	if(bIncludeNullTerm)
		nDataSize += sizeof(wchar_t);

	return Prepend((PCBYTE)sz, nDataSize);
}

//#################################################################################################
bool CMemBuffer::Prepend(const CStr8 &str, const bool bIncludeNullTerm)
{
	return Prepend((PCBYTE)(PCSTR)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::Prepend(const CStrW &str, const bool bIncludeNullTerm)
{
	return Prepend((PCBYTE)(PCWSTR)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::Prepend(const CStr16 &str, const bool bIncludeNullTerm)
{
	return Prepend((PCBYTE)(const char16_t*)str, str.GetSize(bIncludeNullTerm));
}

//#################################################################################################
bool CMemBuffer::operator==(const CMemBuffer &buf) const
{
	return Compare(buf.m_pBuf, buf.m_nDataSize);
}

//#################################################################################################
bool CMemBuffer::operator!=(const CMemBuffer &buf) const
{
	return !Compare(buf.m_pBuf, buf.m_nDataSize);
}

//#################################################################################################
bool CMemBuffer::Compare(const CMemBuffer &buf) const
{
	return Compare(buf.m_pBuf, buf.m_nDataSize);
}

//#################################################################################################
bool CMemBuffer::Compare(PCBYTE pBuf, const size_t nDataSize) const
{
	return (m_nDataSize == nDataSize && std::memcmp(m_pBuf, pBuf, m_nDataSize) == 0);
}

//#################################################################################################
CMemBuffer::operator PBYTE(void) noexcept
{
	return m_pBuf;
}

//#################################################################################################
CMemBuffer::operator PCBYTE(void) const noexcept
{
	return m_pBuf;
}

//#################################################################################################
BYTE CMemBuffer::GetAt(const size_t nIndex) const
{
	return (nIndex < m_nBufSize) ? m_pBuf[nIndex] : 0;
}

//#################################################################################################
void CMemBuffer::SetAt(const size_t nIndex, const BYTE n)
{
	if(nIndex < m_nBufSize)
		m_pBuf[nIndex] = n;
}

//#################################################################################################
BYTE& CMemBuffer::operator[](const size_t nIndex)
{
	Assert(nIndex < m_nBufSize);
	return m_pBuf[nIndex];
}

//#################################################################################################
BYTE CMemBuffer::operator[](const size_t nIndex) const
{
	return (nIndex < m_nBufSize) ? m_pBuf[nIndex] : 0;
}

//#################################################################################################
void CMemBuffer::Reverse(void)
{
	if(m_nDataSize > 1)
	{
		PBYTE pBegin = m_pBuf;
		PBYTE pEnd = m_pBuf + m_nDataSize - 1;
		do{
			SWAP(*pBegin, *pEnd);
		}while(++pBegin < --pEnd);
	}
}

//#################################################################################################
bool CMemBuffer::IsEmpty(void) const noexcept
{
	return (m_nDataSize == 0);
}

//#################################################################################################
void CMemBuffer::Empty(void)
{
	std::free(m_pBuf);
	m_pBuf = nullptr;
	m_nDataSize = m_nBufSize = 0;
}

//#################################################################################################
PBYTE CMemBuffer::GetBuffer(void) noexcept
{
	return m_pBuf;
}

//#################################################################################################
PCBYTE CMemBuffer::GetBuffer(void) const noexcept
{
	return m_pBuf;
}

//#################################################################################################
size_t CMemBuffer::GetDataSize(void) const noexcept
{
	return m_nDataSize;
}

//#################################################################################################
void CMemBuffer::SetDataSize(const size_t nDataSize) noexcept
{
	m_nDataSize = MIN(nDataSize, m_nBufSize);
}

//#################################################################################################
size_t CMemBuffer::GetBufferSize(void) const noexcept
{
	return m_nBufSize;
}

//#################################################################################################
bool CMemBuffer::SetBufferSize(const size_t nBufSize)
{
	bool bResult = true;

	if(nBufSize > m_nBufSize)
	{	// Existing buffer is not large enough, realloc it
		PBYTE pBuf;
		if(m_nBufSize)
			pBuf = (PBYTE)std::realloc(m_pBuf, nBufSize);
		else
			pBuf = (PBYTE)std::malloc(nBufSize);

		if(pBuf)
		{
			m_pBuf = pBuf;
			m_nBufSize = nBufSize;
		}
		else
			bResult = false;
	}

	return bResult;
}

NS_END

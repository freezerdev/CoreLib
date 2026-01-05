#include "Base.h"
#include "MemStream.h"
#include "MemBuffer.h"

NS_BEGIN

//#################################################################################################
CMemStream::CMemStream(void)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0),
	m_nReadPosition(0)
{
}

//#################################################################################################
CMemStream::CMemStream(const CMemStream &src)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0),
	m_nReadPosition(0)
{
	if(src.m_nDataSize)
	{
		m_pBuf = (PBYTE)std::malloc(src.m_nDataSize);
		if(m_pBuf)
		{
			std::memcpy(m_pBuf, src.m_pBuf, src.m_nDataSize);
			m_nDataSize = m_nBufSize = src.m_nDataSize;
			m_nReadPosition = src.m_nReadPosition;
		}
	}
}

//#################################################################################################
CMemStream::CMemStream(CMemStream &&src) noexcept
	: m_pBuf(src.m_pBuf),
	m_nDataSize(src.m_nDataSize),
	m_nBufSize(src.m_nBufSize),
	m_nReadPosition(src.m_nReadPosition)
{
	src.m_pBuf = nullptr;
	src.m_nDataSize = 0;
	src.m_nBufSize = 0;
	src.m_nReadPosition = 0;
}

//#################################################################################################
CMemStream::CMemStream(const CMemBuffer &buf)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0),
	m_nReadPosition(0)
{
	if(buf.GetDataSize())
	{
		m_pBuf = (PBYTE)std::malloc(buf.GetDataSize());
		if(m_pBuf)
		{
			std::memcpy(m_pBuf, buf.GetBuffer(), buf.GetDataSize());
			m_nDataSize = m_nBufSize = buf.GetDataSize();
		}
	}
}

//#################################################################################################
CMemStream::CMemStream(CMemBuffer &&buf) noexcept
	: m_pBuf(buf.GetBuffer()),
	m_nDataSize(buf.GetDataSize()),
	m_nBufSize(buf.GetBufferSize()),
	m_nReadPosition(0)
{
	buf.Detach();
}

//#################################################################################################
CMemStream::CMemStream(const size_t nBufSize)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0),
	m_nReadPosition(0)
{
	SetBufferSize(nBufSize);
}

//#################################################################################################
CMemStream::CMemStream(PCBYTE pBuf, const size_t nDataSize, const size_t nBufSize)
	: m_pBuf(nullptr),
	m_nDataSize(0),
	m_nBufSize(0),
	m_nReadPosition(0)
{
	SetBufferSize(MAX(nDataSize, nBufSize));
	if(m_pBuf)
		std::memcpy(m_pBuf, pBuf, nDataSize);
	m_nDataSize = nDataSize;
}

//#################################################################################################
CMemStream::~CMemStream(void)
{
	std::free(m_pBuf);
}

//#################################################################################################
CMemStream &CMemStream::Attach(PBYTE pBuf, const size_t nDataSize, const size_t nBufSize)
{
	Empty();

	if(nDataSize)
	{
		m_pBuf = pBuf;
		m_nDataSize = nDataSize;
		m_nBufSize = MAX(nBufSize, nDataSize);
		m_nReadPosition = 0;
	}

	return *this;
}

//#################################################################################################
PBYTE CMemStream::Detach(void) noexcept
{
	PBYTE pBuf = m_pBuf;
	m_pBuf = nullptr;
	m_nDataSize = m_nBufSize = m_nReadPosition = 0;

	return pBuf;
}

//#################################################################################################
CMemStream &CMemStream::operator=(const CMemStream &src)
{
	if(this != &src)
	{
		if(m_nBufSize < src.m_nDataSize)
		{	// Existing buffer is not large enough
			PBYTE pBufTmp;
			if(m_nBufSize)
				pBufTmp = (PBYTE)std::realloc(m_pBuf, src.m_nDataSize);
			else
				pBufTmp = (PBYTE)std::malloc(src.m_nDataSize);

			if(pBufTmp)
			{
				m_pBuf = pBufTmp;
				std::memcpy(m_pBuf, src.m_pBuf, src.m_nDataSize);
				m_nDataSize = m_nBufSize = src.m_nDataSize;
			}
		}
		else
		{
			if(src.m_nDataSize)
				std::memcpy(m_pBuf, src.m_pBuf, src.m_nDataSize);
			m_nDataSize = src.m_nDataSize;
		}

		m_nReadPosition = src.m_nReadPosition;
	}

	return *this;
}

//#################################################################################################
CMemStream &CMemStream::operator=(CMemStream &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufSize)
			std::free(m_pBuf);

		m_pBuf = src.m_pBuf;
		m_nDataSize = src.m_nDataSize;
		m_nBufSize = src.m_nBufSize;
		m_nReadPosition = src.m_nReadPosition;

		src.m_pBuf = nullptr;
		src.m_nDataSize = 0;
		src.m_nBufSize = 0;
		src.m_nReadPosition = 0;
	}

	return *this;
}

//#################################################################################################
CMemStream &CMemStream::operator=(const CMemBuffer &buf)
{
	if(m_nBufSize < buf.GetDataSize())
	{	// Existing buffer is not large enough
		PBYTE pBufTmp;
		if(m_nBufSize)
			pBufTmp = (PBYTE)std::realloc(m_pBuf, buf.GetDataSize());
		else
			pBufTmp = (PBYTE)std::malloc(buf.GetDataSize());

		if(pBufTmp)
		{
			m_pBuf = pBufTmp;
			std::memcpy(m_pBuf, buf.GetBuffer(), buf.GetDataSize());
			m_nDataSize = m_nBufSize = buf.GetDataSize();
		}
	}
	else
	{
		if(buf.GetDataSize())
			std::memcpy(m_pBuf, buf.GetBuffer(), buf.GetDataSize());
		m_nDataSize = buf.GetDataSize();
	}

	m_nReadPosition = 0;

	return *this;
}

//#################################################################################################
CMemStream &CMemStream::operator=(CMemBuffer &&buf) noexcept
{
	if(m_nBufSize)
		std::free(m_pBuf);

	m_pBuf = buf.GetBuffer();
	m_nDataSize = buf.GetDataSize();
	m_nBufSize = buf.GetBufferSize();
	m_nReadPosition = 0;

	buf.Detach();

	return *this;
}

//#################################################################################################
bool CMemStream::operator==(const CMemStream &buf) const
{
	return Compare(buf.m_pBuf, buf.m_nDataSize);
}

//#################################################################################################
bool CMemStream::operator!=(const CMemStream &buf) const
{
	return !Compare(buf.m_pBuf, buf.m_nDataSize);
}

//#################################################################################################
bool CMemStream::Compare(const CMemStream &buf) const
{
	return Compare(buf.m_pBuf, buf.m_nDataSize);
}

//#################################################################################################
bool CMemStream::Compare(PCBYTE pBuf, const size_t nDataSize) const
{
	return (m_nDataSize == nDataSize && std::memcmp(m_pBuf, pBuf, m_nDataSize) == 0);
}

//#################################################################################################
CMemStream::operator PBYTE(void) noexcept
{
	return m_pBuf;
}

//#################################################################################################
CMemStream::operator PCBYTE(void) const noexcept
{
	return m_pBuf;
}

//#################################################################################################
bool CMemStream::Read(CStr8 &strData, const size_t nSize, const bool bAdvanceStream)
{
	bool bResult = false;

	strData.Empty();

	if(m_nReadPosition + nSize <= m_nDataSize)
	{
		strData.Assign((PCSTR)(m_pBuf + m_nReadPosition), 0, nSize);
		if(bAdvanceStream)
			m_nReadPosition += nSize;
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool CMemStream::Read(CStrW &strData, const size_t nSize, const bool bAdvanceStream)
{
	bool bResult = false;

	strData.Empty();

	if(m_nReadPosition + (nSize / sizeof(wchar_t)) <= m_nDataSize)
	{
		strData.Assign((PCWSTR)(m_pBuf + m_nReadPosition), 0, nSize / sizeof(wchar_t));
		if(bAdvanceStream)
			m_nReadPosition += nSize;
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool CMemStream::Read(CStr16 &strData, const size_t nSize, const bool bAdvanceStream)
{
	bool bResult = false;

	strData.Empty();

	if(m_nReadPosition + (nSize / sizeof(char16_t)) <= m_nDataSize)
	{
		strData.Assign((const char16_t*)(m_pBuf + m_nReadPosition), 0, nSize / sizeof(char16_t));
		if(bAdvanceStream)
			m_nReadPosition += nSize;
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool CMemStream::Read(CMemBuffer &bufData, const size_t nSize, const bool bAdvanceStream)
{
	bool bResult = false;

	bufData.Empty();

	if(m_nReadPosition + nSize <= m_nDataSize)
	{
		bufData.Assign(m_pBuf + m_nReadPosition, nSize);
		if(bAdvanceStream)
			m_nReadPosition += nSize;
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool CMemStream::Read(PBYTE pBuf, const size_t nSize, const bool bAdvanceStream)
{
	Assert(pBuf);

	bool bResult = false;

	SetMemoryZero(pBuf, nSize);

	if(m_nReadPosition + nSize <= m_nDataSize)
	{
		std::memcpy(pBuf, m_pBuf + m_nReadPosition, nSize);
		if(bAdvanceStream)
			m_nReadPosition += nSize;
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool CMemStream::Write(const CStr8 &strData, const bool bIncludeNullTerm)
{
	if(m_nDataSize + strData.GetSize(bIncludeNullTerm) > m_nBufSize)
		SetBufferSize(m_nDataSize + strData.GetSize(bIncludeNullTerm));	// Existing buffer is not large enough

	std::memcpy(m_pBuf + m_nDataSize, strData, strData.GetSize(bIncludeNullTerm));
	m_nDataSize += strData.GetSize(bIncludeNullTerm);

	return true;
}

//#################################################################################################
bool CMemStream::Write(const CStrW &strData, const bool bIncludeNullTerm)
{
	if(m_nDataSize + strData.GetSize(bIncludeNullTerm) > m_nBufSize)
		SetBufferSize(m_nDataSize + strData.GetSize(bIncludeNullTerm));	// Existing buffer is not large enough

	std::memcpy(m_pBuf + m_nDataSize, strData, strData.GetSize(bIncludeNullTerm));
	m_nDataSize += strData.GetSize(bIncludeNullTerm);

	return true;
}

//#################################################################################################
bool CMemStream::Write(const CStr16 &strData, const bool bIncludeNullTerm)
{
	if(m_nDataSize + strData.GetSize(bIncludeNullTerm) > m_nBufSize)
		SetBufferSize(m_nDataSize + strData.GetSize(bIncludeNullTerm));	// Existing buffer is not large enough

	std::memcpy(m_pBuf + m_nDataSize, strData, strData.GetSize(bIncludeNullTerm));
	m_nDataSize += strData.GetSize(bIncludeNullTerm);

	return true;
}

//#################################################################################################
bool CMemStream::Write(const CMemBuffer &bufData)
{
	if(m_nDataSize + bufData.GetDataSize() > m_nBufSize)
		SetBufferSize(m_nDataSize + bufData.GetDataSize());	// Existing buffer is not large enough

	std::memcpy(m_pBuf + m_nDataSize, bufData, bufData.GetDataSize());
	m_nDataSize += bufData.GetDataSize();

	return true;
}

//#################################################################################################
bool CMemStream::Write(PCBYTE pBuf, const size_t nSize)
{
	if(m_nDataSize + nSize > m_nBufSize)
		SetBufferSize(m_nDataSize + nSize);	// Existing buffer is not large enough

	std::memcpy(m_pBuf + m_nDataSize, pBuf, nSize);
	m_nDataSize += nSize;

	return true;
}

//#################################################################################################
bool CMemStream::IsEmpty(void) const noexcept
{
	return (m_nDataSize == 0);
}

//#################################################################################################
void CMemStream::Empty(void)
{
	std::free(m_pBuf);
	m_pBuf = nullptr;
	m_nDataSize = m_nBufSize = m_nReadPosition = 0;
}

//#################################################################################################
void CMemStream::ResetReadPosition(void) noexcept
{
	m_nReadPosition = 0;
}

//#################################################################################################
PBYTE CMemStream::GetBuffer(void) noexcept
{
	return m_pBuf;
}

//#################################################################################################
PCBYTE CMemStream::GetBuffer(void) const noexcept
{
	return m_pBuf;
}

//#################################################################################################
size_t CMemStream::GetDataSize(void) const noexcept
{
	return m_nDataSize;
}

//#################################################################################################
void CMemStream::SetDataSize(const size_t nDataSize) noexcept
{
	m_nDataSize = MIN(nDataSize, m_nBufSize);
}

//#################################################################################################
size_t CMemStream::GetBufferSize(void) const noexcept
{
	return m_nBufSize;
}

//#################################################################################################
bool CMemStream::SetBufferSize(const size_t nBufSize)
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

//#################################################################################################
size_t CMemStream::GetReadPosition(void) const noexcept
{
	return m_nReadPosition;
}

//#################################################################################################
void CMemStream::SetReadPosition(const size_t nReadPosition) noexcept
{
	m_nReadPosition = MIN(nReadPosition, m_nDataSize);
}

NS_END

#include "Base.h"
#include "Container.h"

NS_BEGIN

//#################################################################################################
CContainer::CContainer(void)
	: m_nReadPosition(0)
{
}

//#################################################################################################
CContainer::CContainer(PCBYTE pBuf, const size_t nDataSize)
	: m_buffer(pBuf, nDataSize),
	m_nReadPosition(0)
{
}

//#################################################################################################
CContainer::CContainer(CContainer &&src) noexcept
	: m_buffer(std::move(src.m_buffer)),
	m_nReadPosition(src.m_nReadPosition)
{
	src.m_nReadPosition = 0;
}

//#################################################################################################
CContainer &CContainer::operator=(CContainer &&src) noexcept
{
	if(this != &src)
	{
		m_buffer = std::move(src.m_buffer);
		m_nReadPosition = src.m_nReadPosition;

		src.m_nReadPosition = 0;
	}

	return *this;
}

//#################################################################################################
bool CContainer::IsEmpty(void) const noexcept
{
	return m_buffer.IsEmpty();
}

//#################################################################################################
void CContainer::Empty(void)
{
	m_buffer.Empty();
	m_nReadPosition = 0;
}

//#################################################################################################
bool CContainer::HasMoreData(void) const noexcept
{
	return (m_nReadPosition < m_buffer.GetDataSize());
}

//#################################################################################################
bool CContainer::operator==(const CContainer &buf) const
{
	return (m_buffer == buf.m_buffer);
}

//#################################################################################################
bool CContainer::operator!=(const CContainer &buf) const
{
	return (m_buffer != buf.m_buffer);
}

//#################################################################################################
bool CContainer::GetValueBool(void) const
{
	bool bValue = false;

	if(m_nReadPosition + sizeof(bValue) <= m_buffer.GetDataSize())
	{
		std::memcpy(&bValue, m_buffer.GetBuffer() + m_nReadPosition, sizeof(bValue));
		m_nReadPosition += sizeof(bValue);
	}

	return bValue;
}

//#################################################################################################
int64_t CContainer::GetValueInt(void) const
{
	int64_t nValue = 0;

	if(m_nReadPosition + sizeof(nValue) <= m_buffer.GetDataSize())
	{
		std::memcpy(&nValue, m_buffer.GetBuffer() + m_nReadPosition, sizeof(nValue));
		m_nReadPosition += sizeof(nValue);
	}

	return nValue;
}

//#################################################################################################
uint64_t CContainer::GetValueUInt(void) const
{
	uint64_t nValue = 0;

	if(m_nReadPosition + sizeof(nValue) <= m_buffer.GetDataSize())
	{
		std::memcpy(&nValue, m_buffer.GetBuffer() + m_nReadPosition, sizeof(nValue));
		m_nReadPosition += sizeof(nValue);
	}

	return nValue;
}

//#################################################################################################
double CContainer::GetValueDouble(void) const
{
	double dValue = 0.0;

	if(m_nReadPosition + sizeof(dValue) <= m_buffer.GetDataSize())
	{
		std::memcpy(&dValue, m_buffer.GetBuffer() + m_nReadPosition, sizeof(dValue));
		m_nReadPosition += sizeof(dValue);
	}

	return dValue;
}

//#################################################################################################
CStr8 CContainer::GetValueString8(void) const
{
	CStr8 strValue;

	if(m_nReadPosition + sizeof(uint32_t) <= m_buffer.GetDataSize())
	{
		uint32_t nSize;
		std::memcpy(&nSize, m_buffer.GetBuffer() + m_nReadPosition, sizeof(nSize));
		m_nReadPosition += sizeof(nSize);

		Assert(m_nReadPosition + nSize <= m_buffer.GetDataSize());
		strValue.Assign((PCSTR)(m_buffer.GetBuffer() + m_nReadPosition), 0, nSize);
		m_nReadPosition += nSize;
	}

	return strValue;
}

//#################################################################################################
CStrW CContainer::GetValueStringW(void) const
{
	CStrW strValue;

	if(m_nReadPosition + sizeof(uint32_t) <= m_buffer.GetDataSize())
	{
		uint32_t nSize;
		std::memcpy(&nSize, m_buffer.GetBuffer() + m_nReadPosition, sizeof(nSize));
		m_nReadPosition += sizeof(nSize);

		Assert(m_nReadPosition + nSize <= m_buffer.GetDataSize());
		strValue.Assign((PCWSTR)(m_buffer.GetBuffer() + m_nReadPosition), 0, nSize / sizeof(wchar_t));
		m_nReadPosition += nSize;
	}

	return strValue;
}

//#################################################################################################
CStr CContainer::GetValueString(void) const
{
	CStr strValue;

	if(m_nReadPosition + sizeof(uint32_t) <= m_buffer.GetDataSize())
	{
		uint32_t nSize;
		std::memcpy(&nSize, m_buffer.GetBuffer() + m_nReadPosition, sizeof(nSize));
		m_nReadPosition += sizeof(nSize);

		Assert(m_nReadPosition + nSize <= m_buffer.GetDataSize());
		strValue.Assign((PCNSTR)(m_buffer.GetBuffer() + m_nReadPosition), 0, nSize / sizeof(NCHAR));
		m_nReadPosition += nSize;
	}

	return strValue;
}

//#################################################################################################
CMemBuffer CContainer::GetValueBuffer(void) const
{
	CMemBuffer bufValue;

	if(m_nReadPosition + sizeof(uint32_t) <= m_buffer.GetDataSize())
	{
		uint32_t nSize;
		std::memcpy(&nSize, m_buffer.GetBuffer() + m_nReadPosition, sizeof(nSize));
		m_nReadPosition += sizeof(nSize);

		Assert(m_nReadPosition + nSize <= m_buffer.GetDataSize());
		bufValue.Assign(m_buffer.GetBuffer() + m_nReadPosition, nSize);
		m_nReadPosition += nSize;
	}

	return bufValue;
}

//#################################################################################################
void CContainer::SetValueBool(const bool bValue)
{
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(bValue));
	m_buffer.Append((PCBYTE)&bValue, sizeof(bValue));
}

//#################################################################################################
void CContainer::SetValueInt(const int64_t nValue)
{
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(nValue));
	m_buffer.Append((PCBYTE)&nValue, sizeof(nValue));
}

//#################################################################################################
void CContainer::SetValueUInt(const uint64_t nValue)
{
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(nValue));
	m_buffer.Append((PCBYTE)&nValue, sizeof(nValue));
}

//#################################################################################################
void CContainer::SetValueDouble(const double dValue)
{
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(dValue));
	m_buffer.Append((PCBYTE)&dValue, sizeof(dValue));
}

//#################################################################################################
void CContainer::SetValueString8(const CStr8 &strValue)
{
	uint32_t nSize = (uint32_t)strValue.GetSize(true);
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(nSize) + nSize);
	m_buffer.Append((PCBYTE)&nSize, sizeof(nSize));
	m_buffer.Append(strValue);
}

//#################################################################################################
void CContainer::SetValueStringW(const CStrW &strValue)
{
	uint32_t nSize = (uint32_t)strValue.GetSize(true);
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(nSize) + nSize);
	m_buffer.Append((PCBYTE)&nSize, sizeof(nSize));
	m_buffer.Append(strValue);
}

//#################################################################################################
void CContainer::SetValueString(const CStr &strValue)
{
	uint32_t nSize = (uint32_t)strValue.GetSize(true);
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(nSize) + nSize);
	m_buffer.Append((PCBYTE)&nSize, sizeof(nSize));
	m_buffer.Append(strValue);
}

//#################################################################################################
void CContainer::SetValueBuffer(const CMemBuffer &bufValue)
{
	uint32_t nSize = (uint32_t)bufValue.GetDataSize();
	m_buffer.SetBufferSize(m_buffer.GetDataSize() + sizeof(nSize) + nSize);
	m_buffer.Append((PCBYTE)&nSize, sizeof(nSize));
	m_buffer.Append(bufValue);
}

//#################################################################################################
bool CContainer::StreamIn(const CStr &strStream)
{
	Empty();
	return UUDecode(strStream, m_buffer);
}

//#################################################################################################
CStr CContainer::StreamOut(void) const
{
	return UUEncode(m_buffer);
}

//#################################################################################################
PCBYTE CContainer::GetRawBuffer(void) const noexcept
{
	return m_buffer.GetBuffer();
}

//#################################################################################################
void CContainer::SetRawBuffer(PCBYTE pBuffer, size_t nBufferSize)
{
	m_buffer.Assign(pBuffer, nBufferSize);
}

//#################################################################################################
size_t CContainer::GetSize(void) const noexcept
{
	return m_buffer.GetDataSize();
}

//#################################################################################################
void CContainer::Reserve(const size_t nSize)
{
	m_buffer.SetBufferSize(nSize);
}

//#################################################################################################
void CContainer::ResetReadPosition(void) noexcept
{
	m_nReadPosition = 0;
}

NS_END

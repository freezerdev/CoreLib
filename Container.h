#pragma once

#include "MemBuffer.h"

NS_BEGIN

//#################################################################################################
class CContainer final
{
public:
	CContainer(void);
	CContainer(PCBYTE pBuf, const size_t nDataSize);
	// Copy and move constructors
	CContainer(const CContainer &src) = default;
	CContainer(CContainer &&src) noexcept;

	// Copy and move assignment operators
	CContainer &operator=(const CContainer &src) = default;
	CContainer &operator=(CContainer &&src) noexcept;

	bool IsEmpty(void) const noexcept;
	void Empty(void);
	bool HasMoreData(void) const noexcept;

	bool operator==(const CContainer &buf) const;
	bool operator!=(const CContainer &buf) const;

	bool GetValueBool(void) const;
	int64_t GetValueInt(void) const;
	uint64_t GetValueUInt(void) const;
	double GetValueDouble(void) const;
	CStr8 GetValueString8(void) const;
	CStrW GetValueStringW(void) const;
	CStr GetValueString(void) const;
	CMemBuffer GetValueBuffer(void) const;

	void SetValueBool(const bool bValue);
	void SetValueInt(const int64_t nValue);
	void SetValueUInt(const uint64_t nValue);
	void SetValueDouble(const double dValue);
	void SetValueString8(const CStr8 &strValue);
	void SetValueStringW(const CStrW &strValue);
	void SetValueString(const CStr &strValue);
	void SetValueBuffer(const CMemBuffer &bufValue);

	bool StreamIn(const CStr &strStream);
	CStr StreamOut(void) const;

	PCBYTE GetRawBuffer(void) const noexcept;
	void SetRawBuffer(PCBYTE pBuffer, size_t nBufferSize);
	size_t GetSize(void) const noexcept;
	void Reserve(const size_t nSize);
	void ResetReadPosition(void) noexcept;

private:
	CMemBuffer m_buffer;
	mutable size_t m_nReadPosition;
};

NS_END

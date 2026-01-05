#pragma once

NS_BEGIN

// Forward declarations
class CMemBuffer;

//#################################################################################################
class CMemStream final
{
public:
	CMemStream(void);
	CMemStream(const CMemStream &src);
	CMemStream(CMemStream &&src) noexcept;
	explicit CMemStream(const CMemBuffer &buf);
	explicit CMemStream(CMemBuffer &&buf) noexcept;
	explicit CMemStream(const size_t nBufSize);
	CMemStream(PCBYTE pBuf, const size_t nDataSize, const size_t nBufSize = 0);
	~CMemStream(void);

	CMemStream &Attach(PBYTE pBuf, const size_t nDataSize, const size_t nBufSize = 0);
	PBYTE Detach(void) noexcept;

	CMemStream &operator=(const CMemStream &src);
	CMemStream &operator=(CMemStream &&src) noexcept;
	CMemStream &operator=(const CMemBuffer &buf);
	CMemStream &operator=(CMemBuffer &&buf) noexcept;

	bool operator==(const CMemStream &buf) const;
	bool operator!=(const CMemStream &buf) const;
	bool Compare(const CMemStream &buf) const;
	bool Compare(PCBYTE pBuf, const size_t nDataSize) const;

	operator PBYTE(void) noexcept;
	operator PCBYTE(void) const noexcept;

	//#################################################################################################
	template<typename DATATYPE>
	bool Read(DATATYPE &data, const bool bAdvanceStream = true)
	{	// Read simple data types from the stream
		bool bResult = false;

		data = 0;

		if(m_nReadPosition + sizeof(data) <= m_nDataSize)
		{
			std::memcpy(&data, m_pBuf + m_nReadPosition, sizeof(data));
			if(bAdvanceStream)
				m_nReadPosition += sizeof(data);
			bResult = true;
		}

		return bResult;
	}

	// Read data from the stream
	bool Read(CStr8 &strData, const size_t nSize, const bool bAdvanceStream = true);
	bool Read(CStrW &strData, const size_t nSize, const bool bAdvanceStream = true);
	bool Read(CStr16 &strData, const size_t nSize, const bool bAdvanceStream = true);
	bool Read(CMemBuffer &bufData, const size_t nSize, const bool bAdvanceStream = true);
	bool Read(PBYTE pBuf, const size_t nSize, const bool bAdvanceStream = true);

	//#################################################################################################
	template<typename DATATYPE>
	bool Write(const DATATYPE data)
	{	// Write simple data types to the stream
		if(m_nDataSize + sizeof(data) > m_nBufSize)
			SetBufferSize(m_nDataSize + sizeof(data));	// Existing buffer is not large enough

		std::memcpy(m_pBuf + m_nDataSize, &data, sizeof(data));
		m_nDataSize += sizeof(data);

		return true;
	}

	// Write data from the stream
	bool Write(const CStr8 &strData, const bool bIncludeNullTerm = true);
	bool Write(const CStrW &strData, const bool bIncludeNullTerm = true);
	bool Write(const CStr16 &strData, const bool bIncludeNullTerm = true);
	bool Write(const CMemBuffer &bufData);
	bool Write(PCBYTE pBuf, const size_t nSize);

	bool IsEmpty(void) const noexcept;
	void Empty(void);
	void ResetReadPosition(void) noexcept;

	PBYTE GetBuffer(void) noexcept;
	PCBYTE GetBuffer(void) const noexcept;
	size_t GetDataSize(void) const noexcept;
	void SetDataSize(const size_t nDataSize) noexcept;
	size_t GetBufferSize(void) const noexcept;
	bool SetBufferSize(const size_t nBufSize);
	size_t GetReadPosition(void) const noexcept;
	void SetReadPosition(const size_t nReadPosition) noexcept;

private:
	PBYTE m_pBuf;
	size_t m_nDataSize;
	size_t m_nBufSize;
	size_t m_nReadPosition;
};

NS_END

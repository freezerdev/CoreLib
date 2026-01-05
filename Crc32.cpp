#include "Base.h"
#include "Crc32.h"
#include "FileSystemUtils.h"

NS_BEGIN

static constexpr auto CRC_BUFFER_SIZE = 8192;

//#################################################################################################
CCrc32::CCrc32(const uint32_t nPolynomial)
{	// The default polynomial of "0xEDB88320" is the official one used by CRC32 in PKZip.
	// Often times the polynomial shown reversed as 0x04C11DB7.
	m_pnCrc32Table = std::make_unique<uint32_t[]>(256);
	for(uint16_t i = 0; i < 256; ++i)
	{
		uint32_t nCrc = i;
		for(uint8_t j = 0; j < 8; ++j)
		{
			if(nCrc & 1)
				nCrc = (nCrc >> 1) ^ nPolynomial;
			else
				nCrc >>= 1;
		}

		m_pnCrc32Table[i] = nCrc;
	}
}

//#################################################################################################
bool CCrc32::Crc32File(PCNSTR szFilename, uint32_t &nCrc32) const
{
	bool bResult = false;

	nCrc32 = (uint32_t)-1;

	if(szFilename == nullptr)
		return bResult;

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(szFilename, EFM_ExistingReadOnly, hFile) == FW_NO_ERROR)
	{
		auto pBuffer = std::make_unique<BYTE[]>(CRC_BUFFER_SIZE);
		size_t nBytesRead;
		bool bSuccess = (FileRead(hFile, pBuffer.get(), CRC_BUFFER_SIZE, nBytesRead) == FW_NO_ERROR);
		while(bSuccess && nBytesRead)
		{
			for(size_t n = 0; n < nBytesRead; ++n)
				nCrc32 = (nCrc32 >> 8) ^ m_pnCrc32Table[pBuffer[n] ^ (nCrc32 & 0x000000FF)];

			bSuccess = (FileRead(hFile, pBuffer.get(), CRC_BUFFER_SIZE, nBytesRead) == FW_NO_ERROR);
		}

		nCrc32 = ~nCrc32;
		bResult = true;

		FileClose(hFile);
	}

	return bResult;
}

//#################################################################################################
bool CCrc32::Crc32Buffer(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const
{
	if(pBuffer == nullptr)
		return false;

	nCrc32 = (uint32_t)-1;
	for(size_t n = 0; n < nSize; ++n)
		nCrc32 = (nCrc32 >> 8) ^ m_pnCrc32Table[pBuffer[n] ^ (nCrc32 & 0x000000FF)];
	nCrc32 = ~nCrc32;

	return true;
}

//#################################################################################################
bool CCrc32::Crc32StreamInit(uint32_t &nCrc32) const noexcept
{
	nCrc32 = (uint32_t)-1;
	return true;
}

//#################################################################################################
void CCrc32::Crc32Stream(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const noexcept
{
	for(size_t n = 0; n < nSize; ++n)
		nCrc32 = (nCrc32 >> 8) ^ m_pnCrc32Table[pBuffer[n] ^ (nCrc32 & 0x000000FF)];
}

//#################################################################################################
void CCrc32::Crc32StreamEnd(uint32_t &nCrc32) noexcept
{
	nCrc32 = ~nCrc32;
}




//#################################################################################################
CCrc32Mpeg2::CCrc32Mpeg2(void)
{	// "0xEDB88320" is the official polynomial used by CRC32 in PKZip.
	// Often times the polynomial shown reversed as 0x04C11DB7.
	m_pnCrc32Table = std::make_unique<uint32_t[]>(256);
	for(uint16_t i = 0; i < 256; ++i)
	{
		uint32_t nCrc = i;
		nCrc <<= 24;
		for(uint8_t j = 8; j > 0; --j)
		{
			if(nCrc & 0x80000000)
				nCrc = (nCrc << 1) ^ 0x04C11DB7;
			else
				nCrc <<= 1;
		}

		m_pnCrc32Table[i] = nCrc;
	}
}

//#################################################################################################
bool CCrc32Mpeg2::Crc32File(PCNSTR szFilename, uint32_t &nCrc32) const
{
	bool bResult = false;

	nCrc32 = (uint32_t)-1;

	NHANDLE hFile = INVALID_NHANDLE;
	if(FileCreate(szFilename, EFM_ExistingReadOnly, hFile) == FW_NO_ERROR)
	{
		auto pBuffer = std::make_unique<BYTE[]>(CRC_BUFFER_SIZE);
		size_t nBytesRead;
		bool bSuccess = (FileRead(hFile, pBuffer.get(), CRC_BUFFER_SIZE, nBytesRead) == FW_NO_ERROR);
		while(bSuccess && nBytesRead)
		{
			for(size_t n = 0; n < nBytesRead; ++n)
				nCrc32 = m_pnCrc32Table[(nCrc32 >> 24) ^ (pBuffer[n] & 0xFF)] ^ (nCrc32 << 8);

			bSuccess = (FileRead(hFile, pBuffer.get(), CRC_BUFFER_SIZE, nBytesRead) == FW_NO_ERROR);
		}

		bResult = true;

		FileClose(hFile);
	}

	return bResult;
}

//#################################################################################################
bool CCrc32Mpeg2::Crc32Buffer(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const
{
	nCrc32 = (uint32_t)-1;
	for(size_t n = 0; n < nSize; ++n)
		nCrc32 = m_pnCrc32Table[(nCrc32 >> 24) ^ (pBuffer[n] & 0xFF)] ^ (nCrc32 << 8);

	return true;
}

//#################################################################################################
bool CCrc32Mpeg2::Crc32StreamInit(uint32_t &nCrc32) noexcept
{
	nCrc32 = (uint32_t)-1;
	return true;
}

//#################################################################################################
void CCrc32Mpeg2::Crc32Stream(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const noexcept
{
	for(size_t n = 0; n < nSize; ++n)
		nCrc32 = m_pnCrc32Table[(nCrc32 >> 24) ^ (pBuffer[n] & 0xFF)] ^ (nCrc32 << 8);
}

NS_END

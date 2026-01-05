#pragma once

NS_BEGIN

//#################################################################################################
class CCrc32 final
{
public:
	explicit CCrc32(const uint32_t nPolynomial = 0xEDB88320);

	bool Crc32File(PCNSTR szFilename, uint32_t &nCrc32) const;

	bool Crc32Buffer(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const;

	bool Crc32StreamInit(uint32_t &nCrc32) const noexcept;
	void Crc32Stream(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const noexcept;
	static void Crc32StreamEnd(uint32_t &nCrc32) noexcept;

private:
	std::unique_ptr<uint32_t[]> m_pnCrc32Table;
};


//#################################################################################################
class CCrc32Mpeg2 final
{
public:
	CCrc32Mpeg2(void);

	bool Crc32File(PCNSTR szFilename, uint32_t &nCrc32) const;

	bool Crc32Buffer(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const;

	static bool Crc32StreamInit(uint32_t &nCrc32) noexcept;
	void Crc32Stream(PCBYTE pBuffer, const size_t nSize, uint32_t &nCrc32) const noexcept;

private:
	std::unique_ptr<uint32_t[]> m_pnCrc32Table;
};

NS_END

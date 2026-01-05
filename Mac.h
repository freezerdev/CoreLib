#pragma once

NS_BEGIN

//#################################################################################################
class CMac final
{
public:
	CMac(void);
	CMac(const CMac &src);
	CMac(CMac &&src) noexcept;
	explicit CMac(PCBYTE pMac);
	explicit CMac(PCNSTR szMac, const NCHAR chDelim = _N(':'));
	explicit CMac(const uint64_t nMac);

	bool IsValid(void) const noexcept;		// An invalid MAC is all zeros or all 0xFF
	bool IsEmpty(void) const noexcept;
	void Empty(void);

	void Set(PCBYTE pMac);
	void Set(PCNSTR szMac, const NCHAR chDelim = _N(':'));
	void Set(const uint64_t nMac);

	PCBYTE Get(void) const noexcept;
	CStr GetAsString(const NCHAR chDelim = _N(':')) const;
	uint64_t GetAsUInt(void) const noexcept;

	CMac &operator=(const CMac &src);
	CMac &operator=(CMac &&src) noexcept;
	void Assign(const CMac &src);
	void Assign(PCNSTR szMac, const NCHAR chDelim = _N(':'));
	void Assign(const uint64_t nMac);

	friend std::ostream &operator<<(std::ostream &stream, const CMac &mac);
	friend std::wostream &operator<<(std::wostream &stream, const CMac &mac);

	bool operator==(const CMac &mac) const;
	bool operator!=(const CMac &mac) const;
	int Compare(const CMac &mac) const;

private:
	BYTE m_mac[6];
};

NS_END

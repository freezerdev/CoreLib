#pragma once

NS_BEGIN

//#################################################################################################
class CIPv6 final
{
public:
	CIPv6(void);
	CIPv6(const CIPv6 &src);
	CIPv6(CIPv6 &&src) noexcept;
	explicit CIPv6(PCBYTE pIP);
	explicit CIPv6(PCNSTR szIP);

	bool IsEmpty(void) const noexcept;
	void Empty(void);

	void Set(PCBYTE pIP);
	void Set(PCNSTR szIP);

	PCBYTE Get(void) const noexcept;
	CStr GetAsString(void) const;

	CIPv6 &operator=(const CIPv6 &src);
	CIPv6 &operator=(CIPv6 &&src) noexcept;
	void Assign(const CIPv6 &src);
	void Assign(PCNSTR szIP);

	friend std::ostream &operator<<(std::ostream &stream, const CIPv6 &ip);
	friend std::wostream &operator<<(std::wostream &stream, const CIPv6 &ip);

	bool operator==(const CIPv6 &ip) const;
	bool operator!=(const CIPv6 &ip) const;
	int Compare(const CIPv6 &ip) const;

private:
	BYTE m_ip[16];
};

NS_END

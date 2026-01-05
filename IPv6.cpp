#include "Base.h"
#include "IPv6.h"

NS_BEGIN

//#################################################################################################
CIPv6::CIPv6(void)
{
	SetMemoryZero(m_ip, 16);
}

//#################################################################################################
CIPv6::CIPv6(const CIPv6 &src)
{
	SetMemoryZero(m_ip, 16);
	Assign(src);
}

//#################################################################################################
CIPv6::CIPv6(CIPv6 &&src) noexcept
{
	std::memcpy(m_ip, src.m_ip, 16);
	SetMemoryZero(src.m_ip, 16);
}

//#################################################################################################
CIPv6::CIPv6(PCBYTE pIP)
{
	SetMemoryZero(m_ip, 16);
	Set(pIP);
}

//#################################################################################################
CIPv6::CIPv6(PCNSTR szIP)
{
	SetMemoryZero(m_ip, 16);
	Set(szIP);
}

//#################################################################################################
bool CIPv6::IsEmpty(void) const noexcept
{
	return (m_ip[0] == 0 && m_ip[1] == 0 && m_ip[2] == 0 && m_ip[3] == 0 && m_ip[4] == 0 && m_ip[5] == 0 && m_ip[6] == 0 && m_ip[7] == 0 &&
		m_ip[8] == 0 && m_ip[9] == 0 && m_ip[10] == 0 && m_ip[11] == 0 && m_ip[12] == 0 && m_ip[13] == 0 && m_ip[14] == 0 && m_ip[15] == 0);
}

//#################################################################################################
void CIPv6::Empty(void)
{
	SetMemoryZero(m_ip, 16);
}

//#################################################################################################
void CIPv6::Set(PCBYTE pIP)
{
	if(pIP)
		std::memcpy(m_ip, pIP, 16);
}

//#################################################################################################
void CIPv6::Set(PCNSTR szIP)
{
	CStr strIP(szIP);
	strIP.Remove(_N(':'));
	if(strIP.GetLength() == 32 && IsHexString(strIP))
	{
		PBYTE pScan = m_ip;
		NCHAR sz[3] = {0};
		for(size_t n = 0; n < 32; n += 2)
		{
			sz[0] = strIP.GetAt(n);
			sz[1] = strIP.GetAt(n + 1);
			*pScan++ = StringToInteger<BYTE>(sz, 16);
		}
	}
	else
		SetMemoryZero(m_ip, 16);
}

//#################################################################################################
PCBYTE CIPv6::Get(void) const noexcept
{
	return m_ip;
}

//#################################################################################################
CStr CIPv6::GetAsString(void) const
{
	return CStr(CStr::EPT_Printf, _N("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X"),
		m_ip[0], m_ip[1], m_ip[2], m_ip[3], m_ip[4], m_ip[5], m_ip[6], m_ip[7],
		m_ip[8], m_ip[9], m_ip[10], m_ip[11], m_ip[12], m_ip[13], m_ip[14], m_ip[15]);
}

//#################################################################################################
CIPv6 &CIPv6::operator=(const CIPv6 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CIPv6 &CIPv6::operator=(CIPv6 &&src) noexcept
{
	if(this != &src)
	{
		std::memcpy(m_ip, src.m_ip, 16);
		SetMemoryZero(src.m_ip, 16);
	}

	return *this;
}

//#################################################################################################
void CIPv6::Assign(const CIPv6 &src)
{
	if(this != &src)
		std::memcpy(m_ip, src.m_ip, 16);
}

//#################################################################################################
void CIPv6::Assign(PCNSTR szIP)
{
	Set(szIP);
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CIPv6 &ip)
{
	CStr8 strIP(CStr8::EPT_Printf, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
		ip.m_ip[0], ip.m_ip[1], ip.m_ip[2], ip.m_ip[3], ip.m_ip[4], ip.m_ip[5], ip.m_ip[6], ip.m_ip[7],
		ip.m_ip[8], ip.m_ip[9], ip.m_ip[10], ip.m_ip[11], ip.m_ip[12], ip.m_ip[13], ip.m_ip[14], ip.m_ip[15]);
	stream << strIP;
	return stream;
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CIPv6 &ip)
{
	CStrW strIP(CStrW::EPT_Printf, L"%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
		ip.m_ip[0], ip.m_ip[1], ip.m_ip[2], ip.m_ip[3], ip.m_ip[4], ip.m_ip[5], ip.m_ip[6], ip.m_ip[7],
		ip.m_ip[8], ip.m_ip[9], ip.m_ip[10], ip.m_ip[11], ip.m_ip[12], ip.m_ip[13], ip.m_ip[14], ip.m_ip[15]);
	stream << strIP;
	return stream;
}

//#################################################################################################
bool CIPv6::operator==(const CIPv6 &ip) const
{
	return (Compare(ip) == 0);
}

//#################################################################################################
bool CIPv6::operator!=(const CIPv6 &ip) const
{
	return (Compare(ip) != 0);
}

//#################################################################################################
int CIPv6::Compare(const CIPv6 &ip) const
{
	return std::memcmp(m_ip, ip.m_ip, 16);
}

NS_END

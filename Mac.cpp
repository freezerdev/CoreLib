#include "Base.h"
#include "Mac.h"

NS_BEGIN

//#################################################################################################
CMac::CMac(void)
{
	SetMemoryZero(m_mac, 6);
}

//#################################################################################################
CMac::CMac(CMac &&src) noexcept
{
	std::memcpy(m_mac, src.m_mac, 6);
	SetMemoryZero(src.m_mac, 6);
}

//#################################################################################################
CMac::CMac(PCBYTE pMac)
{
	SetMemoryZero(m_mac, 6);
	Set(pMac);
}

//#################################################################################################
CMac::CMac(PCNSTR szMac, const NCHAR chDelim)
{
	SetMemoryZero(m_mac, 6);
	Set(szMac, chDelim);
}

//#################################################################################################
CMac::CMac(const uint64_t nMac)
{
	SetMemoryZero(m_mac, 6);
	Set(nMac);
}

//#################################################################################################
bool CMac::IsValid(void) const noexcept
{	// An invalid MAC is all zeros or all 0xFF
	return ((m_mac[0] != 0 || m_mac[1] != 0 || m_mac[2] != 0 || m_mac[3] != 0 || m_mac[4] != 0 || m_mac[5] != 0) &&
		(m_mac[0] != 0xFF || m_mac[1] != 0xFF || m_mac[2] != 0xFF || m_mac[3] != 0xFF || m_mac[4] != 0xFF || m_mac[5] != 0xFF));
}

//#################################################################################################
bool CMac::IsEmpty(void) const noexcept
{
	return (m_mac[0] == 0 && m_mac[1] == 0 && m_mac[2] == 0 && m_mac[3] == 0 && m_mac[4] == 0 && m_mac[5] == 0);
}

//#################################################################################################
void CMac::Empty(void)
{
	SetMemoryZero(m_mac, 6);
}

//#################################################################################################
void CMac::Set(PCBYTE pMac)
{
	if(pMac)
		std::memcpy(m_mac, pMac, 6);
}

//#################################################################################################
void CMac::Set(PCNSTR szMac, const NCHAR chDelim)
{
	CStr strMac(szMac);
	strMac.Remove(chDelim);
	if(strMac.GetLength() == 12 && IsHexString(strMac))
	{
		PBYTE pScan = m_mac;
		NCHAR sz[3] = {0};
		for(size_t n = 0; n < 12; n += 2)
		{
			sz[0] = strMac.GetAt(n);
			sz[1] = strMac.GetAt(n + 1);
			*pScan++ = StringToInteger<BYTE>(sz, 16);
		}
	}
	else
		SetMemoryZero(m_mac, 6);
}

//#################################################################################################
void CMac::Set(const uint64_t nMac)
{
	Assert((nMac >> 48) == 0);

	m_mac[0] = (BYTE)(nMac >> 40);
	m_mac[1] = (BYTE)(nMac >> 32);
	m_mac[2] = (BYTE)(nMac >> 24);
	m_mac[3] = (BYTE)(nMac >> 16);
	m_mac[4] = (BYTE)(nMac >> 8);
	m_mac[5] = (BYTE)nMac;
}

//#################################################################################################
PCBYTE CMac::Get(void) const noexcept
{
	return m_mac;
}

//#################################################################################################
CStr CMac::GetAsString(const NCHAR chDelim) const
{
	if(chDelim == g_chNull || chDelim == _N('\n') || chDelim == _N('\r'))
		return CStr(CStr::EPT_Printf, _N("%02X%02X%02X%02X%02X%02X"), m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);
	else
		return CStr(CStr::EPT_Printf, _N("%02X%c%02X%c%02X%c%02X%c%02X%c%02X"), m_mac[0], chDelim, m_mac[1], chDelim, m_mac[2], chDelim, m_mac[3], chDelim, m_mac[4], chDelim, m_mac[5]);
}

//#################################################################################################
uint64_t CMac::GetAsUInt(void) const noexcept
{
	uint64_t nMac;

	nMac = (uint64_t)(BYTE)m_mac[0] << 40;
	nMac |= (uint64_t)(BYTE)m_mac[1] << 32;
	nMac |= (uint64_t)(BYTE)m_mac[2] << 24;
	nMac |= (uint64_t)(BYTE)m_mac[3] << 16;
	nMac |= (uint64_t)(BYTE)m_mac[4] << 8;
	nMac |= (uint64_t)(BYTE)m_mac[5];

	return nMac;
}

//#################################################################################################
CMac &CMac::operator=(CMac &&src) noexcept
{
	if(this != &src)
	{
		std::memcpy(m_mac, src.m_mac, 6);
		SetMemoryZero(src.m_mac, 6);
	}

	return *this;
}

//#################################################################################################
void CMac::Assign(const CMac &src)
{
	if(this != &src)
		std::memcpy(m_mac, src.m_mac, 6);
}

//#################################################################################################
void CMac::Assign(PCNSTR szMac, const NCHAR chDelim)
{
	Set(szMac, chDelim);
}

//#################################################################################################
void CMac::Assign(const uint64_t nMac)
{
	Set(nMac);
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CMac &mac)
{
	CStr8 strMac(CStr8::EPT_Printf, "%02X:%02X:%02X:%02X:%02X:%02X", mac.m_mac[0], mac.m_mac[1], mac.m_mac[2], mac.m_mac[3], mac.m_mac[4], mac.m_mac[5]);
	stream << strMac;
	return stream;
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CMac &mac)
{
	CStrW strMac(CStrW::EPT_Printf, L"%02X:%02X:%02X:%02X:%02X:%02X", mac.m_mac[0], mac.m_mac[1], mac.m_mac[2], mac.m_mac[3], mac.m_mac[4], mac.m_mac[5]);
	stream << strMac;
	return stream;
}

//#################################################################################################
bool CMac::operator==(const CMac &mac) const
{
	return (Compare(mac) == 0);
}

//#################################################################################################
bool CMac::operator!=(const CMac &mac) const
{
	return (Compare(mac) != 0);
}

//#################################################################################################
int CMac::Compare(const CMac &mac) const
{
	return std::memcmp(m_mac, mac.m_mac, 6);
}

NS_END

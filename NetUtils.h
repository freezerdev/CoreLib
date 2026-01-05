#pragma once

#include "Mac.h"
#include "Url.h"
#ifdef __APPLE__
#include "StlHelper.h"
#endif
#include <vector>

NS_BEGIN

// Forward declarations
class CMemBuffer;

#ifndef _WIN32
void CurlInit(void);
void CurlFree(void);
#endif

bool IsValidIPv48(const CStr8 &strIP);
bool IsValidIPv4W(const CStrW &strIP);

bool IsValidIPv68(const CStr8 &strIP);
bool IsValidIPv6W(const CStrW &strIP);

bool IsLocallyAdministered(const CMac &mac) noexcept;

bool IPv4FromString8(const CStr8 &strIP, uint32_t &nIPv4Address, const bool bNetworkByteOrder = true);
bool IPv4FromStringW(const CStrW &strIP, uint32_t &nIPv4Address, const bool bNetworkByteOrder = true);

CStr8 IPv4ToString8(const uint32_t nIPv4Address, const bool bNetworkByteOrder = true);
CStrW IPv4ToStringW(const uint32_t nIPv4Address, const bool bNetworkByteOrder = true);
CStr8 IPv6ToString8(PCBYTE pnIPv6Address);
CStrW IPv6ToStringW(PCBYTE pnIPv6Address);

void RemoveScope8(CStr8 &strIP);
void RemoveScopeW(CStrW &strIP);

ERRCODE DownloadFile(const CUrl &url, const CFilePath &pathFile);
ERRCODE DownloadFile(const CUrl &url, CMemBuffer &buf);

void GetPublicIPAddress(CStr &strIPv4, CStr &strIPv6);

// Returns the PID of the process on the other end of the socket or -1 if not found; only works for processes on the same machine
uint32_t GetSocketProcessId(const NSOCKET hSocket);

struct SocketInfo final
{
	CStr8 strSelfAddress;
	CStr8 strPeerAddress;
	uint16_t nSelfPort;
	uint16_t nPeerPort;
	uint32_t nProcessId;
};

// Returns information on all socket connections on the computer
void GetSocketConnections(std::vector<SocketInfo> &vecSocketInfo);

#ifdef __APPLE__
std::unordered_map<CStr, CStr, CStrHashTraits> GetNetworkGateways(void);
std::unordered_map<CStr, CStr, CStrHashTraits> GetNetworkSuffixes(void);
#endif

bool IsIPv4LinkLocal(const CStr &strIP);
bool IsIPv6LinkLocal(const CStr &strIP);

#ifdef _WIN32
#define IsValidIPv4 IsValidIPv4W
#define IsValidIPv6 IsValidIPv6W
#define IPv4FromString IPv4FromStringW
#define IPv4ToString IPv4ToStringW
#define IPv6ToString IPv6ToStringW
#define RemoveScope RemoveScopeW
#else
#define IsValidIPv4 IsValidIPv48
#define IsValidIPv6 IsValidIPv68
#define IPv4FromString IPv4FromString8
#define IPv4ToString IPv4ToString8
#define IPv6ToString IPv6ToString8
#define RemoveScope RemoveScope8
#endif

NS_END

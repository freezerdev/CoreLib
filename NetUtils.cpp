#include "Base.h"
#include "NetUtils.h"
#include "FileSystemUtils.h"
#include "MemBuffer.h"
#ifdef _WIN32
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <winhttp.h>
#elif __APPLE__
#include "Object.h"
#include <SystemConfiguration/SystemConfiguration.h>
#include <curl/curl.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/sysctl.h>
#include <netinet/in_pcb.h>
#include <netinet/tcp.h>
#include <netinet/tcp_fsm.h>
#include <netinet/tcp_timer.h>
#elif __linux__
#include <curl/curl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

NS_BEGIN

// Macros to extract different parts of an IPv4 address
#define IPV4ADDR0(ip)		((BYTE)((ip) >> 24))
#define IPV4ADDR1(ip)		((BYTE)((ip) >> 16))
#define IPV4ADDR2(ip)		((BYTE)((ip) >> 8))
#define IPV4ADDR3(ip)		((BYTE)(ip))

#ifdef __APPLE__
static constexpr auto XSO_SOCKET = 0x001;
static constexpr auto XSO_RCVBUF = 0x002;
static constexpr auto XSO_SNDBUF = 0x004;
static constexpr auto XSO_STATS = 0x008;
static constexpr auto XSO_INPCB = 0x010;
static constexpr auto XSO_TCPCB = 0x020;
static constexpr auto XSO_KCREG = 0x040;
static constexpr auto XSO_KCB = 0x080;
static constexpr auto XSO_EVT = 0x100;

static constexpr auto ALL_XGN_KIND_INP = XSO_SOCKET | XSO_RCVBUF | XSO_SNDBUF | XSO_STATS | XSO_INPCB;
static constexpr auto ALL_XGN_KIND_TCP = ALL_XGN_KIND_INP | XSO_TCPCB;

// Round up to the next 64 byte boundary
#define ROUNDUP64(n)	((n) > 0 ? (1 + (((n) - 1) | (sizeof(uint64_t) - 1))) : sizeof(uint64_t))

struct xgen_n final
{
	uint32_t xgn_len;
	uint32_t xgn_kind;
};

#pragma pack(push, 4)

struct xtcpcb_n final
{
	uint32_t xt_len;
	uint32_t xt_kind;
	uint64_t t_segq;
	int t_dupacks;
	int t_timer[TCPT_NTIMERS_EXT];
	int t_state;
	unsigned int t_flags;
	int t_force;
	tcp_seq snd_una;
	tcp_seq snd_max;
	tcp_seq snd_nxt;
	tcp_seq snd_up;
	tcp_seq snd_wl1;
	tcp_seq snd_wl2;
	tcp_seq iss;
	tcp_seq irs;
	tcp_seq rcv_nxt;
	tcp_seq rcv_adv;
	uint32_t rcv_wnd;
	tcp_seq rcv_up;
	uint32_t snd_wnd;
	uint32_t snd_cwnd;
	uint32_t snd_ssthresh;
	unsigned int t_maxopd;
	uint32_t t_rcvtime;
	uint32_t t_starttime;
	int t_rtttime;
	tcp_seq t_rtseq;
	int t_rxtcur;
	unsigned int t_maxseg;
	int t_srtt;
	int t_rttvar;
	int t_rxtshift;
	unsigned int t_rttmin;
	uint32_t t_rttupdated;
	uint32_t max_sndwnd;
	int t_softerror;
	char t_oobflags;
	char t_iobc;
	uint8_t snd_scale;
	uint8_t rcv_scale;
	uint8_t request_r_scale;
	uint8_t requested_s_scale;
	uint32_t ts_recent;
	uint32_t ts_recent_age;
	tcp_seq last_ack_sent;
	tcp_cc cc_send;
	tcp_cc cc_recv;
	tcp_seq snd_recover;
	uint32_t snd_cwnd_prev;
	uint32_t snd_ssthresh_prev;
};

struct xinpcb_n final
{
	uint32_t xi_len;
	uint32_t xi_kind;
	uint64_t xi_inpp;
	uint16_t inp_fport;
	uint16_t inp_lport;
	uint64_t inp_ppcb;
	inp_gen_t inp_gencnt;
	int inp_flags;
	uint32_t inp_flow;
	uint8_t inp_vflag;
	uint8_t inp_ip_ttl;
	uint8_t inp_ip_p;
	union
	{
		struct in_addr_4in6 inp46_foreign;
		struct in6_addr inp6_foreign;
	} inp_dependfaddr;
	union
	{
		struct in_addr_4in6 inp46_local;
		struct in6_addr inp6_local;
	} inp_dependladdr;
	struct
	{
		uint8_t	inp4_ip_tos;
	} inp_depend4;
	struct
	{
		uint8_t inp6_hlim;
		int inp6_cksum;
		uint16_t inp6_ifindex;
		int16_t inp6_hops;
	} inp_depend6;
	uint32_t inp_flowhash;
	uint32_t inp_flags2;
};

struct xsocket_n final
{
	uint32_t xso_len;
	uint32_t xso_kind;
	uint64_t xso_so;
	int16_t so_type;
	uint32_t so_options;
	int16_t so_linger;
	int16_t so_state;
	uint64_t so_pcb;
	int xso_protocol;
	int xso_family;
	int16_t so_qlen;
	int16_t so_incqlen;
	int16_t so_qlimit;
	int16_t so_timeo;
	uint16_t so_error;
	pid_t so_pgid;
	uint32_t so_oobmark;
	uid_t so_uid;
	pid_t so_last_pid;
	pid_t so_e_pid;
};

#pragma pack(pop)
#endif


#ifndef _WIN32
//#################################################################################################
void CurlInit(void)
{
	curl_global_init(CURL_GLOBAL_ALL);
}

//#################################################################################################
void CurlFree(void)
{
	curl_global_cleanup();
}
#endif

//#################################################################################################
bool GetSocketSelfInfo(const NSOCKET hSocket, CStr8 &strIP, uint16_t &nPort)
{
	bool bReturn = false;

	strIP.Empty();
	nPort = 0;

	if(hSocket != INVALID_NSOCKET)
	{
		sockaddr sai = {0};

#ifdef _WIN32
		int nSize = sizeof(sai);
		if(getsockname(hSocket, &sai, &nSize) != SOCKET_ERROR)
		{
			if(sai.sa_family == AF_INET)
			{
				char sz[NI_MAXHOST] = {0};
				if(getnameinfo(&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
					strIP = sz;

				nPort = ntohs(((sockaddr_in*)&sai)->sin_port);
				bReturn = true;
			}
			else if(sai.sa_family == AF_INET6)
			{
				char sz[NI_MAXHOST] = {0};
				if(getnameinfo(&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
					strIP = sz;

				nPort = ntohs(((sockaddr_in6*)&sai)->sin6_port);
				bReturn = true;
			}
		}
#else
		socklen_t nSize = sizeof(sai);
		if(getsockname(hSocket, &sai, &nSize) != -1)
		{
			if(sai.sa_family == AF_INET)
			{
				char sz[INET_ADDRSTRLEN] = {0};
				if(inet_ntop(AF_INET, &((sockaddr_in*)&sai)->sin_addr, sz, sizeof(sz)))
					strIP = sz;

				nPort = ntohs(((sockaddr_in*)&sai)->sin_port);
				bReturn = true;
			}
			else if(sai.sa_family == AF_INET6)
			{
				char sz[INET6_ADDRSTRLEN] = {0};
				if(inet_ntop(AF_INET6, &((sockaddr_in6*)&sai)->sin6_addr, sz, sizeof(sz)))
					strIP = sz;

				nPort = ntohs(((sockaddr_in6*)&sai)->sin6_port);
				bReturn = true;
			}
		}
#endif
	}

	return bReturn;
}

//#################################################################################################
bool GetSocketPeerInfo(const NSOCKET hSocket, CStr8 &strIP, uint16_t &nPort)
{
	bool bReturn = false;

	strIP.Empty();
	nPort = 0;

	sockaddr sai = {0};

#ifdef _WIN32
	int nSize = sizeof(sai);
	if(getpeername(hSocket, &sai, &nSize) != SOCKET_ERROR)
	{
		if(sai.sa_family == AF_INET)
		{
			char sz[NI_MAXHOST] = {0};
			if(getnameinfo(&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
				strIP = sz;

			nPort = ntohs(((sockaddr_in*)&sai)->sin_port);
			bReturn = true;
		}
		else if(sai.sa_family == AF_INET6)
		{
			char sz[NI_MAXHOST] = {0};
			if(getnameinfo(&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
				strIP = sz;

			nPort = ntohs(((sockaddr_in6*)&sai)->sin6_port);
			bReturn = true;
		}
	}
#else
	socklen_t nSize = sizeof(sai);
	if(getpeername(hSocket, &sai, &nSize) != -1)
	{
		if(sai.sa_family == AF_INET)
		{
			char sz[INET_ADDRSTRLEN] = {0};
			if(inet_ntop(AF_INET, &((sockaddr_in*)&sai)->sin_addr, sz, sizeof(sz)))
				strIP = sz;

			nPort = ntohs(((sockaddr_in*)&sai)->sin_port);
			bReturn = true;
		}
		else if(sai.sa_family == AF_INET6)
		{
			char sz[INET6_ADDRSTRLEN] = {0};
			if(inet_ntop(AF_INET6, &((sockaddr_in6*)&sai)->sin6_addr, sz, sizeof(sz)))
				strIP = sz;

			nPort = ntohs(((sockaddr_in6*)&sai)->sin6_port);
			bReturn = true;
		}
	}
#endif

	return bReturn;
}

//#################################################################################################
void GetSocketConnections(std::vector<SocketInfo> &vecSocketInfo)
{
	vecSocketInfo.clear();

#ifdef _WIN32
	DWORD dwErrorCode;
	DWORD dwSize = 4096;
	auto pBuf = std::make_unique<BYTE[]>(dwSize);
	char sz[NI_MAXHOST] = {0};

	// Process TCP/IP4
	do{
		dwErrorCode = GetExtendedTcpTable(pBuf.get(), &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
		if(dwErrorCode == NO_ERROR)
		{
			PMIB_TCPTABLE_OWNER_PID pTable = (PMIB_TCPTABLE_OWNER_PID)pBuf.get();
			for(DWORD dw = 0; dw < pTable->dwNumEntries; ++dw)
			{
				SocketInfo si;
				sockaddr_in sai = {0};
				sai.sin_family = AF_INET;

				sai.sin_addr.S_un.S_addr = pTable->table[dw].dwLocalAddr;
				if(getnameinfo((sockaddr*)&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
					si.strSelfAddress = sz;

				sai.sin_addr.S_un.S_addr = pTable->table[dw].dwRemoteAddr;
				if(getnameinfo((sockaddr*)&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
					si.strPeerAddress = sz;

				si.nSelfPort = ntohs((uint16_t)pTable->table[dw].dwLocalPort);
				si.nPeerPort = ntohs((uint16_t)pTable->table[dw].dwRemotePort);
				si.nProcessId = pTable->table[dw].dwOwningPid;

				vecSocketInfo.push_back(std::move(si));
			}
		}
		else if(dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
		{
			dwSize += 1024;		// Add extra memory in case new sockets appear between the two calls to GetExtendedTcpTable()
			pBuf = std::make_unique<BYTE[]>(dwSize);
		}
	}while(dwErrorCode == ERROR_INSUFFICIENT_BUFFER);

	// Process TCP/IP6
	do{
		dwErrorCode = GetExtendedTcpTable(pBuf.get(), &dwSize, TRUE, AF_INET6, TCP_TABLE_OWNER_PID_ALL, 0);
		if(dwErrorCode == NO_ERROR)
		{
			PMIB_TCP6TABLE_OWNER_PID pTable = (PMIB_TCP6TABLE_OWNER_PID)pBuf.get();
			for(DWORD dw = 0; dw < pTable->dwNumEntries; ++dw)
			{
				SocketInfo si;
				sockaddr_in6 sai = {0};
				sai.sin6_family = AF_INET6;

				std::memcpy(&sai.sin6_addr.u, pTable->table[dw].ucLocalAddr, 16);
				sai.sin6_scope_id = pTable->table[dw].dwLocalScopeId;
				if(getnameinfo((sockaddr*)&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
					si.strSelfAddress = sz;

				std::memcpy(&sai.sin6_addr.u, pTable->table[dw].ucRemoteAddr, 16);
				sai.sin6_scope_id = pTable->table[dw].dwRemoteScopeId;
				if(getnameinfo((sockaddr*)&sai, sizeof(sai), sz, sizeof(sz), nullptr, 0, NI_NUMERICHOST) == 0)
					si.strPeerAddress = sz;

				si.nSelfPort = ntohs((uint16_t)pTable->table[dw].dwLocalPort);
				si.nPeerPort = ntohs((uint16_t)pTable->table[dw].dwRemotePort);
				si.nProcessId = pTable->table[dw].dwOwningPid;

				vecSocketInfo.push_back(std::move(si));
			}
		}
		else if(dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
		{
			dwSize += 1024;		// Add extra memory in case new sockets appear between the two calls to GetExtendedTcpTable()
			pBuf = std::make_unique<BYTE[]>(dwSize);
		}
	}while(dwErrorCode == ERROR_INSUFFICIENT_BUFFER);
#elif __APPLE__
	size_t nSize = 0;
	int nWhich = 0;
	xtcpcb_n *ptcp = nullptr;
	xinpcb_n *pinp = nullptr;
	xsocket_n *pso = nullptr;
	xgen_n *pxgn = nullptr;
	char szSelfAddress[INET6_ADDRSTRLEN] = {0};
	char szPeerAddress[INET6_ADDRSTRLEN] = {0};

	static PCSTR szMibName = "net.inet.tcp.pcblist_n";

	if(sysctlbyname(szMibName, nullptr, &nSize, nullptr, 0) == -1)
		return;

	auto pBuf = std::make_unique<char[]>(nSize);

	if(sysctlbyname(szMibName, pBuf.get(), &nSize, nullptr, 0) == -1 || nSize <= sizeof(xinpgen))
		return;

	const xinpgen *pxig = (const xinpgen*)pBuf.get();
	for(PSTR pNext = pBuf.get() + ROUNDUP64(pxig->xig_len); pNext < pBuf.get() + nSize; pNext += ROUNDUP64(pxgn->xgn_len))
	{
		pxgn = (xgen_n*)pNext;
		if(pxgn->xgn_len <= sizeof(xinpgen))
			break;

		if((nWhich & pxgn->xgn_kind) == 0)
		{
			nWhich |= pxgn->xgn_kind;
			switch(pxgn->xgn_kind)
			{
			case XSO_SOCKET:
				pso = (xsocket_n*)pxgn;
				break;

			case XSO_INPCB:
				pinp = (xinpcb_n*)pxgn;
				break;

			case XSO_TCPCB:
				ptcp = (xtcpcb_n*)pxgn;
				break;

			default:
				break;
			}
		}

		if(nWhich != ALL_XGN_KIND_TCP)
			continue;
		nWhich = 0;

		if(pso == nullptr || pinp == nullptr || ptcp == nullptr || pso->xso_protocol != IPPROTO_TCP ||
			pinp->inp_gencnt > ((xinpgen*)pBuf.get())->xig_gen ||
			(!HAS_FLAG(pinp->inp_vflag, INP_IPV4) && !HAS_FLAG(pinp->inp_vflag, INP_IPV6)) ||
			ptcp->t_state <= TCPS_LISTEN)
		{
			continue;
		}

		if(HAS_FLAG(pinp->inp_vflag, INP_IPV4))
		{
			inet_ntop(AF_INET, &pinp->inp_laddr, szSelfAddress, sizeof(szSelfAddress));
			inet_ntop(AF_INET, &pinp->inp_faddr, szPeerAddress, sizeof(szPeerAddress));
		}
		else if(HAS_FLAG(pinp->inp_vflag, INP_IPV6))
		{
			inet_ntop(AF_INET6, &(pinp->in6p_laddr), szSelfAddress, sizeof(szSelfAddress));
			inet_ntop(AF_INET6, &(pinp->in6p_faddr), szPeerAddress, sizeof(szPeerAddress));
		}

		SocketInfo si;
		si.strSelfAddress = szSelfAddress;
		si.strPeerAddress = szPeerAddress;
		si.nSelfPort = ntohs(pinp->inp_lport);
		si.nPeerPort = ntohs(pinp->inp_fport);
		si.nProcessId = pso->so_last_pid;

		vecSocketInfo.push_back(std::move(si));
	}
#elif __linux__
	// TODO
#endif
}

//#################################################################################################
bool IsValidIPv48(const CStr8 &strIP)
{
	bool bValid = false;

	if(strIP.Count('.') == 3)
	{
		bValid = true;

		size_t nStart = 0;
		CStr8 strOctet = strIP.Tokenize('.', nStart);
		while(nStart != CStr8::not_found)
		{
			if(strOctet.IsEmpty() || !IsDigitString8(strOctet) || StringToInteger8<size_t>(strOctet) > 255)
			{
				bValid = false;
				break;
			}

			strOctet = strIP.Tokenize('.', nStart);
		}
	}

	return bValid;
}

//#################################################################################################
bool IsValidIPv4W(const CStrW &strIP)
{
	bool bValid = false;

	if(strIP.Count(L'.') == 3)
	{
		bValid = true;

		size_t nStart = 0;
		CStrW strOctet = strIP.Tokenize(L'.', nStart);
		while(nStart != CStrW::not_found)
		{
			if(strOctet.IsEmpty() || !IsDigitStringW(strOctet) || StringToIntegerW<size_t>(strOctet) > 255)
			{
				bValid = false;
				break;
			}

			strOctet = strIP.Tokenize(L'.', nStart);
		}
	}

	return bValid;
}

//#################################################################################################
bool IsValidIPv68(const CStr8 &strIP)
{
	bool bValid = false;

	if(strIP.Count(':') <= 8)
	{
		bValid = true;

		size_t nStart = 0;
		CStr8 strOctet = strIP.Tokenize(':', nStart);
		while(nStart != CStr8::not_found)
		{
			if(strOctet.GetLength() > 4 || !IsHexString8(strOctet))
			{
				bValid = false;
				break;
			}

			strOctet = strIP.Tokenize(':', nStart);
		}
	}

	return bValid;
}

//#################################################################################################
bool IsValidIPv6W(const CStrW &strIP)
{
	bool bValid = false;

	if(strIP.Count(L':') <= 8)
	{
		bValid = true;

		size_t nStart = 0;
		CStrW strOctet = strIP.Tokenize(L':', nStart);
		while(nStart != CStrW::not_found)
		{
			if(strOctet.GetLength() > 4 || !IsHexStringW(strOctet))
			{
				bValid = false;
				break;
			}

			strOctet = strIP.Tokenize(L':', nStart);
		}
	}

	return bValid;
}

//#################################################################################################
bool IsLocallyAdministered(const CMac &mac) noexcept
{
	return HAS_FLAG(mac.Get()[0], 0x2);
}

//#################################################################################################
bool IPv4FromString8(const CStr8 &strIP, uint32_t &nIPv4Address, const bool bNetworkByteOrder)
{
	bool bResult = false;

	nIPv4Address = 0;

	if(strIP.Count('.') == 3)
	{
		bResult = true;

		size_t nStart = 0;
		size_t nShift = 24;
		CStr8 strOctet = strIP.Tokenize('.', nStart);
		while(nStart != CStr8::not_found)
		{
			size_t nOctet;
			if(strOctet.IsEmpty() || !IsDigitString8(strOctet) || (nOctet = StringToInteger8<size_t>(strOctet)) > 255)
			{
				nIPv4Address = 0;
				bResult = false;
				break;
			}

			nIPv4Address |= (nOctet << nShift);
			nShift -= 8;
			strOctet = strIP.Tokenize('.', nStart);
		}

		if(bNetworkByteOrder)
			nIPv4Address = htonl(nIPv4Address);
	}

	return bResult;
}

//#################################################################################################
bool IPv4FromStringW(const CStrW &strIP, uint32_t &nIPv4Address, const bool bNetworkByteOrder)
{
	bool bResult = false;

	nIPv4Address = 0;

	if(strIP.Count(L'.') == 3)
	{
		bResult = true;

		size_t nStart = 0;
		size_t nShift = 24;
		CStrW strOctet = strIP.Tokenize(L'.', nStart);
		while(nStart != CStrW::not_found)
		{
			size_t nOctet;
			if(strOctet.IsEmpty() || !IsDigitStringW(strOctet) || (nOctet = StringToIntegerW<size_t>(strOctet)) > 255)
			{
				nIPv4Address = 0;
				bResult = false;
				break;
			}

			nIPv4Address |= (nOctet << nShift);
			nShift -= 8;
			strOctet = strIP.Tokenize(L'.', nStart);
		}

		if(bNetworkByteOrder)
			nIPv4Address = htonl(nIPv4Address);
	}

	return bResult;
}

//#################################################################################################
CStr8 IPv4ToString8(const uint32_t nIPv4Address, const bool bNetworkByteOrder)
{
	CStr8 strIP;

	uint32_t nIP = (bNetworkByteOrder) ? ntohl(nIPv4Address) : nIPv4Address;
	strIP.Format("{0}.{1}.{2}.{3}", (size_t)IPV4ADDR0(nIP), (size_t)IPV4ADDR1(nIP), (size_t)IPV4ADDR2(nIP), (size_t)IPV4ADDR3(nIP));

	return strIP;
}

//#################################################################################################
CStrW IPv4ToStringW(const uint32_t nIPv4Address, const bool bNetworkByteOrder)
{
	CStrW strIP;

	uint32_t nIP = (bNetworkByteOrder) ? ntohl(nIPv4Address) : nIPv4Address;
	strIP.Format(L"{0}.{1}.{2}.{3}", (size_t)IPV4ADDR0(nIP), (size_t)IPV4ADDR1(nIP), (size_t)IPV4ADDR2(nIP), (size_t)IPV4ADDR3(nIP));

	return strIP;
}

//#################################################################################################
CStr8 IPv6ToString8(PCBYTE pnIPv6Address)
{
	CStr8 strIP;

	if(pnIPv6Address)
	{
		strIP.Printf("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
			MAKEUINT16(pnIPv6Address[0], pnIPv6Address[1]),
			MAKEUINT16(pnIPv6Address[2], pnIPv6Address[3]),
			MAKEUINT16(pnIPv6Address[4], pnIPv6Address[5]),
			MAKEUINT16(pnIPv6Address[6], pnIPv6Address[7]),
			MAKEUINT16(pnIPv6Address[8], pnIPv6Address[9]),
			MAKEUINT16(pnIPv6Address[10], pnIPv6Address[11]),
			MAKEUINT16(pnIPv6Address[12], pnIPv6Address[13]),
			MAKEUINT16(pnIPv6Address[14], pnIPv6Address[15]));
	}

	return strIP;
}

//#################################################################################################
CStrW IPv6ToStringW(PCBYTE pnIPv6Address)
{
	CStrW strIP;

	if(pnIPv6Address)
	{
		strIP.Printf(L"%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
			MAKEUINT16(pnIPv6Address[0], pnIPv6Address[1]),
			MAKEUINT16(pnIPv6Address[2], pnIPv6Address[3]),
			MAKEUINT16(pnIPv6Address[4], pnIPv6Address[5]),
			MAKEUINT16(pnIPv6Address[6], pnIPv6Address[7]),
			MAKEUINT16(pnIPv6Address[8], pnIPv6Address[9]),
			MAKEUINT16(pnIPv6Address[10], pnIPv6Address[11]),
			MAKEUINT16(pnIPv6Address[12], pnIPv6Address[13]),
			MAKEUINT16(pnIPv6Address[14], pnIPv6Address[15]));
	}

	return strIP;
}

//#################################################################################################
void RemoveScope8(CStr8 &strIP)
{
	size_t nPos = strIP.Find('%');
	if(nPos != CStr8::end)
		strIP.Delete(nPos, CStr8::end);	// Remove IPv6 scope
}

//#################################################################################################
void RemoveScopeW(CStrW &strIP)
{
	size_t nPos = strIP.Find(L'%');
	if(nPos != CStrW::end)
		strIP.Delete(nPos, CStrW::end);	// Remove IPv6 scope
}

//#################################################################################################
ERRCODE DownloadFile(const CUrl &url, const CFilePath &pathFile)
{
	ERRCODE nErrorCode;

	CMemBuffer buf;
	nErrorCode = DownloadFile(url, buf);
	if(nErrorCode == FW_NO_ERROR)
	{
		NHANDLE hFile = INVALID_NHANDLE;
		nErrorCode = FileCreate(pathFile.Get(), EFM_CreateWriteOnly, hFile);
		if(nErrorCode == FW_NO_ERROR)
		{
			size_t nBytesToWrite = buf.GetDataSize();
			size_t nOffset = 0;
			while(nBytesToWrite)
			{
				size_t nBytesWritten;
				nErrorCode = FileWrite(hFile, buf.GetBuffer() + nOffset, nBytesToWrite, nBytesWritten);
				if(nErrorCode != FW_NO_ERROR)
					break;
				nBytesToWrite -= nBytesWritten;
				nOffset += nBytesWritten;
			}

			FileClose(hFile);

			if(nErrorCode != FW_NO_ERROR)
				FileDelete(pathFile.Get());
		}
	}

	return nErrorCode;
}

#ifndef _WIN32
//#################################################################################################
size_t CurlBufferCallback(PSTR szData, size_t nIgnored, size_t nDataSize, PVOID pUserData)
{
	Assert(pUserData);

	UNUSED(nIgnored);

	CMemBuffer *pBuf = (CMemBuffer*)pUserData;
	pBuf->Append((PCBYTE)szData, nDataSize);

	return nDataSize;
}

//#################################################################################################
size_t CurlStringCallback(PSTR szData, size_t nIgnored, size_t nDataSize, PVOID pUserData)
{
	Assert(pUserData);

	UNUSED(nIgnored);

	CStr8 *pstr = (CStr8*)pUserData;
	pstr->Append(szData, 0, nDataSize);

	return nDataSize;
}
#endif

//#################################################################################################
ERRCODE DownloadFile(const CUrl &url, CMemBuffer &buf)
{
	ERRCODE nErrorCode = FW_ERROR_NETWORK_ISSUE;

	buf.Empty();

#ifdef _WIN32
	static constexpr auto BUFFER_SIZE = 8192;

	HINTERNET hInternet = WinHttpOpen(L"freezerware-agent/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if(hInternet)
	{
		bool bHttps = (url.GetScheme().Compare(L"https", true) == 0);
		HINTERNET hCon = WinHttpConnect(hInternet, url.GetDomain(), bHttps ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
		if(hCon)
		{
			HINTERNET hReq = WinHttpOpenRequest(hCon, nullptr, url.GetPath(), nullptr, WINHTTP_NO_REFERER, nullptr, bHttps ? WINHTTP_FLAG_SECURE : 0);
			if(hReq)
			{
				if(WinHttpSendRequest(hReq, nullptr, 0, nullptr, 0, 0, 0))
				{
					if(WinHttpReceiveResponse(hReq, nullptr))
					{
						DWORD dwCode;
						DWORD dwSize = sizeof(dwCode);
						if(WinHttpQueryHeaders(hReq, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwCode, &dwSize, WINHTTP_NO_HEADER_INDEX))
						{
							if(dwCode == HTTP_STATUS_OK)
							{
								DWORD dwInternetFileSize;

								dwSize = sizeof(dwInternetFileSize);
								WinHttpQueryHeaders(hReq, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwInternetFileSize, &dwSize, WINHTTP_NO_HEADER_INDEX);

								buf.SetBufferSize(dwInternetFileSize);

								DWORD dwBytesRemaining = dwInternetFileSize;

								DWORD dwBufSize = BUFFER_SIZE;
								PBYTE pBuf = (PBYTE)std::malloc(dwBufSize);
								if(pBuf)
								{
									DWORD dwBytesReady;
									WinHttpQueryDataAvailable(hReq, &dwBytesReady);
									while(dwBytesRemaining && dwBytesReady)
									{
										if(dwBytesReady > dwBufSize)
										{
											PBYTE pBufNew = (PBYTE)realloc(pBuf, dwBytesReady);
											if(pBufNew)
											{
												pBuf = pBufNew;
												dwBufSize = dwBytesReady;
											}
											else
												dwBytesReady = dwBufSize;
										}

										DWORD dwBytesRead = 0;
										if(!WinHttpReadData(hReq, pBuf, dwBytesReady, &dwBytesRead) || dwBytesRead == 0)
											break;
										buf.Append(pBuf, dwBytesRead);

										dwBytesRemaining -= dwBytesRead;
										WinHttpQueryDataAvailable(hReq, &dwBytesReady);
									}

									std::free(pBuf);

									if(dwBytesRemaining == 0)
										nErrorCode = FW_NO_ERROR;
								}
							}
						}
					}
				}

				WinHttpCloseHandle(hReq);
			}

			WinHttpCloseHandle(hCon);
		}

		WinHttpCloseHandle(hInternet);
	}
#else
	CURL *pCurl = curl_easy_init();
	if(pCurl)
	{
		Verify(curl_easy_setopt(pCurl, CURLOPT_URL, (PCSTR)url.Get()) == CURLE_OK);
		Verify(curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CurlBufferCallback) == CURLE_OK);
		Verify(curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (PVOID)&buf) == CURLE_OK);
		Verify(curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "freezerware-agent/1.0") == CURLE_OK);

		if(curl_easy_perform(pCurl) == CURLE_OK)
			nErrorCode = FW_NO_ERROR;

		curl_easy_cleanup(pCurl);
	}
#endif

	return nErrorCode;
}

//#################################################################################################
void GetPublicIPAddress(CStr &strIPv4, CStr &strIPv6)
{
	strIPv4.Empty();
	strIPv6.Empty();

	CUrl urlV4(_N("https://api.ipify.org"));
	CUrl urlV6(_N("https://api64.ipify.org"));

#ifdef _WIN32
	HINTERNET hInternet = WinHttpOpen(L"freezerware-agent/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if(hInternet)
	{	// IPv4
		bool bHttps = (urlV4.GetScheme().Compare(L"https", true) == 0);
		HINTERNET hCon = WinHttpConnect(hInternet, urlV4.GetDomain(), bHttps ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
		if(hCon)
		{
			HINTERNET hReq = WinHttpOpenRequest(hCon, nullptr, urlV4.GetPath(), nullptr, WINHTTP_NO_REFERER, nullptr, bHttps ? WINHTTP_FLAG_SECURE : 0);
			if(hReq)
			{
				if(WinHttpSendRequest(hReq, nullptr, 0, nullptr, 0, 0, 0))
				{
					if(WinHttpReceiveResponse(hReq, nullptr))
					{
						DWORD dwCode;
						DWORD dwSize = sizeof(dwCode);
						if(WinHttpQueryHeaders(hReq, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwCode, &dwSize, WINHTTP_NO_HEADER_INDEX))
						{
							if(dwCode == HTTP_STATUS_OK)
							{
								DWORD dwInternetDataSize;
								dwSize = sizeof(dwInternetDataSize);
								if(WinHttpQueryHeaders(hReq, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwInternetDataSize, &dwSize, WINHTTP_NO_HEADER_INDEX))
								{
									auto szIP = std::make_unique<char[]>(dwInternetDataSize);
									DWORD dwBytesRead = 0;
									if(WinHttpReadData(hReq, szIP.get(), dwInternetDataSize, &dwBytesRead))
									{
										strIPv4.Assign(szIP.get(), 0, dwInternetDataSize);
										if(!IsValidIPv4(strIPv4))
											strIPv4.Empty();
									}
								}
							}
						}
					}
				}

				WinHttpCloseHandle(hReq);
			}

			WinHttpCloseHandle(hCon);
		}

		// IPv6
		bHttps = (urlV6.GetScheme().Compare(L"https", true) == 0);
		hCon = WinHttpConnect(hInternet, urlV6.GetDomain(), bHttps ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
		if(hCon)
		{
			HINTERNET hReq = WinHttpOpenRequest(hCon, nullptr, urlV6.GetPath(), nullptr, WINHTTP_NO_REFERER, nullptr, bHttps ? WINHTTP_FLAG_SECURE : 0);
			if(hReq)
			{
				if(WinHttpSendRequest(hReq, nullptr, 0, nullptr, 0, 0, 0))
				{
					if(WinHttpReceiveResponse(hReq, nullptr))
					{
						DWORD dwCode;
						DWORD dwSize = sizeof(dwCode);
						if(WinHttpQueryHeaders(hReq, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwCode, &dwSize, WINHTTP_NO_HEADER_INDEX))
						{
							if(dwCode == HTTP_STATUS_OK)
							{
								DWORD dwInternetDataSize;
								dwSize = sizeof(dwInternetDataSize);
								if(WinHttpQueryHeaders(hReq, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwInternetDataSize, &dwSize, WINHTTP_NO_HEADER_INDEX))
								{
									auto szIP = std::make_unique<char[]>(dwInternetDataSize);
									DWORD dwBytesRead = 0;
									if(WinHttpReadData(hReq, szIP.get(), dwInternetDataSize, &dwBytesRead))
									{
										strIPv6.Assign(szIP.get(), 0, dwInternetDataSize);
										if(!IsValidIPv6(strIPv6))
											strIPv6.Empty();
									}
								}
							}
						}
					}
				}

				WinHttpCloseHandle(hReq);
			}

			WinHttpCloseHandle(hCon);
		}

		WinHttpCloseHandle(hInternet);
	}
#else
	CURL *pCurl = curl_easy_init();
	if(pCurl)
	{
		Verify(curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CurlBufferCallback) == CURLE_OK);
		Verify(curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "freezerware-agent/1.0") == CURLE_OK);

		Verify(curl_easy_setopt(pCurl, CURLOPT_URL, (PCSTR)urlV4.Get()) == CURLE_OK);
		Verify(curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (PVOID)&strIPv4) == CURLE_OK);
		if(curl_easy_perform(pCurl) == CURLE_OK && !IsValidIPv4(strIPv4))
			strIPv4.Empty();

		Verify(curl_easy_setopt(pCurl, CURLOPT_URL, (PCSTR)urlV6.Get()) == CURLE_OK);
		Verify(curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (PVOID)&strIPv6) == CURLE_OK);
		if(curl_easy_perform(pCurl) == CURLE_OK && !IsValidIPv6(strIPv6))
			strIPv6.Empty();

		curl_easy_cleanup(pCurl);
	}
#endif
}

//#################################################################################################
uint32_t GetSocketProcessId(const NSOCKET hSocket)
{
	uint32_t nClientProcessId = (uint32_t)-1;

	CStr8 strSelfAddress;
	CStr8 strPeerAddress;
	uint16_t nSelfPort;
	uint16_t nPeerPort;
	if(GetSocketSelfInfo(hSocket, strSelfAddress, nSelfPort) && GetSocketPeerInfo(hSocket, strPeerAddress, nPeerPort))
	{
		std::vector<SocketInfo> vecSocketInfo;
		GetSocketConnections(vecSocketInfo);
		for(const auto &itr : vecSocketInfo)
		{	// We are looking for the other end of this socket, so reverse self and peer when looking
			if(itr.nSelfPort == nPeerPort && itr.nPeerPort == nSelfPort && itr.strSelfAddress == strPeerAddress && itr.strPeerAddress == strSelfAddress)
			{
				nClientProcessId = itr.nProcessId;
				break;
			}
		}
	}

	return nClientProcessId;
}

#ifdef __APPLE__
//#################################################################################################
std::unordered_map<CStr, CStr, CStrHashTraits> GetNetworkGateways(void)
{
	std::unordered_map<CStr, CStr, CStrHashTraits> mapInterfaces;

	CCFObject cfStore = SCDynamicStoreCreate(nullptr, CFSTR(AGENT_NAME), nullptr, nullptr);
	if(cfStore)
	{
		CCFObject cfPrefs = SCPreferencesCreate(nullptr, CFSTR(AGENT_NAME), nullptr);
		if(cfPrefs)
		{
			CCFObject cfServices = SCNetworkServiceCopyAll((SCPreferencesRef)cfPrefs.Get());
			if(cfServices)
			{
				CCFObject cfService;
				CCFObject cfServiceID;
				CCFObject cfKey;
				CCFObject cfProp;
				CCFObject cfValue;
				CStr strKey;
				CStr strName;
				CStr strGateway;
				size_t nCount = cfServices.GetArrayCount();
				for(size_t n = 0; n < nCount; ++n)
				{
					cfService = cfServices.GetArrayObject(n);
					if(cfService)
					{
						cfServiceID.Attach(SCNetworkServiceGetServiceID((SCNetworkServiceRef)cfService.Get()), false);
						if(cfServiceID)
						{
							strKey.Format("State:/Network/Service/{0}/IPv4", cfServiceID.GetString());
							cfKey = CFStringCreateWithCString(nullptr, strKey, kCFStringEncodingUTF8);
							if(cfKey)
							{
								cfProp = SCDynamicStoreCopyValue((SCDynamicStoreRef)cfStore.Get(), (CFStringRef)cfKey.Get());
								if(cfProp.GetType() == CFDictionaryGetTypeID())
								{
									cfValue = cfProp.GetDictionary(CFSTR("Router"));
									if(cfValue.GetType() == CFArrayGetTypeID())
									{
										if(cfValue.GetArrayCount())
											strGateway = cfValue.GetArrayObject(0).GetString();
									}
									else if(cfValue.GetType() == CFStringGetTypeID())
										strGateway = cfValue.GetString();

									strName = cfProp.GetDictionaryString(CFSTR("InterfaceName"));
									if(!strName.IsEmpty() && !strGateway.IsEmpty())
										mapInterfaces.emplace(strName, strGateway);

									strGateway.Empty();
								}
							}
						}
					}
				}
			}
		}
	}

	return mapInterfaces;
}

//#################################################################################################
std::unordered_map<CStr, CStr, CStrHashTraits> GetNetworkSuffixes(void)
{
	std::unordered_map<CStr, CStr, CStrHashTraits> mapInterfaces;

	CCFObject cfStore = SCDynamicStoreCreate(nullptr, CFSTR(AGENT_NAME), nullptr, nullptr);
	if(cfStore)
	{
		CCFObject cfPrefs = SCPreferencesCreate(nullptr, CFSTR(AGENT_NAME), nullptr);
		if(cfPrefs)
		{
			CCFObject cfServices = SCNetworkServiceCopyAll((SCPreferencesRef)cfPrefs.Get());
			if(cfServices)
			{
				CCFObject cfService;
				CCFObject cfServiceID;
				CCFObject cfKey;
				CCFObject cfProp;
				CCFObject cfValue;
				CStr strKey;
				CStr strName;
				CStr strSuffix;
				size_t nCount = cfServices.GetArrayCount();
				for(size_t n = 0; n < nCount; ++n)
				{
					cfService = cfServices.GetArrayObject(n);
					if(cfService)
					{
						cfServiceID.Attach(SCNetworkServiceGetServiceID((SCNetworkServiceRef)cfService.Get()), false);
						if(cfServiceID)
						{
							strKey.Format("Setup:/Network/Service/{0}/Interface", cfServiceID.GetString());
							cfKey = CFStringCreateWithCString(nullptr, strKey, kCFStringEncodingUTF8);
							if(cfKey)
							{
								cfProp = SCDynamicStoreCopyValue((SCDynamicStoreRef)cfStore.Get(), (CFStringRef)cfKey.Get());
								strName = cfProp.GetDictionaryString(CFSTR("DeviceName"));
							}

							strKey.Format("State:/Network/Service/{0}/DNS", cfServiceID.GetString());
							cfKey = CFStringCreateWithCString(nullptr, strKey, kCFStringEncodingUTF8);
							if(cfKey)
							{
								cfProp = SCDynamicStoreCopyValue((SCDynamicStoreRef)cfStore.Get(), (CFStringRef)cfKey.Get());
								if(cfProp.GetType() == CFDictionaryGetTypeID())
								{
									cfValue = cfProp.GetDictionary(CFSTR("DomainName"));
									if(cfValue.GetType() == CFArrayGetTypeID())
									{
										if(cfValue.GetArrayCount())
											strSuffix = cfValue.GetArrayObject(0).GetString();
									}
									else if(cfValue.GetType() == CFStringGetTypeID())
										strSuffix = cfValue.GetString();
								}
							}

							if(!strName.IsEmpty() && !strSuffix.IsEmpty())
								mapInterfaces.emplace(strName, strSuffix);

							strName.Empty();
							strSuffix.Empty();
						}
					}
				}
			}
		}
	}

	return mapInterfaces;
}
#endif

//#################################################################################################
bool IsIPv4LinkLocal(const CStr &strIP)
{
	return (strIP.CompareLeft(_N("169.254."), 8) == 0);
}

//#################################################################################################
bool IsIPv6LinkLocal(const CStr &strIP)
{
	return (strIP.CompareLeft(_N("fe80:"), 5, true) == 0);
}

NS_END

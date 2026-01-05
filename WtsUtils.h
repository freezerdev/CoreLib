#pragma once

#include <wtsapi32.h>

NS_BEGIN

void WtsInit(void);

bool WtsGetCurrentSessionId(uint32_t &nSessionId);

bool WtsGetDomainName(CStrW &strDomain, const uint32_t nSessionId = WTS_CURRENT_SESSION);

bool WtsGetUserName(CStrW &strUser, const uint32_t nSessionId = WTS_CURRENT_SESSION);

bool WtsGetLogonTime(FILETIME &ftLogon, const uint32_t nSessionId = WTS_CURRENT_SESSION);

bool WtsIsRemoteLogon(bool &bRemote, const uint32_t nSessionId = WTS_CURRENT_SESSION);

bool WtsGetRemoteName(CStrW &strRemote, const uint32_t nSessionId = WTS_CURRENT_SESSION);

bool WtsGetRemoteAddress(CStrW &strAddress, const uint32_t nSessionId = WTS_CURRENT_SESSION);

NS_END

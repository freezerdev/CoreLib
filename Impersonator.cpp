#include "Base.h"
#include "Impersonator.h"
#include "PlatformUtils.h"
#include <wtsapi32.h>

NS_BEGIN

//#################################################################################################
CImpersonator::CImpersonator(void)
	: m_bImpersonating(false)
{
}

//#################################################################################################
CImpersonator::~CImpersonator(void)
{
	Revert();
}

//#################################################################################################
ERRCODE CImpersonator::Impersonate(const uint32_t nSessionId)
{
	ERRCODE nErrorCode = FW_ERROR_ACCESS_DENIED;

	if(!m_bImpersonating)
	{
		Verify(m_pm.Grant(SE_TCB_NAME));
		Verify(m_pm.Grant(SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME));

		HANDLE hUserToken;
		if(WTSQueryUserToken(nSessionId, &hUserToken) && ImpersonateLoggedOnUser(hUserToken))
		{
			m_bImpersonating = true;
			nErrorCode = FW_NO_ERROR;
		}
		else
			m_pm.RevokeAll();
	}
	else
		nErrorCode = FW_ERROR_BUSY;

	return nErrorCode;
}

//#################################################################################################
bool CImpersonator::IsImpersonating(void) const noexcept
{
	return m_bImpersonating;
}

//#################################################################################################
void CImpersonator::Revert(void)
{
	if(m_bImpersonating)
	{
		Verify(RevertToSelf());
		m_bImpersonating = false;
	}

	m_pm.RevokeAll();
}

NS_END

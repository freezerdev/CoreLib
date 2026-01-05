#pragma once

#include "PrivilegeManager.h"

NS_BEGIN

//#################################################################################################
class CImpersonator final
{
public:
	CImpersonator(void);
	~CImpersonator(void);

	ERRCODE Impersonate(const uint32_t nSessionId);
	bool IsImpersonating(void) const noexcept;
	void Revert(void);

private:
	CPrivilegeManager m_pm;
	bool m_bImpersonating;
};

NS_END

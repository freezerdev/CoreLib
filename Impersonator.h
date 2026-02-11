#pragma once

#include "PrivilegeManager.h"

NS_BEGIN

//#################################################################################################
class CImpersonator final
{
public:
	CImpersonator(void);
	// Copy and move constructors
	CImpersonator(const CImpersonator &src) = delete;
	CImpersonator(CImpersonator &&src) = delete;
	~CImpersonator(void);

	// Copy and move assignment operators
	CImpersonator &operator=(const CImpersonator &src) = delete;
	CImpersonator &operator=(CImpersonator &&src) = delete;

	ERRCODE Impersonate(const uint32_t nSessionId);
	bool IsImpersonating(void) const noexcept;
	void Revert(void);

private:
	CPrivilegeManager m_pm;
	bool m_bImpersonating;
};

NS_END

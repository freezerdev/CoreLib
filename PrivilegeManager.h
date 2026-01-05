#pragma once

#include "StlHelper.h"
#include <set>

NS_BEGIN

//#################################################################################################
class CPrivilegeManager final
{
public:
	CPrivilegeManager(void);
	~CPrivilegeManager(void);

	enum EGrantLevel : uint8_t
	{
		EGL_Thread = 0,	// Grant the privilege to the current thread
		EGL_Process		// Grant the privilege to the current process
	};

	bool Grant(PCNSTR szPrivilege, const EGrantLevel eLevel = EGL_Thread);
	bool Revoke(PCNSTR szPrivilege, const EGrantLevel eLevel = EGL_Thread);
	void RevokeAll(void);

	static bool IsEnabled(PCNSTR szPrivilege, const EGrantLevel eLevel = EGL_Thread);

private:
	std::set<std::pair<CStr, EGrantLevel>> m_setPrivileges;
};


NS_END

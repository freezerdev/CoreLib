#pragma once

#include "StlHelper.h"
#include <set>

NS_BEGIN

//#################################################################################################
class CPrivilegeManager final
{
public:
	CPrivilegeManager(void) = default;
	// Copy and move constructors
	CPrivilegeManager(const CPrivilegeManager &src) = delete;
	CPrivilegeManager(CPrivilegeManager &&src) = delete;
	~CPrivilegeManager(void);

	// Copy and move assignment operators
	CPrivilegeManager &operator=(const CPrivilegeManager &src) = delete;
	CPrivilegeManager &operator=(CPrivilegeManager &&src) = delete;

	enum class EGrantLevel : uint8_t
	{
		Thread,		// Grant the privilege to the current thread
		Process		// Grant the privilege to the current process
	};

	bool Grant(PCNSTR szPrivilege, const EGrantLevel eLevel = EGrantLevel::Thread);
	bool Revoke(PCNSTR szPrivilege, const EGrantLevel eLevel = EGrantLevel::Thread);
	void RevokeAll(void);

	static bool IsEnabled(PCNSTR szPrivilege, const EGrantLevel eLevel = EGrantLevel::Thread);

private:
	std::set<std::pair<CStr, EGrantLevel>> m_setPrivileges;
};


NS_END

#pragma once

NS_BEGIN

//#################################################################################################
class CCritSec final
{
public:
	CCritSec(void);
	CCritSec(const CCritSec &src) = delete;
	CCritSec(CCritSec &&src) = delete;
	~CCritSec(void);

	CCritSec &operator=(const CCritSec &src) = delete;
	CCritSec &operator=(CCritSec &&src) = delete;

	inline void Lock(void) {EnterCriticalSection(&m_cs);}
	inline bool TryLock(void) {return (TryEnterCriticalSection(&m_cs) != FALSE);}
	inline void Unlock(void) {LeaveCriticalSection(&m_cs);}

private:
	CRITICAL_SECTION m_cs;
};


//#################################################################################################
class CCritSecMgr final
{
public:
	explicit CCritSecMgr(CCritSec &cs, const bool bLock = true);
	CCritSecMgr(const CCritSecMgr &src) = delete;
	CCritSecMgr(CCritSecMgr &&src) = delete;
	~CCritSecMgr(void);

	CCritSecMgr &operator=(const CCritSecMgr &src) = delete;
	CCritSecMgr &operator=(CCritSecMgr &&src) = delete;

	void Lock(void);
	bool TryLock(void);
	void Unlock(void);

private:
	CCritSec &m_cs;
	bool m_bLocked;
};

NS_END

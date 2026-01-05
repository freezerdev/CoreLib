#pragma once

NS_BEGIN

//#################################################################################################
class CSrwLock final
{
public:
	CSrwLock(void);
	CSrwLock(const CSrwLock &src) = delete;
	CSrwLock(CSrwLock &&src) = delete;

	CSrwLock &operator=(const CSrwLock &src) = delete;
	CSrwLock &operator=(CSrwLock &&src) = delete;

	inline void LockReader(void) {AcquireSRWLockShared(&m_srw);}
	inline void LockWriter(void) {AcquireSRWLockExclusive(&m_srw);}
	inline bool TryLockReader(void) {return (TryAcquireSRWLockShared(&m_srw) != FALSE);}
	inline bool TryLockWriter(void) {return (TryAcquireSRWLockExclusive(&m_srw) != FALSE);}
	inline void UnlockReader(void) {ReleaseSRWLockShared(&m_srw);}
	inline void UnlockWriter(void) {ReleaseSRWLockExclusive(&m_srw);}

private:
	SRWLOCK m_srw;
};

//#################################################################################################
class CSrwLocker final
{
public:
	enum ELockType : int8_t
	{
		ELT_WriterLock = -1,
		ELT_NoLock = 0,
		ELT_ReaderLock = 1
	};

	explicit CSrwLocker(CSrwLock &srw, const ELockType eType = ELT_NoLock);
	~CSrwLocker(void);

	ELockType GetLockStatus(void) const noexcept;

	void LockReader(void);
	void LockWriter(void);
	bool TryLockReader(void);
	bool TryLockWriter(void);
	void UnlockReader(void);
	void UnlockWriter(void);

private:
	CSrwLock &m_srw;
	ELockType m_eType;
};

NS_END

#pragma once

NS_BEGIN

//#################################################################################################
class CSrwLock final : private SRWLOCK
{
public:
	CSrwLock(void);
	CSrwLock(const CSrwLock &src) = delete;
	CSrwLock(CSrwLock &&src) = delete;

	CSrwLock &operator=(const CSrwLock &src) = delete;
	CSrwLock &operator=(CSrwLock &&src) = delete;

	inline void LockReader(void) {AcquireSRWLockShared(this);}
	inline void LockWriter(void) {AcquireSRWLockExclusive(this);}
	inline bool TryLockReader(void) {return (TryAcquireSRWLockShared(this) != FALSE);}
	inline bool TryLockWriter(void) {return (TryAcquireSRWLockExclusive(this) != FALSE);}
	inline void UnlockReader(void) {ReleaseSRWLockShared(this);}
	inline void UnlockWriter(void) {ReleaseSRWLockExclusive(this);}
};

//#################################################################################################
class CSrwLocker final
{
public:
	enum class ELockType : int8_t
	{
		WriterLock = -1,
		NoLock = 0,
		ReaderLock = 1
	};

	explicit CSrwLocker(CSrwLock &srw, const ELockType eType = ELockType::NoLock);
	CSrwLocker(const CSrwLocker &src) = delete;
	CSrwLocker(CSrwLocker &&src) = delete;
	~CSrwLocker(void);

	CSrwLocker &operator=(const CSrwLocker &src) = delete;
	CSrwLocker &operator=(CSrwLocker &&src) = delete;

	ELockType GetLockStatus(void) const noexcept;

	void LockReader(void);
	void LockWriter(void);
	bool TryLockReader(void);
	bool TryLockWriter(void);
	void UnlockReader(void);
	void UnlockWriter(void);

private:
	CSrwLock &m_srw;
	ELockType m_eType = ELockType::NoLock;
};

NS_END

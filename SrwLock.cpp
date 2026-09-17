#include "Base.h"
#include "SrwLock.h"

NS_BEGIN

/////////////////////////////////////////////////
// CSrwLock
/////////////////////////////////////////////////

//#################################################################################################
CSrwLock::CSrwLock(void)
	: SRWLOCK(SRWLOCK_INIT)
{
}


/////////////////////////////////////////////////
// CSrwLocker
/////////////////////////////////////////////////

//#################################################################################################
CSrwLocker::CSrwLocker(CSrwLock &srw, const ELockType eType)
	: m_srw(srw)
{
	if(eType == ELockType::ReaderLock)
		LockReader();
	else if(eType == ELockType::WriterLock)
		LockWriter();
}

//#################################################################################################
CSrwLocker::~CSrwLocker(void)
{
	if(m_eType == ELockType::ReaderLock)
		UnlockReader();
	else if(m_eType == ELockType::WriterLock)
		UnlockWriter();
}

//#################################################################################################
CSrwLocker::ELockType CSrwLocker::GetLockStatus(void) const noexcept
{
	return m_eType;
}

//#################################################################################################
void CSrwLocker::LockReader(void)
{
	Assert(m_eType == ELockType::NoLock);

	m_srw.LockReader();
	m_eType = ELockType::ReaderLock;
}

//#################################################################################################
void CSrwLocker::LockWriter(void)
{
	Assert(m_eType == ELockType::NoLock);

	m_srw.LockWriter();
	m_eType = ELockType::WriterLock;
}

//#################################################################################################
bool CSrwLocker::TryLockReader(void)
{
	Assert(m_eType == ELockType::NoLock);

	if(m_srw.TryLockReader())
	{
		m_eType = ELockType::ReaderLock;
		return true;
	}
	else
		return false;
}

//#################################################################################################
bool CSrwLocker::TryLockWriter(void)
{
	Assert(m_eType == ELockType::NoLock);

	if(m_srw.TryLockWriter())
	{
		m_eType = ELockType::WriterLock;
		return true;
	}
	else
		return false;
}

//#################################################################################################
void CSrwLocker::UnlockReader(void)
{
	Assert(m_eType == ELockType::ReaderLock);

	m_srw.UnlockReader();
	m_eType = ELockType::NoLock;
}

//#################################################################################################
void CSrwLocker::UnlockWriter(void)
{
	Assert(m_eType == ELockType::WriterLock);

	m_srw.UnlockWriter();
	m_eType = ELockType::NoLock;
}

NS_END

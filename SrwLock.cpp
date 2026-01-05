#include "Base.h"
#include "SrwLock.h"

NS_BEGIN

/////////////////////////////////////////////////
// CSrwLock
/////////////////////////////////////////////////

//#################################################################################################
CSrwLock::CSrwLock(void)
	: m_srw(SRWLOCK_INIT)
{
}

//#################################################################################################
CSrwLocker::CSrwLocker(CSrwLock &srw, const ELockType eType)
	: m_srw(srw),
	m_eType(ELT_NoLock)
{
	if(eType == ELT_ReaderLock)
		LockReader();
	else if(eType == ELT_WriterLock)
		LockWriter();
}

//#################################################################################################
CSrwLocker::~CSrwLocker(void)
{
	if(m_eType == ELT_ReaderLock)
		UnlockReader();
	else if(m_eType == ELT_WriterLock)
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
	Assert(m_eType == ELT_NoLock);

	m_srw.LockReader();
	m_eType = ELT_ReaderLock;
}

//#################################################################################################
void CSrwLocker::LockWriter(void)
{
	Assert(m_eType == ELT_NoLock);

	m_srw.LockWriter();
	m_eType = ELT_WriterLock;
}

//#################################################################################################
bool CSrwLocker::TryLockReader(void)
{
	Assert(m_eType == ELT_NoLock);

	if(m_srw.TryLockReader())
	{
		m_eType = ELT_ReaderLock;
		return true;
	}
	else
		return false;
}

//#################################################################################################
bool CSrwLocker::TryLockWriter(void)
{
	Assert(m_eType == ELT_NoLock);

	if(m_srw.TryLockWriter())
	{
		m_eType = ELT_WriterLock;
		return true;
	}
	else
		return false;
}

//#################################################################################################
void CSrwLocker::UnlockReader(void)
{
	Assert(m_eType == ELT_ReaderLock);

	m_srw.UnlockReader();
	m_eType = ELT_NoLock;
}

//#################################################################################################
void CSrwLocker::UnlockWriter(void)
{
	Assert(m_eType == ELT_WriterLock);

	m_srw.UnlockWriter();
	m_eType = ELT_NoLock;
}

NS_END

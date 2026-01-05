#include "Base.h"
#include "Event.h"
#ifdef __APPLE__
#include <sys/time.h>
#endif

NS_BEGIN

//#################################################################################################
CEvent::CEvent(const bool bSignaled)
#ifndef _WIN32
	: m_bSignaled(bSignaled)
#endif
{
#ifdef _WIN32
	m_hEvent = CreateEventW(nullptr, true, bSignaled, nullptr);
#endif
}

//#################################################################################################
CEvent::~CEvent(void)
{
	Signal();
#ifdef _WIN32
	CloseHandle(m_hEvent);
#endif
}

//#################################################################################################
void CEvent::Signal(void)
{
#ifdef _WIN32
	SetEvent(m_hEvent);
#else
	std::unique_lock<std::mutex> lock(m_mutex);
	m_bSignaled = true;
	lock.unlock();
	m_cv.notify_all();
#endif
}

//#################################################################################################
void CEvent::Reset(void)
{
#ifdef _WIN32
	ResetEvent(m_hEvent);
#else
	std::lock_guard<std::mutex> lock(m_mutex);
	m_bSignaled = false;
#endif
}

//#################################################################################################
bool CEvent::Wait(const size_t nMilliseconds)
{
	bool bSignaled = true;

#ifdef _WIN32
	if(WaitForSingleObjectEx(m_hEvent, (DWORD)nMilliseconds, false) != WAIT_OBJECT_0)
		bSignaled = false;
#else
	if(nMilliseconds == INFINITE_WAIT)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while(!m_bSignaled)
			m_cv.wait(lock);
	}
	else
	{
		struct timespec tsWait = {0};
#ifdef __APPLE__
		timeval tv;
		gettimeofday(&tv, nullptr);
		tsWait.tv_sec = tv.tv_sec;
		tsWait.tv_nsec = tv.tv_usec * 1000;
#elif __linux__
		clock_gettime(CLOCK_REALTIME, &tsWait);
#endif

		uint64_t nNS = ((nMilliseconds % 1000) * 1000000) + tsWait.tv_nsec;
		tsWait.tv_sec += (nMilliseconds / 1000) + (nNS / 1000000000);
		tsWait.tv_nsec = nNS % 1000000000;

		std::lock_guard<std::mutex> lock(m_mutex);
		while(!m_bSignaled)
		{
			if(pthread_cond_timedwait(m_cv.native_handle(), m_mutex.native_handle(), &tsWait) == ETIMEDOUT)
			{
				bSignaled = false;
				break;
			}
		}
	}
#endif

	return bSignaled;
}

#ifdef _WIN32
//#################################################################################################
CEvent::operator HANDLE(void) const noexcept
{
	return m_hEvent;
}
#endif

NS_END

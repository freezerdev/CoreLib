#include "Base.h"
#include "Event.h"

NS_BEGIN

//#################################################################################################
CEvent::CEvent(const bool bSignaled)
	: m_bSignaled(bSignaled)
{
}

//#################################################################################################
CEvent::~CEvent(void)
{
	Signal();
}

//#################################################################################################
void CEvent::Signal(void)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_bSignaled = true;
	lock.unlock();
	m_cv.notify_all();
}

//#################################################################################################
void CEvent::Reset(void)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_bSignaled = false;
}

//#################################################################################################
bool CEvent::Wait(const size_t nMilliseconds)
{
	bool bSignaled = true;

	if(nMilliseconds == INFINITE_WAIT)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this](void){return m_bSignaled;});
	}
	else
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		bSignaled = m_cv.wait_for(lock, std::chrono::milliseconds(nMilliseconds), [this](void){return m_bSignaled;});
	}

	return bSignaled;
}

NS_END

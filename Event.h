#pragma once

#ifndef _WIN32
#include <condition_variable>
#include <mutex>
#endif

NS_BEGIN

static constexpr auto INFINITE_WAIT = (size_t)-1;

//#################################################################################################
class CEvent final
{
public:
	explicit CEvent(const bool bSignaled = false);
	CEvent(const CEvent &src) = delete;
	CEvent(CEvent &&src) = delete;
	~CEvent(void);

	CEvent &operator=(const CEvent &src) = delete;
	CEvent &operator=(CEvent &&src) = delete;

	void Signal(void);
	void Reset(void);
	// Waits for the event to be signaled, returns true if signaled or false if timed out
	bool Wait(const size_t nMilliseconds = INFINITE_WAIT);

#ifdef _WIN32
	operator HANDLE(void) const noexcept;
#endif

private:
#ifdef _WIN32
	HANDLE m_hEvent;
#else
	std::condition_variable m_cv;
	std::mutex m_mutex;
	bool m_bSignaled;
#endif
};

NS_END

#pragma once

#include "Base.h"
#include <condition_variable>
#include <mutex>
#include <queue>

NS_BEGIN

//#################################################################################################
template<typename T>
class CBlockingQueue
{
public:
	CBlockingQueue(void) : m_bQuit(false) {}
	// Copy and move constructors
	CBlockingQueue(const CBlockingQueue &src) = delete;
	CBlockingQueue(CBlockingQueue &&src) = delete;

	virtual ~CBlockingQueue(void)
	{
		Shutdown();
	}

	// Copy and move assignment operators
	CBlockingQueue &operator=(const CBlockingQueue &src) = delete;
	CBlockingQueue &operator=(CBlockingQueue &&src) = delete;

	bool IsEmpty(void) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_q.empty();
	}

	void Shutdown(void)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while(!m_q.empty())
			m_q.pop();
		m_bQuit = true;
		lock.unlock();
		m_cv.notify_all();
	}

	void Reset(void)
	{
		Assert(m_q.empty());
		m_bQuit = false;
	}

	void Push(const T &t)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Assert(!m_bQuit);
		m_q.push(t);
		lock.unlock();
		m_cv.notify_one();
	}

	template<typename ...ARGS>
	void Emplace(ARGS&&... args)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Assert(!m_bQuit);
		m_q.emplace(std::forward<ARGS>(args)...);
		lock.unlock();
		m_cv.notify_one();
	}

	void Push(T &&t)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Assert(!m_bQuit);
		m_q.push(std::move(t));
		lock.unlock();
		m_cv.notify_one();
	}

	bool Pop(T &t)
	{
		bool bSuccess = false;
		std::unique_lock<std::mutex> lock(m_mutex);
		while(m_q.empty() && !m_bQuit)
			m_cv.wait(lock);

		if(!m_q.empty())
		{
			t = m_q.front();
			m_q.pop();
			bSuccess = true;
		}

		return bSuccess;
	}

	bool PopNoWait(T &t)
	{
		bool bSuccess = false;
		std::lock_guard<std::mutex> lock(m_mutex);
		if(!m_q.empty())
		{
			t = m_q.front();
			m_q.pop();
			bSuccess = true;
		}

		return bSuccess;
	}

	size_t GetQueueSize(void) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_q.size();
	}

private:
	std::queue<T> m_q;
	mutable std::mutex m_mutex;
	std::condition_variable m_cv;
	bool m_bQuit;
};

NS_END

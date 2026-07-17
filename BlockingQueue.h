#pragma once

#include "Base.h"
#include <condition_variable>
#include <mutex>
#include <deque>

NS_BEGIN

//#################################################################################################
template<typename T>
class CBlockingQueue
{
public:
	CBlockingQueue(void) = default;
	// Copy and move constructors
	CBlockingQueue(const CBlockingQueue &src) = delete;
	CBlockingQueue(CBlockingQueue &&src) = delete;

	virtual ~CBlockingQueue(void) noexcept
	{
		Shutdown();
	}

	// Copy and move assignment operators
	CBlockingQueue &operator=(const CBlockingQueue &src) = delete;
	CBlockingQueue &operator=(CBlockingQueue &&src) = delete;

	bool IsEmpty(void) const noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_q.empty();
	}

	void Shutdown(void) noexcept
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_q.clear();
		m_bQuit = true;
		lock.unlock();
		m_cv.notify_all();
	}

	void Reset(void) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		Assert(m_q.empty());
		m_bQuit = false;
	}

	void Push(const T &t)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Assert(!m_bQuit);
		m_q.push_back(t);
		lock.unlock();
		m_cv.notify_one();
	}

	template<typename ...ARGS>
	void Emplace(ARGS&&... args)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Assert(!m_bQuit);
		m_q.emplace_back(std::forward<ARGS>(args)...);
		lock.unlock();
		m_cv.notify_one();
	}

	void Push(T &&t)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Assert(!m_bQuit);
		m_q.push_back(std::move(t));
		lock.unlock();
		m_cv.notify_one();
	}

	bool Pop(T &t)
	{
		bool bSuccess = false;
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this](void){return m_bQuit || !m_q.empty();});

		if(!m_q.empty())
		{
			t = std::move(m_q.front());
			m_q.pop_front();
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
			t = std::move(m_q.front());
			m_q.pop_front();
			bSuccess = true;
		}

		return bSuccess;
	}

	size_t GetQueueSize(void) const noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_q.size();
	}

private:
	std::deque<T> m_q;
	mutable std::mutex m_mutex;
	std::condition_variable m_cv;
	bool m_bQuit = false;
};

NS_END

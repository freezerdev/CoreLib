#pragma once

#include "Event.h"
#include "StlHelper.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_map>

NS_BEGIN

using PFNPROC = void (*)(PVOID);

//#################################################################################################
class CThread
{
public:
	explicit CThread(const CStr &strThreadName);
	// Copy and move constructors
	CThread(const CThread &src) = delete;
	CThread(CThread &&src) = delete;
	virtual ~CThread(void);

	// Copy and move assignment operators
	CThread &operator=(const CThread &src) = delete;
	CThread &operator=(CThread &&src) = delete;

	// Spawns a new thread and executes 'ThreadProc' - use this for classes derived from CThread
	bool Start(void);

	// Spawns a new thread and executes 'pfnProc' - use without having to derived from CThread
	bool Start(PFNPROC pfnProc, PVOID pParam = nullptr);
	void Wait(void);
	void Reset(void);
	const CStr &GetName(void) const noexcept;
	std::thread::native_handle_type GetHandle(void) const;
	std::thread::id GetId(void) const;
	bool IsActive(void) const noexcept;

	static CStr GetThreadName(const std::thread::id &id = std::thread::id());

	// Attaches to an existing thread and executes 'pfnProc' - adds crash minidump support to an existing thread
	static bool MonitorThread(const CStr &strThreadName, PFNPROC pfnProc, PVOID pParam = nullptr);

private:
	CStr m_strThreadName;
	std::unique_ptr<std::thread> m_pThread;
	mutable std::mutex m_mutexThread;
	PFNPROC m_pfnProc = nullptr;
	PVOID m_pParam = nullptr;
	CEvent m_event;
	std::atomic_bool m_bActive{false};

	static std::unordered_map<size_t, PCNSTR, CNumberHashTraits<size_t>> s_mapThreads;
	static std::mutex s_mutexThreads;

	virtual void ThreadProc(void);

	static bool AddThread(const size_t nThreadId, PCNSTR szThreadName);
	static void RemoveThread(const size_t nThreadId);
	static void InternalThreadProc(CThread *pThis);
};

NS_END

#include "Base.h"
#include "Thread.h"
#include "ThreadUtils.h"
#include "ExceptionUtils.h"
#ifndef _WIN32
#include "TimeUtils.h"
#endif

NS_BEGIN

std::unordered_map<size_t, PCNSTR, CNumberHashTraits<size_t>> CThread::s_mapThreads;
std::mutex CThread::s_mutexThreads;

//#################################################################################################
CThread::CThread(const CStr &strThreadName)
	: m_strThreadName(strThreadName),
	m_pfnProc(nullptr),
	m_pParam(nullptr),
	m_bActive(false)
{
}

//#################################################################################################
CThread::~CThread(void)
{
	Wait();
}

//#################################################################################################
bool CThread::Start(void)
{
	bool bStarted = false;

	std::lock_guard<std::mutex> lock(m_mutexThread);
	if(!m_pThread)
	{
		m_pThread = std::make_unique<std::thread>(InternalThreadProc, this);
		m_bActive = true;
		m_event.Signal();	// Signal the thread that m_pThread has finished initialization
		bStarted = true;
	}

	return bStarted;
}

//#################################################################################################
bool CThread::Start(PFNPROC pfnProc, PVOID pParam)
{
	bool bStarted = false;

	std::lock_guard<std::mutex> lock(m_mutexThread);
	if(!m_pThread && pfnProc)
	{
		m_pfnProc = pfnProc;
		m_pParam = pParam;
		m_pThread = std::make_unique<std::thread>(InternalThreadProc, this);
		m_bActive = true;
		m_event.Signal();	// Signal the thread that m_pThread has finished initialization
		bStarted = true;
	}

	return bStarted;
}

//#################################################################################################
void CThread::Wait(void)
{
	std::lock_guard<std::mutex> lock(m_mutexThread);
	if(m_pThread && m_pThread->joinable())
		m_pThread->join();
}

//#################################################################################################
void CThread::Reset(void)
{
	std::lock_guard<std::mutex> lock(m_mutexThread);
	if(m_pThread)
	{
		if(m_pThread->joinable())
			m_pThread->join();

		m_pThread.reset();
		m_event.Reset();
	}
}

//#################################################################################################
const CStr &CThread::GetName(void) const noexcept
{
	return m_strThreadName;
}

//#################################################################################################
std::thread::native_handle_type CThread::GetHandle(void) const
{
	std::lock_guard<std::mutex> lock(m_mutexThread);
#ifndef __linux__
	return (m_pThread) ? m_pThread->native_handle() : nullptr;
#else
	return (m_pThread) ? m_pThread->native_handle() : 0;
#endif
}

//#################################################################################################
std::thread::id CThread::GetId(void) const
{
	std::lock_guard<std::mutex> lock(m_mutexThread);
	return (m_pThread) ? m_pThread->get_id() : std::thread::id();
}

//#################################################################################################
bool CThread::IsActive(void) const noexcept
{
	return m_bActive;
}

//#################################################################################################
PCNSTR CThread::GetThreadName(const std::thread::id &id)
{
	PCNSTR szThreadName = nullptr;

	size_t nThreadId = GetThreadId(id);

	std::unique_lock<std::mutex> lock(s_mutexThreads);
	auto itr = s_mapThreads.find(nThreadId);
	if(itr != s_mapThreads.end())
		szThreadName = itr->second;
	lock.unlock();

	return szThreadName;
}

//#################################################################################################
bool CThread::MonitorThread(const CStr &strThreadName, PFNPROC pfnProc, PVOID pParam)
{
	bool bMonitored = false;

	if(pfnProc == nullptr)
		return bMonitored;

	size_t nThreadId = GetThreadId();

	bMonitored = AddThread(nThreadId, strThreadName);
	if(bMonitored)
	{
#ifdef _WIN32
		__try{
			SetThreadExceptionHandlers();
#else
		try{
#endif
			pfnProc(pParam);
#ifdef _WIN32
		}
		__except(CreateMiniDump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
		{
			throw std::exception();	// Throw an exception to ensure the program crashes
		}
#else
		}
		catch(const std::exception &e)
		{
			CStr8 strDetails = "Unhandled standard exception!  Time: " + GetFormattedTime() + ", Thread: " + strThreadName + ", Details: " + e.what() + g_szEol;
			CreateMiniDump(strDetails);
			throw;	// Rethrow the exception to ensure the program crashes
		}
		catch(...)
		{
			CStr8 strDetails = "Unhandled exception!  Time: " + GetFormattedTime() + ", Thread: " + strThreadName + g_szEol;
			CreateMiniDump(strDetails);
			throw;	// Rethrow the exception to ensure the program crashes
		}
#endif

		RemoveThread(nThreadId);
	}

	return bMonitored;
}

//#################################################################################################
void CThread::ThreadProc(void)
{
}

//#################################################################################################
bool CThread::AddThread(const size_t nThreadId, PCNSTR szThreadName)
{
	bool bAdded = false;

	std::unique_lock<std::mutex> lock(s_mutexThreads);
	auto itr = s_mapThreads.find(nThreadId);
	if(itr == s_mapThreads.end())
	{
		s_mapThreads[nThreadId] = szThreadName;
		bAdded = true;
	}
	lock.unlock();

	return bAdded;
}

//#################################################################################################
void CThread::RemoveThread(const size_t nThreadId)
{
	std::lock_guard<std::mutex> lock(s_mutexThreads);
	auto itr = s_mapThreads.find(nThreadId);
	if(itr != s_mapThreads.end())
		s_mapThreads.erase(itr);
}

//#################################################################################################
void CThread::InternalThreadProc(CThread *pThis)
{
	if(pThis)
	{
		pThis->m_event.Wait();	// Wait for the spawning thread to finish initialization of the m_pThread object

		size_t nThreadId = GetThreadId();
		PCNSTR szThreadName = pThis->GetName();
		AddThread(nThreadId, szThreadName);

#ifdef _WIN32
		__try{
			SetThreadExceptionHandlers();
#else
		try{
#endif
			if(pThis->m_pfnProc)
				pThis->m_pfnProc(pThis->m_pParam);
			else
				pThis->ThreadProc();
#ifdef _WIN32
		}
		__except(CreateMiniDump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
		{
			throw std::exception();	// Throw an exception to ensure the program crashes
		}
#else
		}
		catch(const std::exception &e)
		{
			CStr8 strDetails = "Unhandled standard exception!  Time: " + GetFormattedTime() + ", Thread: " + pThis->m_strThreadName + ", Details: " + e.what() + g_szEol;
			CreateMiniDump(strDetails);
			std::cerr << strDetails;
			throw;	// Rethrow the exception to ensure the program crashes
		}
		catch(...)
		{
			CStr8 strDetails = "Unhandled exception!  Time: " + GetFormattedTime() + ", Thread: " + pThis->m_strThreadName + g_szEol;
			CreateMiniDump(strDetails);
			std::cerr << strDetails;
			throw;	// Rethrow the exception to ensure the program crashes
		}
#endif

		RemoveThread(nThreadId);
		pThis->m_bActive = false;
	}
}

NS_END

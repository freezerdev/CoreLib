#include "Base.h"
#include "ThreadUtils.h"
#include <sstream>
#ifndef _WIN32
#include <pthread.h>
#endif

NS_BEGIN

size_t OpaqueThreadIdToThreadId(const std::thread::id &id);
#ifndef _WIN32
size_t OpaqueThreadIdToThreadId(const pthread_t &id);
#endif

//#################################################################################################
size_t GetThreadId(const std::thread::id &id)
{
	size_t nThreadId = 0;

	if(id == std::thread::id())
	{	// Get current thread Id
#ifdef _WIN32
		nThreadId = GetCurrentThreadId();
#else
		nThreadId = OpaqueThreadIdToThreadId(pthread_self());
#endif
	}
	else
	{	// Get Id for provided thread
		nThreadId = OpaqueThreadIdToThreadId(id);
	}

	return nThreadId;
}

//#################################################################################################
size_t OpaqueThreadIdToThreadId(const std::thread::id &id)
{
	std::ostringstream stream;
	stream << id;
	return StringToInteger8<size_t>(stream.str().c_str());
}

#ifndef _WIN32
//#################################################################################################
size_t OpaqueThreadIdToThreadId(const pthread_t &id)
{
	std::ostringstream stream;
	stream << id;
	return StringToInteger8<size_t>(stream.str().c_str());
}
#endif

NS_END

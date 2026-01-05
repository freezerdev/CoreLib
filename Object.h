#pragma once

#include <CoreFoundation/CFBase.h>
#include <IOKit/IOKitLib.h>
#include "MemBuffer.h"

NS_BEGIN

//#################################################################################################
class CCFObject final
{
public:
	CCFObject(void);
	CCFObject(const CCFObject &src);
	CCFObject(CCFObject &&src) noexcept;
	CCFObject(const CFTypeRef cf, const bool bAutoRelease = true);
	~CCFObject(void);

	bool IsValid(void) const noexcept;
	void Release(void);

	CCFObject &Attach(const CFTypeRef cf, const bool bAutoRelease = true);
	CFTypeRef Detach(bool *pbAutoRelease = nullptr) noexcept;

	operator CFTypeRef(void) const noexcept;
	CFTypeRef Get(void) const noexcept;
	CFTypeRef &Receive(const bool bAutoRelease = true) noexcept;		// A form of 'Get' which allows the underlying CFTypeRef be set

	CCFObject &operator=(const CCFObject &src);
	CCFObject &operator=(CCFObject &&src) noexcept;

	CFTypeID GetType(void) const;

	bool GetBoolean(const bool bDefault = false) const;
	int64_t GetInteger(const int64_t nDefault = 0) const;
	CStr GetString(PCSTR szDefault = nullptr) const;
	CMemBuffer GetData(void) const;

	size_t GetArrayCount(void) const;
	CCFObject GetArrayObject(const size_t nIndex) const;

	CCFObject GetDictionaryEx(PCSTR szProperty) const;
	bool GetDictionaryBooleanEx(PCSTR szProperty, const bool bDefault = false) const;
	int64_t GetDictionaryIntegerEx(PCSTR szProperty, const int64_t nDefault = 0) const;
	CStr GetDictionaryStringEx(PCSTR szProperty, PCSTR szDefault = nullptr) const;
	CMemBuffer GetDictionaryDataEx(PCSTR szProperty) const;

	//#########################################################################
	template<typename T>
	CCFObject GetDictionary(T&& cfProperty) const
	{
		CCFObject cf;

		if(GetType() == CFDictionaryGetTypeID())
			cf.Attach(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfProperty), false);

		return cf;
	}

	//#########################################################################
	template<typename T>
	bool GetDictionaryBoolean(T&& cfProperty, const bool bDefault = false) const
	{
		bool b = bDefault;

		if(GetType() == CFDictionaryGetTypeID())
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfProperty), false);
			b = cfValue.GetBoolean();
		}

		return b;
	}

	//#########################################################################
	template<typename T>
	int64_t GetDictionaryInteger(T&& cfProperty, const int64_t nDefault = 0) const
	{
		int64_t n = nDefault;

		if(GetType() == CFDictionaryGetTypeID())
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfProperty), false);
			n = cfValue.GetInteger();
		}

		return n;
	}

	//#########################################################################
	template<typename T>
	CStr GetDictionaryString(T&& cfProperty, PCSTR szDefault = nullptr) const
	{
		CStr str(szDefault);

		if(GetType() == CFDictionaryGetTypeID())
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfProperty), false);
			str = cfValue.GetString();
		}

		return str;
	}

	//#########################################################################
	template<typename T>
	CMemBuffer GetDictionaryData(T&& cfProperty) const
	{
		CMemBuffer buf;

		if(GetType() == CFDictionaryGetTypeID())
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfProperty), false);
			buf = cfValue.GetData();
		}

		return buf;
	}

	//#########################################################################
	template<typename T, typename ...ARGS>
	static CCFObject CreateWithFormat(T&& cfFormat, const ARGS&... args)
	{
		return CFStringCreateWithFormat(nullptr, nullptr, cfFormat, args...);
	}

private:
	CFTypeRef m_cf;
	bool m_bAutoRelease;
};


//#################################################################################################
class CIOObject final
{
public:
	CIOObject(void);
	CIOObject(const CIOObject &src);
	CIOObject(CIOObject &&src) noexcept;
	CIOObject(const io_object_t io);
	~CIOObject(void);

	bool IsValid(void) const noexcept;
	void Release(void);

	CIOObject &Attach(io_object_t io);
	io_object_t Detach(void) noexcept;

	operator io_object_t(void) const noexcept;
	io_object_t Get(void) const noexcept;
	io_object_t &Receive(void) noexcept;		// A form of 'Get' which allows the underlying io_object_t be set

	CIOObject &operator=(const CIOObject &src);
	CIOObject &operator=(CIOObject &&src) noexcept;

	CStr GetName(void) const;

private:
	io_object_t m_io;
};

NS_END
